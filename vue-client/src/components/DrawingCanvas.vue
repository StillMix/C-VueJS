<template>
  <div class="drawing-canvas" ref="canvasContainer">
    <!-- Подложка с изображением -->
    <canvas
      ref="imageCanvas"
      class="drawing-canvas__image"
      :width="canvasWidth"
      :height="canvasHeight"
    ></canvas>

    <!-- Холст для рисования -->
    <canvas
      ref="drawCanvas"
      class="drawing-canvas__drawing"
      :width="canvasWidth"
      :height="canvasHeight"
      @mousedown="startDrawing"
      @mousemove="draw"
      @mouseup="stopDrawing"
      @mouseleave="stopDrawing"
      @touchstart="handleTouch($event, startDrawing)"
      @touchmove="handleTouch($event, draw)"
      @touchend="stopDrawing"
    ></canvas>

    <!-- Индикатор процесса автовоспроизведения -->
    <div v-if="isReplaying" class="drawing-canvas__replay-indicator">
      Воспроизведение...
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    imageUrl: String,
    color: {
      type: String,
      default: "#000000",
    },
    thickness: {
      type: Number,
      default: 3,
    },
    showImage: {
      type: Boolean,
      default: true,
    },
  },
  emits: ["drawing-updated", "replay-complete"],
  watch: {
    // Добавляем наблюдатель за изменением showImage
    showImage: function (newVal, oldVal) {
      // При изменении showImage перерисовываем холст
      this.drawImageOnCanvas();
    },
  },
})
export default class DrawingCanvas extends Vue {
  imageUrl!: string;
  color!: string;
  thickness!: number;
  showImage!: boolean;

  // Состояние рисования
  isDrawing = false;
  lastX = 0;
  lastY = 0;
  canvasWidth = 800;
  canvasHeight = 600;
  originalImage: HTMLImageElement | null = null;
  imageLoaded = false;

  // Состояние воспроизведения
  isReplaying = false;

  // Запись движений для автовоспроизведения
  drawingCommands: Array<{
    x: number;
    y: number;
    dx: number;
    dy: number;
    color: string;
    width: number;
    lift?: boolean;
  }> = [];

  mounted() {
    this.initCanvas();
    window.addEventListener("resize", this.updateCanvasSize);
  }

  beforeUnmount() {
    window.removeEventListener("resize", this.updateCanvasSize);
  }

  get imageContext(): CanvasRenderingContext2D {
    const canvas = this.$refs.imageCanvas as HTMLCanvasElement;
    return canvas.getContext("2d") as CanvasRenderingContext2D;
  }

  get drawContext(): CanvasRenderingContext2D {
    const canvas = this.$refs.drawCanvas as HTMLCanvasElement;
    return canvas.getContext("2d") as CanvasRenderingContext2D;
  }

  initCanvas() {
    // Инициализация холста для рисования
    this.drawContext.lineCap = "round";
    this.drawContext.lineJoin = "round";
    this.drawContext.strokeStyle = this.color;
    this.drawContext.lineWidth = this.thickness;

    // Загрузка изображения
    if (this.imageUrl) {
      this.loadImage();
    } else {
      // Если изображение не предоставлено, устанавливаем белый фон
      this.setWhiteBackground();
    }

    this.updateCanvasSize();
  }

  loadImage() {
    this.originalImage = new Image();
    this.originalImage.onload = () => {
      this.imageLoaded = true;
      // Установим размеры холста на основе изображения
      if (this.originalImage) {
        // Устанавливаем ширину и высоту в зависимости от размера контейнера
        this.updateCanvasSize();
        this.drawImageOnCanvas();
      }
    };
    this.originalImage.src = this.imageUrl;
  }

