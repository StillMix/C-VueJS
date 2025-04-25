// Исправленный компонент DrawingCanvas.vue
<template>
  <div class="drawing-canvas" ref="canvasContainer">
    <canvas
      ref="imageCanvas"
      class="drawing-canvas__image"
      :width="canvasWidth"
      :height="canvasHeight"
    ></canvas>
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
  emits: ["drawing-updated"],
})
export default class DrawingCanvas extends Vue {
  imageUrl!: string;
  color!: string;
  thickness!: number;
  showImage!: boolean;

  isDrawing = false;
  lastX = 0;
  lastY = 0;
  canvasWidth = 800;
  canvasHeight = 600;
  originalImage: HTMLImageElement | null = null;
  imageLoaded = false;
  // Сохраняем историю рисования для анимации
  drawingCommands: Array<{
    x1: number;
    y1: number;
    x2: number;
    y2: number;
    color: string;
    width: number;
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
    for (const cmd of this.drawingCommands) {
      this.drawContext.strokeStyle = cmd.color;
      this.drawContext.lineWidth = cmd.width;
      this.drawContext.beginPath();
      this.drawContext.moveTo(cmd.x1, cmd.y1);
      this.drawContext.lineTo(cmd.x2, cmd.y2);
      this.drawContext.stroke();
    }
  }

  startDrawing(event: MouseEvent) {
    this.isDrawing = true;
    const { offsetX, offsetY } = this.getCoordinates(event);
    this.lastX = offsetX;
    this.lastY = offsetY;
  }

  draw(event: MouseEvent) {
    if (!this.isDrawing) return;

    const { offsetX, offsetY } = this.getCoordinates(event);

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
      x1: this.lastX,
      y1: this.lastY,
      x2: offsetX,
      y2: offsetY,
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

    // Возвращаем данные изображения в формате base64
    return this.animateDrawing(tempCanvas, tempContext);
  }

  // Новый метод для анимации рисования линий
  animateDrawing(
    canvas: HTMLCanvasElement,
    ctx: CanvasRenderingContext2D
  ): string {
    // Для демонстрации сразу нарисуем все линии
    // (в реальном приложении здесь была бы анимация)
    for (const cmd of this.drawingCommands) {
      ctx.strokeStyle = cmd.color;
      ctx.lineWidth = cmd.width;
      ctx.lineCap = "round";
      ctx.lineJoin = "round";
      ctx.beginPath();
      ctx.moveTo(cmd.x1, cmd.y1);
      ctx.lineTo(cmd.x2, cmd.y2);
      ctx.stroke();
    }

    return canvas.toDataURL("image/png");
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

  &__image,
  &__drawing {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
  }

  &__image {
    z-index: 1;
  }

  &__drawing {
    z-index: 2;
    cursor: crosshair;
  }
}
</style>
