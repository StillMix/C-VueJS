<template>
  <div class="image-editor">
    <Header />

    <div class="image-editor__content">
      <h1 class="image-editor__title">Редактор изображений</h1>
      <p class="image-editor__subtitle">Обведите контуры изображения</p>

      <div class="image-editor__container">
        <DrawingTools
          @color-change="updateColor"
          @thickness-change="updateThickness"
          @clear="clearDrawing"
          @finish="saveDrawing"
        />

        <div class="image-editor__canvas-container">
          <DrawingCanvas
            ref="drawingCanvas"
            :imageUrl="getImageUrl()"
            :color="currentColor"
            :thickness="currentThickness"
            :showImage="showImage"
            @drawing-updated="drawingUpdated"
            @replay-complete="onReplayComplete"
          />
        </div>

        <ReplayControls
          :is-replaying="isReplaying"
          @replay="startReplay"
          @stop-replay="stopReplay"
          @speed-change="updateReplaySpeed"
        />

        <div class="image-editor__controls">
          <div class="image-editor__view-toggle">
            <label class="image-editor__toggle-label">
              <input
                type="checkbox"
                v-model="showImage"
                class="image-editor__toggle-input"
              />
              <span class="image-editor__toggle-text">
                {{ showImage ? "Показать фон" : "Только линии" }}
              </span>
            </label>
          </div>

          <div class="image-editor__actions">
            <button
              class="image-editor__btn image-editor__btn--back"
              @click="goBack"
            >
              Назад
            </button>
            <button
              class="image-editor__btn image-editor__btn--save"
              @click="saveDrawing"
            >
              Сохранить
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- Модальное окно предпросмотра -->
    <transition name="modal">
      <div v-if="showPreview" class="image-editor__preview-modal">
        <div class="image-editor__preview-content">
          <button
            class="image-editor__preview-close"
            @click="showPreview = false"
          >
            ×
          </button>
          <h3>Предпросмотр обводки</h3>
          <div class="image-editor__preview-image-container">
            <img
              :src="linesOnlyImage"
              alt="Предпросмотр обводки"
              class="image-editor__preview-image"
            />
          </div>
          <div class="image-editor__preview-actions">
            <button
              class="image-editor__btn image-editor__btn--back"
              @click="showPreview = false"
            >
              Вернуться к редактированию
            </button>
            <button
              class="image-editor__btn image-editor__btn--save"
              @click="downloadDrawing"
            >
              Сохранить
            </button>
          </div>
        </div>
      </div>
    </transition>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Header from "@/components/Header.vue";
import DrawingTools from "@/components/DrawingTools.vue";
import DrawingCanvas from "@/components/DrawingCanvas.vue";
import ReplayControls from "@/components/ReplayControls.vue";

@Options({
  components: {
    Header,
    DrawingTools,
    DrawingCanvas,
    ReplayControls,
  },
  props: {
    image: String,
  },
})
export default class ImageEditorView extends Vue {
  image!: string;
  currentColor = "#000000";
  currentThickness = 3;
  showImage = true;
  showPreview = false;
  linesOnlyImage = "";
  isReplaying = false;
  replaySpeed = 1;
  hasDrawn = false;

  getImageUrl(): string {
    if (!this.image) return "";

    try {
      // Проверяем, начинается ли путь с "userfoto-" (для загруженных пользователем изображений)
      if (this.image.startsWith("userfoto-")) {
        return `./img/${this.image}`;
      }

      // Для стандартных изображений из ресурсов приложения
      return require(`@/assets/Drawings/${this.image}`);
    } catch (e) {
      console.error("Ошибка при загрузке изображения:", e);
      return "";
    }
  }

  updateColor(color: string) {
    this.currentColor = color;
  }

  updateThickness(thickness: number) {
    this.currentThickness = thickness;
  }

  clearDrawing() {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    typeof DrawingCanvas;
    if (canvas) {
      canvas.clearDrawing();
      this.hasDrawn = false;
    }
  }

  drawingUpdated() {
    this.hasDrawn = true;
  }

  goBack() {
    if (this.hasDrawn) {
      if (
        confirm(
          "Вы уверены, что хотите вернуться? Несохраненные изменения будут потеряны."
        )
      ) {
        this.$router.back();
      }
    } else {
      this.$router.back();
    }
  }
  startReplay(speed: number) {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    if (canvas) {
      this.isReplaying = true;
      canvas.replayDrawing(speed);
    }
  }

  stopReplay() {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    if (canvas) {
      canvas.stopReplay();
      this.isReplaying = false;
    }
  }

  updateReplaySpeed(speed: number) {
    this.replaySpeed = speed;
  }