  updateCanvasSize() {
    if (!this.$refs.canvasContainer) return;

    const container = this.$refs.canvasContainer as HTMLElement;
    const containerWidth = container.clientWidth;

    // Если изображение загружено, сохраняем его пропорции
    if (this.originalImage && this.imageLoaded) {
      const aspectRatio = this.originalImage.width / this.originalImage.height;

      this.canvasWidth = Math.min(containerWidth, 800);
      this.canvasHeight = this.canvasWidth / aspectRatio;

      // Если холст уже создан, перерисовываем содержимое
      this.$nextTick(() => {
        this.drawImageOnCanvas();
        this.redrawLines();
      });
    } else {
      // Если изображение не загружено, устанавливаем стандартные размеры
      this.canvasWidth = Math.min(containerWidth, 800);
      this.canvasHeight = this.canvasWidth * 0.75; // Пропорция 4:3

      this.$nextTick(() => {
        this.setWhiteBackground();
      });
    }
  }

  drawImageOnCanvas() {
    if (!this.originalImage || !this.imageLoaded) return;

    // Очищаем холст
    this.imageContext.clearRect(0, 0, this.canvasWidth, this.canvasHeight);

    if (this.showImage) {
      // Рисуем изображение на холсте
      this.imageContext.drawImage(
        this.originalImage,
        0,
        0,
        this.canvasWidth,
        this.canvasHeight
      );
    } else {
      // Если режим показа только линий, рисуем белый фон
      this.setWhiteBackground();
    }
  }

  setWhiteBackground() {
    this.imageContext.fillStyle = "#ffffff";
    this.imageContext.fillRect(0, 0, this.canvasWidth, this.canvasHeight);
  }

  redrawLines() {
    // Очищаем холст рисования
    this.drawContext.clearRect(0, 0, this.canvasWidth, this.canvasHeight);

    // Перерисовываем все линии из истории рисования
    let lastX = 0;
    let lastY = 0;
    let drawingStarted = false;

    for (const cmd of this.drawingCommands) {
      if (cmd.lift) {
        drawingStarted = false;
        continue;
      }

      this.drawContext.strokeStyle = cmd.color;
      this.drawContext.lineWidth = cmd.width;

      if (!drawingStarted) {
        // Начинаем новую линию
        lastX = cmd.x;
        lastY = cmd.y;
        drawingStarted = true;
        continue;
      }

      // Рисуем отрезок линии
      this.drawContext.beginPath();
      this.drawContext.moveTo(lastX, lastY);
      this.drawContext.lineTo(cmd.x, cmd.y);
      this.drawContext.stroke();

      // Обновляем последние координаты
      lastX = cmd.x;
      lastY = cmd.y;
    }
  }

  startDrawing(event: MouseEvent) {
    this.isDrawing = true;
    const { offsetX, offsetY } = this.getCoordinates(event);
    this.lastX = offsetX;
    this.lastY = offsetY;

    // Добавляем начальную точку линии
    this.drawingCommands.push({
      x: offsetX,
      y: offsetY,
      dx: 0,
      dy: 0,
      color: this.color,
      width: this.thickness,
    });
  }

  draw(event: MouseEvent) {
    if (!this.isDrawing) return;

    const { offsetX, offsetY } = this.getCoordinates(event);

    // Вычисляем смещение
    const dx = offsetX - this.lastX;
    const dy = offsetY - this.lastY;

    // Обновляем контекст рисования с текущими параметрами
    this.drawContext.strokeStyle = this.color;
    this.drawContext.lineWidth = this.thickness;

    // Рисуем линию
    this.drawContext.beginPath();
    this.drawContext.moveTo(this.lastX, this.lastY);
    this.drawContext.lineTo(offsetX, offsetY);
    this.drawContext.stroke();

    // Добавляем команду в историю
    this.drawingCommands.push({
      x: offsetX,
      y: offsetY,
      dx: dx,
      dy: dy,
      color: this.color,
      width: this.thickness,
    });

    // Обновляем последние координаты
    this.lastX = offsetX;
    this.lastY = offsetY;

    // Отправляем событие обновления рисунка
    this.$emit("drawing-updated");
  }

  stopDrawing() {
    if (this.isDrawing) {
      // Добавляем маркер поднятия пера
      this.drawingCommands.push({
        x: 0,
        y: 0,
        dx: 0,
        dy: 0,
        color: this.color,
        width: this.thickness,
        lift: true,
      });
    }

    this.isDrawing = false;
  }

