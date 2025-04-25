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
          @finish="finishDrawing"
        />

        <div class="image-editor__canvas-container">
          <DrawingCanvas
            ref="drawingCanvas"
            :imageUrl="getImageUrl()"
            :color="currentColor"
            :thickness="currentThickness"
            :showImage="showImage"
            @drawing-updated="drawingUpdated"
          />
        </div>

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
              Скачать
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

@Options({
  components: {
    Header,
    DrawingTools,
    DrawingCanvas,
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
  hasDrawn = false;

  getImageUrl(): string {
    if (!this.image) return "";

    try {
      return require(`@/assets/Drawings/${this.image}`);
    } catch (e) {
      console.error(e);
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

  finishDrawing() {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    if (canvas) {
      // Получаем изображение только с линиями
      this.linesOnlyImage = canvas.getLinesOnly();
      this.showPreview = true;
    }
  }

  saveDrawing() {
    const canvas = this.$refs.drawingCanvas as InstanceType<
      typeof DrawingCanvas
    >;
    if (canvas) {
      const imageWithLines = canvas.getImageWithLines();

      // В реальном приложении здесь был бы код для сохранения изображения
      // Например, отправка на сервер

      // Временно сохраняем в localStorage для демонстрации
      localStorage.setItem("savedDrawing", imageWithLines);

      // Показываем сообщение пользователю
      alert("Изображение успешно сохранено!");

      // Возвращаемся на предыдущую страницу
      this.$router.back();
    }
  }

  downloadDrawing() {
    if (!this.linesOnlyImage) return;

    // Создаем временную ссылку для скачивания
    const link = document.createElement("a");
    link.download = `drawing_${Date.now()}.png`;
    link.href = this.linesOnlyImage;
    link.click();

    // Закрываем модальное окно
    this.showPreview = false;
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
    height: 800px;
    width: 800px;
    margin-left: auto;
    margin-right: auto;
  }

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
}
</style>
