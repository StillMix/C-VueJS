<template>
  <transition name="modal">
    <div v-if="image" class="image-modal" @click="$emit('close')">
      <div class="image-modal__content" @click.stop>
        <button class="image-modal__close" @click="$emit('close')">×</button>
        <div class="image-modal__image-container">
          <img :src="imageUrl" :alt="imageCaption" class="image-modal__image" />
        </div>
        <div class="image-modal__controls">
          <button class="image-modal__btn" @click="$emit('edit')">
            <span class="image-modal__btn-icon">✏️</span>
            Рисовать
          </button>
          <button class="image-modal__btn" @click="$emit('download')">
            <span class="image-modal__btn-icon">⬇️</span>
            Скачать
          </button>
          <button
            class="image-modal__btn image-modal__btn--delete"
            @click="$emit('delete')"
          >
            <span class="image-modal__btn-icon">🗑️</span>
            Удалить
          </button>
        </div>
      </div>
    </div>
  </transition>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    image: String,
  },
  emits: ["close", "edit", "download", "delete"],
})
export default class ImageViewModal extends Vue {
  image!: string;

  // Изменение в методе get imageUrl() в компоненте ImageViewModal.vue
  get imageUrl(): string {
    try {
      // Проверяем, начинается ли изображение с "data:" (для загруженных изображений)
      if (this.image && this.image.startsWith("data:")) {
        return this.image;
      }

      // Проверяем, начинается ли имя файла с "userfoto-" (для изображений от пользователя)
      if (this.image && this.image.startsWith("userfoto-")) {
        // В режиме разработки проверяем localStorage
        const savedImage = localStorage.getItem(`userImage_${this.image}`);
        if (savedImage) {
          return savedImage;
        }

        // Если изображение не найдено в localStorage, пытаемся загрузить из папки
        if (window.backend) {
          return `drawings/${this.image}`;
        }
      }

      // Для обычных изображений из Assets
      return require(`@/assets/Drawings/${this.image}`);
    } catch (e) {
      console.error("Ошибка при загрузке изображения:", e);
      return "";
    }
  }

  get imageCaption(): string {
    // Проверяем, начинается ли изображение с "data:" (для загруженных изображений)
    if (this.image && this.image.startsWith("data:")) {
      return "Загруженное изображение";
    }

    // Убираем расширение файла и заменяем подчеркивания на пробелы
    return this.image.replace(/\.[^/.]+$/, "").replace(/_/g, " ");
  }
}
</script>

<style scoped lang="scss">
.image-modal {
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

  &__content {
    background-color: white;
    border-radius: 12px;
    max-width: 90%;
    max-height: 90vh;
    overflow: hidden;
    position: relative;
    display: flex;
    flex-direction: column;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
  }

  &__close {
    position: absolute;
    top: 15px;
    right: 15px;
    width: 36px;
    height: 36px;
    border-radius: 50%;
    background-color: rgba(0, 0, 0, 0.5);
    color: white;
    font-size: 24px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    z-index: 10;
    transition: all 0.2s ease;

    &:hover {
      background-color: rgba(0, 0, 0, 0.8);
      transform: scale(1.1);
    }
  }

  &__image-container {
    flex: 1;
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
    background-color: #000;
    min-height: 400px;
  }

  &__image {
    max-width: 100%;
    max-height: calc(90vh - 80px);
    object-fit: contain;
  }

  &__controls {
    display: flex;
    justify-content: center;
    gap: 15px;
    padding: 15px;
    background-color: #f5f5f5;
    flex-wrap: wrap;
  }

  &__btn {
    padding: 10px 20px;
    border-radius: 30px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    font-size: 16px;
    cursor: pointer;
    transition: all 0.2s ease;
    display: flex;
    align-items: center;
    gap: 5px;

    &:hover {
      background-color: var(--primary-dark, #a74040);
      transform: translateY(-2px);
    }

    &:active {
      transform: translateY(0);
    }

    &--delete {
      background-color: #ff5252;

      &:hover {
        background-color: #d32f2f;
      }
    }
  }

  &__btn-icon {
    font-size: 18px;
  }
}

// Анимации для модального окна
.modal-enter-active,
.modal-leave-active {
  transition: all 0.3s ease;
}

.modal-enter-from,
.modal-leave-to {
  opacity: 0;
}

.modal-enter-from .image-modal__content,
.modal-leave-to .image-modal__content {
  transform: scale(0.9);
  transition: transform 0.3s ease;
}

// Адаптивный дизайн
@media (max-width: 768px) {
  .image-modal {
    &__controls {
      flex-direction: column;
      align-items: center;
    }

    &__btn {
      width: 80%;
    }
  }
}
</style>
