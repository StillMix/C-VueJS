<template>
  <div class="draw">
    <Header />

    <div class="draw__content">
      <h1 class="draw__title">Рисовальная студия</h1>
      <p class="draw__subtitle">Выберите изображение или добавьте новое</p>

      <!-- Использование компонента DrawToolbar -->
      <DrawToolbar
        :current-filter="currentFilter"
        @upload="showUpload = true"
        @filter-change="setFilter"
      />

      <!-- Использование компонента ImageGallery -->
      <ImageGallery
        :images="filteredImages"
        :favorites="favorites"
        @image-click="openImageDetail"
        @toggle-favorite="toggleFavorite"
      />
    </div>

    <!-- Использование компонента ImageViewModal -->
    <ImageViewModal
      :image="selectedImage"
      @close="closeImageDetail"
      @edit="editImage"
      @download="downloadImage"
      @delete="confirmDelete"
    />

    <!-- Использование компонента DeleteConfirmModal -->
    <DeleteConfirmModal
      :show="showDeleteConfirm"
      @cancel="showDeleteConfirm = false"
      @confirm="deleteImage"
    />

    <!-- Использование компонента UploadModal -->
    <UploadModal
      :show="showUpload"
      :preview-url="uploadPreview"
      @close="showUpload = false"
      @file-selected="previewFile"
      @upload="uploadImage"
    />
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Header from "@/components/Header.vue";
import DrawToolbar from "@/components/DrawToolbar.vue";
import ImageGallery from "@/components/ImageGallery.vue";
import ImageViewModal from "@/components/ImageViewModal.vue";
import DeleteConfirmModal from "@/components/DeleteConfirmModal.vue";
import UploadModal from "@/components/UploadModal.vue";
import initialImages from "../assets/human/images";

@Options({
  components: {
    Header,
    DrawToolbar,
    ImageGallery,
    ImageViewModal,
    DeleteConfirmModal,
    UploadModal,
  },
})
export default class DrawView extends Vue {
  // Данные
  imagesCard = initialImages;
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

  // Методы
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

  getImageUrl(name: string): string {
    try {
      return require(`@/assets/Drawings/${name}`);
    } catch (e) {
      console.error(e);
      return "";
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

  previewFile(file: File) {
    const reader = new FileReader();
    reader.onload = (e) => {
      this.uploadPreview = e.target?.result as string;
    };
    reader.readAsDataURL(file);
  }

  uploadImage() {
    // В реальном приложении здесь был бы код для загрузки изображения
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
}

// Глобальные стили для блокировки прокрутки при открытом модальном окне
:global(body.modal-open) {
  overflow: hidden;
}
</style>
