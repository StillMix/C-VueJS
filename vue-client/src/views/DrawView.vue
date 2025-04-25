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
      v-if="selectedImage"
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
    // Предотвращаем переход по ссылке
    if (event) {
      event.preventDefault();
    }

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
    if (this.selectedImage) {
      // Удаляем изображение из списка
      this.imagesCard = this.imagesCard.filter(
        (img) => img !== this.selectedImage
      );

      // Удаляем из избранного, если было добавлено
      if (this.favorites.includes(this.selectedImage)) {
        this.favorites = this.favorites.filter(
          (img) => img !== this.selectedImage
        );
        localStorage.setItem("favorites", JSON.stringify(this.favorites));
      }

      // Закрываем модальные окна
      this.showDeleteConfirm = false;
      this.closeImageDetail();
    }
  }

  editImage() {
    if (this.selectedImage) {
      // Переход на маршрут по его имени и параметрам
      this.$router
        .push({
          name: "edit", // Исправлено на "edit" вместо "edit-image"
          params: { image: this.selectedImage }, // Исправлено на "image" вместо "imageName"
        })
        .catch((err) => {
          console.error("Ошибка перехода:", err);
          alert("Функция рисования будет доступна в ближайшем обновлении");
        });

      // Закрываем модальное окно
      this.closeImageDetail();
    }
  }

  downloadImage() {
    // Скачивание изображения
    if (this.selectedImage) {
      try {
        const link = document.createElement("a");
        link.href = this.getImageUrl(this.selectedImage);
        link.download = this.selectedImage;
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
      } catch (error) {
        console.error("Ошибка при скачивании изображения:", error);
        alert("Не удалось скачать изображение");
      }
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
    // В реальном приложении здесь был бы код для загрузки изображения на сервер
    // Для демонстрации просто добавляем изображение в список
    if (this.uploadPreview) {
      const timestamp = new Date().getTime();
      const newImageName = `uploaded_image_${timestamp}.jpg`;

      // Добавляем новое изображение в список
      this.imagesCard.unshift(newImageName);

      // Сбрасываем состояние загрузки
      this.uploadPreview = null;
      this.showUpload = false;

      alert("Изображение успешно загружено!");
    }
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
