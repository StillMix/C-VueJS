<template>
  <div class="draw">
    <Header />

    <div class="draw__content">
      <h1 class="draw__title">Рисовальная студия</h1>
      <p class="draw__subtitle">Выберите изображение или добавьте новое</p>

      <div class="draw__tools">
        <button class="draw__tool-btn" @click="showUploadDialog">
          <AddImg :width="28" :height="28" />
          <span>Добавить изображение</span>
        </button>

        <div class="draw__filters">
          <button
            class="draw__filter-btn"
            :class="{ 'draw__filter-btn--active': currentFilter === 'all' }"
            @click="setFilter('all')"
          >
            Все
          </button>
          <button
            class="draw__filter-btn"
            :class="{ 'draw__filter-btn--active': currentFilter === 'recent' }"
            @click="setFilter('recent')"
          >
            Недавние
          </button>
          <button
            class="draw__filter-btn"
            :class="{
              'draw__filter-btn--active': currentFilter === 'favorite',
            }"
            @click="setFilter('favorite')"
          >
            Избранные
          </button>
        </div>
      </div>

      <transition-group name="gallery" tag="div" class="draw__gallery">
        <div
          v-for="(item, index) in filteredImages"
          :key="item"
          class="draw__card"
          :style="{ animationDelay: `${index * 0.05}s` }"
        >
          <div class="draw__card-inner">
            <img
              :src="getImageUrl(item)"
              :alt="`Изображение ${index + 1}`"
              class="draw__image"
              loading="lazy"
              @click="openImageDetail(item, index)"
            />

            <div class="draw__card-overlay">
              <button class="draw__card-btn draw__card-btn--edit">
                <DrawingImg :width="20" :height="20" /> Редактировать
              </button>
              <button
                class="draw__card-btn draw__card-btn--favorite"
                @click="toggleFavorite(item)"
              >
                <span v-if="favorites.includes(item)">★</span>
                <span v-else>☆</span>
              </button>
            </div>
          </div>
          <div class="draw__card-caption">
            {{ formatImageName(item) }}
          </div>
        </div>
      </transition-group>

      <div v-if="filteredImages.length === 0" class="draw__empty">
        <p>Изображения не найдены</p>
      </div>
    </div>

    <!-- Модальное окно для просмотра изображения -->
    <transition name="modal">
      <div v-if="selectedImage" class="draw__modal" @click="closeImageDetail">
        <div class="draw__modal-content" @click.stop>
          <button class="draw__modal-close" @click="closeImageDetail">×</button>
          <div class="draw__modal-image-container">
            <img
              :src="getImageUrl(selectedImage)"
              :alt="formatImageName(selectedImage)"
              class="draw__modal-image"
            />
          </div>
          <div class="draw__modal-controls">
            <button class="draw__modal-btn" @click="editImage">
              Редактировать
            </button>
            <button class="draw__modal-btn" @click="downloadImage">
              Скачать
            </button>
            <button
              class="draw__modal-btn draw__modal-btn--delete"
              @click="confirmDelete"
            >
              Удалить
            </button>
          </div>
        </div>
      </div>
    </transition>

    <!-- Диалог подтверждения удаления -->
    <transition name="modal">
      <div v-if="showDeleteConfirm" class="draw__modal draw__modal--confirm">
        <div class="draw__modal-content draw__modal-content--small">
          <h3>Удалить изображение?</h3>
          <p>Это действие нельзя будет отменить.</p>
          <div class="draw__modal-buttons">
            <button class="draw__modal-btn" @click="showDeleteConfirm = false">
              Отмена
            </button>
            <button
              class="draw__modal-btn draw__modal-btn--delete"
              @click="deleteImage"
            >
              Удалить
            </button>
          </div>
        </div>
      </div>
    </transition>

    <!-- Диалог загрузки файла -->
    <transition name="modal">
      <div v-if="showUpload" class="draw__modal">
        <div class="draw__modal-content draw__modal-content--small">
          <button class="draw__modal-close" @click="showUpload = false">
            ×
          </button>
          <h3>Добавить новое изображение</h3>
          <div
            class="draw__upload-area"
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
            <div class="draw__upload-icon">+</div>
            <p>Нажмите для выбора или перетащите файл сюда</p>
          </div>
          <div v-if="uploadPreview" class="draw__upload-preview">
            <img :src="uploadPreview" alt="Предпросмотр" />
            <button class="draw__modal-btn" @click="uploadImage">
              Загрузить
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
import initialCard from "../assets/human/images";
import AddImg from "@/assets/icons/addImg.vue";
import DrawingImg from "@/assets/icons/drawingImg.vue";