  handleTouch(event: TouchEvent, callback: (event: MouseEvent) => void) {
    event.preventDefault();
    const touch = event.touches[0];
    const canvas = this.$refs.drawCanvas as HTMLElement;
    const rect = canvas.getBoundingClientRect();

    // Создаем синтетическое событие мыши
    const mouseEvent = new MouseEvent("mousemove", {
      clientX: touch.clientX,
      clientY: touch.clientY,
    });

    callback(mouseEvent);
  }

  getCoordinates(event: MouseEvent) {
    const canvas = this.$refs.drawCanvas as HTMLElement;
    const rect = canvas.getBoundingClientRect();
    const scaleX = this.canvasWidth / rect.width;
    const scaleY = this.canvasHeight / rect.height;

    // Точно высчитываем координаты курсора относительно холста
    return {
      offsetX: (event.clientX - rect.left) * scaleX,
      offsetY: (event.clientY - rect.top) * scaleY,
    };
  }

  // Метод для очистки холста рисования
  clearDrawing() {
    this.drawContext.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
    this.drawingCommands = []; // Очищаем историю рисования
    this.$emit("drawing-updated");
  }
  // Добавим в DrawingCanvas.vue новый метод для экспорта в SVG
  getSvgImage(): string {
    // Создаем временный SVG элемент
    const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("width", this.canvasWidth.toString());
    svg.setAttribute("height", this.canvasHeight.toString());
    svg.setAttribute("viewBox", `0 0 ${this.canvasWidth} ${this.canvasHeight}`);

    // Добавляем белый фон
    const background = document.createElementNS(
      "http://www.w3.org/2000/svg",
      "rect"
    );
    background.setAttribute("width", "100%");
    background.setAttribute("height", "100%");
    background.setAttribute("fill", "#ffffff");
    svg.appendChild(background);

    // Если отображаем исходное изображение, добавляем его как фон
    if (this.showImage && this.originalImage && this.imageLoaded) {
      const image = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "image"
      );
      image.setAttribute("width", "100%");
      image.setAttribute("height", "100%");
      image.setAttribute("href", this.imageUrl);
      svg.appendChild(image);
    }

    // Добавляем линии из истории рисования
    let lastX = 0;
    let lastY = 0;
    let drawingStarted = false;
    let currentPath = null;

    for (const cmd of this.drawingCommands) {
      if (cmd.lift) {
        drawingStarted = false;
        continue;
      }

      if (!drawingStarted) {
        // Начинаем новый путь
        currentPath = document.createElementNS(
          "http://www.w3.org/2000/svg",
          "path"
        );
        currentPath.setAttribute("fill", "none");
        currentPath.setAttribute("stroke", cmd.color);
        currentPath.setAttribute("stroke-width", cmd.width.toString());
        currentPath.setAttribute("stroke-linecap", "round");
        currentPath.setAttribute("stroke-linejoin", "round");
        currentPath.setAttribute("d", `M${cmd.x},${cmd.y}`);
        svg.appendChild(currentPath);

        lastX = cmd.x;
        lastY = cmd.y;
        drawingStarted = true;
        continue;
      }

      // Добавляем линию к текущему пути
      if (currentPath) {
        const d = currentPath.getAttribute("d");
        currentPath.setAttribute("d", `${d} L${cmd.x},${cmd.y}`);
      }

      // Обновляем последние координаты
      lastX = cmd.x;
      lastY = cmd.y;
    }

