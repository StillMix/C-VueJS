<template>
  <transition name="modal">
    <div v-if="show" class="upload-modal">
      <div class="upload-modal__content">
        <button class="upload-modal__close" @click="$emit('close')">×</button>
        <h3>Добавить новое изображение</h3>
        <div
          class="upload-modal__drop-area"
          @click="triggerFileInput"
          @dragover.prevent
          @drop.prevent="handleFileDrop"
        >
          <input
            type="file"
            ref="fileInput"
            style="display: none"
            accept="image/*"
            @change="handleFileSelect"
          />
          <div class="upload-modal__icon">+</div>
          <p>Нажмите для выбора или перетащите файл сюда</p>
        </div>
        <div v-if="previewUrl" class="upload-modal__preview">
          <img :src="previewUrl" alt="Предпросмотр" />
          <button class="upload-modal__btn" @click="$emit('upload')">
            Загрузить
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
    show: Boolean,
    previewUrl: String,
  },
  emits: ["close", "file-selected", "upload"],
})
export default class UploadModal extends Vue {
  show!: boolean;
  previewUrl!: string;

  triggerFileInput(): void {
    const fileInput = this.$refs.fileInput as HTMLInputElement;
    fileInput.click();
  }

  handleFileSelect(event: Event): void {
    const files = (event.target as HTMLInputElement).files;
    if (files && files.length > 0) {
      this.handleFile(files[0]);
    }
  }

  handleFileDrop(event: DragEvent): void {
    if (event.dataTransfer && event.dataTransfer.files.length > 0) {
      this.handleFile(event.dataTransfer.files[0]);
    }
  }

  handleFile(file: File): void {
    this.$emit("file-selected", file);
  }
}
</script>

<style scoped lang="scss">
.upload-modal {
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
    width: 500px;
    max-width: 90%;
    padding: 30px;
    background-color: white;
    border-radius: 12px;
    position: relative;
    text-align: center;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);

    h3 {
      margin-bottom: 20px;
      color: #333;
    }
  }

  &__close {
    position: absolute;
    top: 15px;
    right: 15px;
    width: 36px;
    height: 36px;
    border-radius: 50%;
    background-color: rgba(0, 0, 0, 0.1);
    color: #333;
    font-size: 24px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: rgba(0, 0, 0, 0.2);
    }
  }

  &__drop-area {
    margin: 20px 0;
    border: 2px dashed #ccc;
    border-radius: 10px;
    padding: 30px;
    text-align: center;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      border-color: var(--primary-color, #cf6a6a);
      background-color: rgba(0, 0, 0, 0.02);
    }
  }

  &__icon {
    font-size: 36px;
    color: #ccc;
    margin-bottom: 10px;
  }

  &__preview {
    margin-top: 20px;
    text-align: center;

    img {
      max-width: 100%;
      max-height: 200px;
      border-radius: 8px;
      margin-bottom: 15px;
    }
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

.modal-enter-from .upload-modal__content,
.modal-leave-to .upload-modal__content {
  transform: scale(0.9);
  transition: transform 0.3s ease;
}
</style>