  onReplayComplete() {
    this.isReplaying = false;
  }

  replayInPreview() {
    // В этой версии просто закрываем предпросмотр и запускаем воспроизведение
  }
  // Удалён метод finishDrawing, так как он объединён с saveDrawing

  saveDrawing() {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    typeof DrawingCanvas;
    if (canvas) {
      // Получаем изображение только с линиями и отображаем предпросмотр
      this.linesOnlyImage = canvas.getLinesOnly();
      this.showPreview = true;

      // Если мы решим не показывать предпросмотр, то код ниже можно раскомментировать
      /*
      const imageWithLines = canvas.getImageWithLines();
      
      // В реальном приложении здесь был бы код для сохранения изображения
      // Например, отправка на сервер

      // Временно сохраняем в localStorage для демонстрации
      localStorage.setItem("savedDrawing", imageWithLines);

      // Показываем сообщение пользователю
      alert("Изображение успешно сохранено!");

      // Возвращаемся на предыдущую страницу
      this.$router.back();
      */
    }
  }

  downloadDrawing() {
    if (!this.linesOnlyImage) return;

    // Создаем временную ссылку для скачивания
    const link = document.createElement("a");
    link.download = `drawing_${Date.now()}.png`;
    link.href = this.linesOnlyImage;
    link.click();

    // Закрываем модальное окно после скачивания
    this.showPreview = false;

    // Временно сохраняем в localStorage для демонстрации
    localStorage.setItem("savedDrawing", this.linesOnlyImage);

    // Возвращаемся на предыдущую страницу
    this.$router.back();
  }
}
</script>

<style scoped lang="scss">
.image-editor {
  min-height: 100vh;
  display: flex;
  flex-direction: column;

  &__content {
    width: 90%;
    max-width: 1200px;
    margin: 0 auto;
    padding: 20px 0;
    flex: 1;
  }

  &__title {
    text-align: center;
    margin-top: 20px;
    font-size: 36px;
    font-weight: 600;
    color: var(--text-color, #ffffff);
    text-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
  }

  &__subtitle {
    text-align: center;
    margin: 10px 0 30px;
    font-size: 18px;
    color: var(--text-color, #ffffff);
    opacity: 0.8;
  }

  &__container {
    background-color: rgba(255, 255, 255, 0.1);
    border-radius: 12px;
    padding: 20px;
    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
  }

  &__canvas-container {
    margin: 20px 0;
    height: 600px; // Уменьшил высоту для лучшей производительности
    width: 800px;
    margin-left: auto;
    margin-right: auto;
  }

  /* Остальные стили без изменений */
  &__controls {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-top: 20px;
    flex-wrap: wrap;
    gap: 15px;
  }

  &__view-toggle {
    display: flex;
    align-items: center;
  }

  &__toggle-label {
    display: flex;
    align-items: center;
    cursor: pointer;
  }

  &__toggle-input {
    margin-right: 8px;
    width: 18px;
    height: 18px;
  }

  &__toggle-text {
    font-size: 16px;
    color: var(--text-color, #ffffff);
  }

  &__actions {
    display: flex;
    gap: 10px;
  }

  &__btn {
    padding: 10px 20px;
    border-radius: 30px;
    font-size: 16px;
    cursor: pointer;
    transition: all 0.2s ease;

    &--back {
      background-color: rgba(255, 255, 255, 0.2);
      color: var(--text-color, #ffffff);

      &:hover {
        background-color: rgba(255, 255, 255, 0.3);
      }
    }

    &--save {
      background-color: var(--primary-color, #cf6a6a);
      color: white;

      &:hover {
        background-color: var(--primary-dark, #a74040);
      }
    }
  }

  // Стили для модального окна предпросмотра
  &__preview-modal {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.8);
    z-index: 1000;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 20px;
  }

  &__preview-content {
    background-color: white;
    border-radius: 12px;
    max-width: 80%;
    max-height: 80vh;
    padding: 20px;
    position: relative;
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  &__preview-close {
    position: absolute;
    top: 10px;
    right: 10px;
    width: 30px;
    height: 30px;
    border-radius: 50%;
    background-color: rgba(0, 0, 0, 0.1);
    color: #333;
    font-size: 20px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
  }

  &__preview-image-container {
    margin: 20px 0;
    max-width: 100%;
    overflow: auto;
  }

  &__preview-image {
    max-width: 100%;
    display: block;
  }

  &__preview-actions {
    display: flex;
    gap: 10px;
    margin-top: 20px;
  }
}

// Анимация для модального окна
.modal-enter-active,
.modal-leave-active {
  transition: opacity 0.3s ease;
}

.modal-enter-from,
.modal-leave-to {
  opacity: 0;
}
</style>