    // Преобразуем SVG в строку
    const serializer = new XMLSerializer();
    return serializer.serializeToString(svg);
  }
  // Метод для получения данных изображения с линиями
  getImageWithLines(): string {
    // Создаем временный холст для объединения изображения и линий
    const tempCanvas = document.createElement("canvas");
    tempCanvas.width = this.canvasWidth;
    tempCanvas.height = this.canvasHeight;
    const tempContext = tempCanvas.getContext("2d") as CanvasRenderingContext2D;

    // Копируем содержимое холста с изображением
    tempContext.drawImage(
      this.$refs.imageCanvas as HTMLCanvasElement,
      0,
      0,
      this.canvasWidth,
      this.canvasHeight
    );

    // Копируем содержимое холста с линиями
    tempContext.drawImage(
      this.$refs.drawCanvas as HTMLCanvasElement,
      0,
      0,
      this.canvasWidth,
      this.canvasHeight
    );

    // Возвращаем данные изображения в формате base64
    return tempCanvas.toDataURL("image/png");
  }

  // Метод для получения только нарисованных линий
  getLinesOnly(): string {
    // Создаем временный холст для сохранения только линий на белом фоне
    const tempCanvas = document.createElement("canvas");
    tempCanvas.width = this.canvasWidth;
    tempCanvas.height = this.canvasHeight;
    const tempContext = tempCanvas.getContext("2d") as CanvasRenderingContext2D;

    // Заполняем белым фоном
    tempContext.fillStyle = "#ffffff";
    tempContext.fillRect(0, 0, this.canvasWidth, this.canvasHeight);

    // Копируем линии с холста рисования
    tempContext.drawImage(this.$refs.drawCanvas as HTMLCanvasElement, 0, 0);

    // Возвращаем данные изображения в формате base64
    return tempCanvas.toDataURL("image/png");
  }

  // Метод для автоматического воспроизведения процесса рисования
  replayDrawing(speed = 1) {
    if (this.isReplaying || this.drawingCommands.length === 0) return;

    // Очищаем холст рисования
    this.drawContext.clearRect(0, 0, this.canvasWidth, this.canvasHeight);

    this.isReplaying = true;

    // Создаем копию команд для воспроизведения
    const commands = [...this.drawingCommands];
    let lastX = 0;
    let lastY = 0;
    let drawingStarted = false;

    // Функция для отрисовки следующей команды
    const drawNextCommand = () => {
      if (commands.length === 0 || !this.isReplaying) {
        this.isReplaying = false;
        this.$emit("replay-complete");
        return;
      }

      const cmd = commands.shift();
      if (!cmd) return;

      // Если это команда поднятия пера - начинаем новую линию
      if (cmd.lift) {
        drawingStarted = false;
        setTimeout(drawNextCommand, 50 / speed);
        return;
      }

      this.drawContext.strokeStyle = cmd.color;
      this.drawContext.lineWidth = cmd.width;

      if (!drawingStarted) {
        // Начинаем новую линию
        lastX = cmd.x;
        lastY = cmd.y;
        drawingStarted = true;
        setTimeout(drawNextCommand, 50 / speed);
        return;
      }

      // Рисуем отрезок линии
      this.drawContext.beginPath();
      this.drawContext.moveTo(lastX, lastY);
      this.drawContext.lineTo(cmd.x, cmd.y);
      this.drawContext.stroke();

      // Обновляем последние координаты
      lastX = cmd.x;
      lastY = cmd.y;

      // Планируем следующую команду
      setTimeout(drawNextCommand, 10 / speed);
    };

    // Начинаем отрисовку
    drawNextCommand();
  }

  // Метод для остановки воспроизведения
  stopReplay() {
    this.isReplaying = false;
  }
}
</script>

<style scoped lang="scss">
.drawing-canvas {
  position: relative;
  height: 100%;
  width: 100%;
  margin: 0 auto;
  border-radius: 8px;
  overflow: hidden;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
  background-color: white;

  &__image,
  &__drawing {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
  }

  &__image {
    object-fit: contain;
    z-index: 1;
  }

  &__drawing {
    z-index: 2;
    cursor: crosshair;
  }

  &__replay-indicator {
    position: absolute;
    top: 10px;
    left: 10px;
    background-color: rgba(0, 0, 0, 0.6);
    color: white;
    padding: 5px 10px;
    border-radius: 4px;
    font-size: 14px;
    z-index: 3;
    animation: pulse 1.5s infinite;
  }
}

@keyframes pulse {
  0% {
    opacity: 0.7;
  }
  50% {
    opacity: 1;
  }
  100% {
    opacity: 0.7;
  }
}
</style>