@Options({
  components: {
    Header,
    AddImg,
    DrawingImg,
  },
})
export default class DrawView extends Vue {
  imagesCard = initialCard;
  currentFilter = "all";
  favorites: string[] = [];
  selectedImage: string | null = null;
  selectedIndex = -1;
  showDeleteConfirm = false;
  showUpload = false;
  uploadPreview: string | null = null;

  mounted() {
    // Загружаем избранные из localStorage
    const savedFavorites = localStorage.getItem("favorites");
    if (savedFavorites) {
      this.favorites = JSON.parse(savedFavorites);
    }

    // Анимация карточек при первой загрузке
    document.querySelectorAll(".draw__card").forEach((card, index) => {
      setTimeout(() => {
        card.classList.add("animated");
      }, index * 100);
    });

    // Добавляем обработчик клавиш для навигации в модальном окне
    window.addEventListener("keydown", this.handleKeyPress);
  }

  beforeUnmount() {
    window.removeEventListener("keydown", this.handleKeyPress);
  }

  get filteredImages() {
    switch (this.currentFilter) {
      case "recent":
        // В реальном приложении здесь может быть логика сортировки по дате
        // Сейчас просто возвращаем последние 5 изображений
        return [...this.imagesCard].slice(-5);
      case "favorite":
        return this.imagesCard.filter((img) => this.favorites.includes(img));
      default:
        return this.imagesCard;
    }
  }

  getImageUrl(name: string): string {
    try {
      return require(`@/assets/Drawings/${name}`);
    } catch (e) {
      console.error(e);
      return "";
    }
  }

  formatImageName(name: string): string {
    // Убираем расширение файла и заменяем подчеркивания на пробелы
    return name.replace(/\.[^/.]+$/, "").replace(/_/g, " ");
  }

  setFilter(filter: string) {
    this.currentFilter = filter;
  }

  toggleFavorite(image: string) {
    if (this.favorites.includes(image)) {
      this.favorites = this.favorites.filter((img) => img !== image);
    } else {
      this.favorites.push(image);
    }
    localStorage.setItem("favorites", JSON.stringify(this.favorites));
  }

  openImageDetail(image: string, index: number) {
    this.selectedImage = image;
    this.selectedIndex = index;
    document.body.classList.add("modal-open");
  }

  closeImageDetail() {
    this.selectedImage = null;
    document.body.classList.remove("modal-open");
  }

  confirmDelete() {
    this.showDeleteConfirm = true;
  }

  deleteImage() {
    // В реальном приложении здесь был бы код для удаления изображения
    // На данном этапе просто имитируем операцию
    alert("В реальном приложении здесь произошло бы удаление изображения");
    this.showDeleteConfirm = false;
    this.closeImageDetail();
  }

  editImage() {
    // Переход к редактированию изображения
    alert("Переход к редактированию изображения");
    this.closeImageDetail();
  }

  downloadImage() {
    // Имитация скачивания изображения
    if (this.selectedImage) {
      const link = document.createElement("a");
      link.href = this.getImageUrl(this.selectedImage);
      link.download = this.selectedImage;
      link.click();
    }
  }

