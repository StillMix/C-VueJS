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
            Редактировать
          </button>
          <button class="image-modal__btn" @click="$emit('download')">
            Скачать
          </button>
          <button
            class="image-modal__btn image-modal__btn--delete"
            @click="$emit('delete')"
          >
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

  get imageUrl(): string {
    try {
      return require(`@/assets/Drawings/${this.image}`);
    } catch (e) {
      console.error(e);
      return "";
    }
  }

  get imageCaption(): string {
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
    }
  }

  &__image-container {
    flex: 1;
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
    background-color: #000;
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
  }

  &__btn {
    padding: 10px 20px;
    border-radius: 30px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    font-size: 16px;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: var(--primary-dark, #a74040);
    }

    &--delete {
      background-color: #ff5252;

      &:hover {
        background-color: #d32f2f;
      }
    }
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
</style>