  handleKeyPress(e: KeyboardEvent) {
    if (!this.selectedImage) return;

    if (e.key === "Escape") {
      this.closeImageDetail();
    } else if (e.key === "ArrowRight") {
      this.navigateImage(1);
    } else if (e.key === "ArrowLeft") {
      this.navigateImage(-1);
    }
  }

  navigateImage(direction: number) {
    if (this.filteredImages.length <= 1) return;

    let newIndex = this.selectedIndex + direction;

    if (newIndex < 0) {
      newIndex = this.filteredImages.length - 1;
    } else if (newIndex >= this.filteredImages.length) {
      newIndex = 0;
    }

    this.selectedImage = this.filteredImages[newIndex];
    this.selectedIndex = newIndex;
  }

  showUploadDialog() {
    this.showUpload = true;
    this.uploadPreview = null;
  }

  triggerFileInput() {
    const fileInput = this.$refs.fileInput as HTMLInputElement;
    fileInput.click();
  }

  handleFileSelect(event: Event) {
    const files = (event.target as HTMLInputElement).files;
    if (files && files.length > 0) {
      this.previewFile(files[0]);
    }
  }

  handleFileDrop(event: DragEvent) {
    if (event.dataTransfer && event.dataTransfer.files.length > 0) {
      this.previewFile(event.dataTransfer.files[0]);
    }
  }

  previewFile(file: File) {
    const reader = new FileReader();
    reader.onload = (e) => {
      this.uploadPreview = e.target?.result as string;
    };
    reader.readAsDataURL(file);
  }

  uploadImage() {
    // В реальном приложении здесь был бы код для загрузки изображения
    // На данном этапе просто имитируем операцию
    alert("В реальном приложении здесь произошла бы загрузка изображения");
    this.showUpload = false;
  }
}
</script>

<style scoped lang="scss">
.draw {
  &__content {
    width: 90%;
    max-width: 1200px;
    margin: 0 auto;
    padding: 20px 0;
  }

  &__title {
    text-align: center;
    margin-top: 20px;
    font-size: 36px;
    font-weight: 600;
    color: var(--text-color, #ffffff);
    text-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
    transition: transform 0.3s ease;

    &:hover {
      transform: scale(1.02);
    }
  }

  &__subtitle {
    text-align: center;
    margin: 10px 0 30px;
    font-size: 18px;
    color: var(--text-color, #ffffff);
    opacity: 0.8;
  }

  &__tools {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 30px;
    flex-wrap: wrap;
    gap: 15px;
  }

  &__tool-btn {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 10px 16px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    border-radius: 30px;
    font-size: 16px;
    font-weight: 500;
    cursor: pointer;
    transition: all 0.2s ease;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);

    &:hover {
      background-color: var(--primary-dark, #a74040);
      transform: translateY(-2px);
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
    }
  }

  &__filters {
    display: flex;
    gap: 10px;
  }

  &__filter-btn {
    padding: 8px 14px;
    border-radius: 20px;
    background-color: rgba(255, 255, 255, 0.1);
    color: var(--text-color, #ffffff);
    font-size: 14px;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: rgba(255, 255, 255, 0.2);
    }

    &--active {
      background-color: var(--accent-color, #f8d0d0);
      color: var(--primary-dark, #a74040);
      font-weight: 500;
    }
  }

  &__gallery {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 25px;
    margin-top: 20px;
  }

  &__card {
    opacity: 0;
    transform: translateY(20px);
    animation: cardAppear 0.5s forwards;
    position: relative;
    transition: all 0.3s ease;

    &-inner {
      position: relative;
      border-radius: 12px;
      overflow: hidden;
      box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
      aspect-ratio: 4/3;

      &:hover {
        transform: translateY(-5px);
        box-shadow: 0 8px 20px rgba(0, 0, 0, 0.15);

        .draw__card-overlay {
          opacity: 1;
        }
      }
    }

    &-caption {
      margin-top: 10px;
      font-size: 16px;
      color: var(--text-color, #ffffff);
      text-align: center;
    }

    &-overlay {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: linear-gradient(to top, rgba(0, 0, 0, 0.7), transparent);
      display: flex;
      align-items: flex-end;
      justify-content: space-between;
      padding: 15px;
      opacity: 0;
      transition: opacity 0.3s ease;
    }

    &-btn {
      padding: 8px 12px;
      border-radius: 20px;
      font-size: 14px;
      display: flex;
      align-items: center;
      gap: 5px;
      cursor: pointer;
      transition: all 0.2s ease;

      &--edit {
        background-color: white;
        color: #333;

        &:hover {
          background-color: var(--accent-color, #f8d0d0);
        }
      }

      &--favorite {
        width: 36px;
        height: 36px;
        display: flex;
        align-items: center;
        justify-content: center;
        border-radius: 50%;
        background-color: rgba(255, 255, 255, 0.3);
        color: yellow;
        font-size: 18px;
        padding: 0;

        &:hover {
          background-color: white;
          transform: scale(1.1);
        }
      }
    }
  }

  &__image {
    width: 100%;
    height: 100%;
    object-fit: cover;
    transition: transform 0.5s ease;
    cursor: pointer;

    &:hover {
      transform: scale(1.05);
    }
  }

  &__empty {
    text-align: center;
    padding: 50px 0;
    color: var(--text-color, #ffffff);
    opacity: 0.7;
  }

  // Модальное окно
  &__modal {
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

    &--confirm {
      background-color: rgba(0, 0, 0, 0.5);
    }
  }

  &__modal-content {
    background-color: white;
    border-radius: 12px;
    max-width: 90%;
    max-height: 90vh;
    overflow: hidden;
    position: relative;
    display: flex;
    flex-direction: column;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);

    &--small {
      width: 400px;
      padding: 20px;
      text-align: center;
    }
  }

  &__modal-close {
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

  &__modal-image-container {
    flex: 1;
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
    background-color: #000;
  }

  &__modal-image {
    max-width: 100%;
    max-height: calc(90vh - 80px);
    object-fit: contain;
  }

  &__modal-controls {
    display: flex;
    justify-content: center;
    gap: 15px;
    padding: 15px;
    background-color: #f5f5f5;
  }

  &__modal-btn {
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

  &__modal-buttons {
    display: flex;
    justify-content: center;
    gap: 15px;
    margin-top: 20px;
  }

  // Форма загрузки
  &__upload-area {
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

  &__upload-icon {
    font-size: 36px;
    color: #ccc;
    margin-bottom: 10px;
  }

  &__upload-preview {
    margin-top: 20px;
    text-align: center;

    img {
      max-width: 100%;
      max-height: 200px;
      border-radius: 8px;
      margin-bottom: 15px;
    }
  }
}

// Анимации
@keyframes cardAppear {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

// Анимации для transition-group
.gallery-enter-active,
.gallery-leave-active {
  transition: all 0.5s ease;
}

.gallery-enter-from,
.gallery-leave-to {
  opacity: 0;
  transform: translateY(30px);
}

// Анимации для модальных окон
.modal-enter-active,
.modal-leave-active {
  transition: all 0.3s ease;
}

.modal-enter-from,
.modal-leave-to {
  opacity: 0;
}

.modal-enter-from .draw__modal-content,
.modal-leave-to .draw__modal-content {
  transform: scale(0.9);
  transition: transform 0.3s ease;
}

// Стили для мобильных устройств
@media (max-width: 768px) {
  .draw {
    &__tools {
      flex-direction: column;
      align-items: flex-start;
    }

    &__filters {
      width: 100%;
      justify-content: center;
    }

    &__gallery {
      grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
    }
  }
}

// Глобальные стили для блокировки прокрутки при открытом модальном окне
:global(body.modal-open) {
  overflow: hidden;
}
</style>
