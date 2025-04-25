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
// import initialImages from "../assets/human/images";
declare global {
  interface Window {
    QWebChannel?: any;
    qt?: {
      webChannelTransport?: any;
    };
    backend?: any;
  }
}
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
  backend: any = null;
  imagesCard: string[] = [];
  currentFilter = "all";
  favorites: string[] = [];
  selectedImage: string | null = null;
  selectedIndex = -1;
  showDeleteConfirm = false;
  showUpload = false;
  uploadPreview: string | null = null;
  data() {
    return {
      backend: null as any,
      imagesCard: [] as any[],
      favorites: [] as any[],
    };
  }
  mounted() {
    // Загружаем избранные из localStorage
    const savedFavorites = localStorage.getItem("favorites");
    if (savedFavorites) {
      this.favorites = JSON.parse(savedFavorites);
    }

    // Добавляем обработчик клавиш для навигации в модальном окне
    window.addEventListener("keydown", this.handleKeyPress);

    // Загружаем изображения
    this.initQtConnection();
  }
  initQtConnection() {
    if (!window.QWebChannel || !window.qt || !window.qt.webChannelTransport) {
      console.error("QWebChannel или WebChannelTransport не доступны");
      return;
    }

    new window.QWebChannel(window.qt.webChannelTransport, (channel: any) => {
      this.backend = channel.objects.backend;
      window.backend = this.backend;

      // Подписываемся на сигнал обновления изображений
      this.backend.imagesChanged.connect(() => {
        console.log("Получен сигнал об изменении изображений");
        this.loadImagesFromBackend();
      });

      // Загружаем начальный список изображений
      this.loadImagesFromBackend();
    });
  }
  loadImagesFromBackend() {
    // Проверяем, доступен ли бэкенд
    if (
      typeof window !== "undefined" &&
      typeof window.backend !== "undefined"
    ) {
      console.log("Используем существующий бэкенд");
      window.backend
        .getDrawingImages()
        .then((images: any) => {
          console.log("Получены изображения:", images);
          if (Array.isArray(images)) {
            this.imagesCard = images;
          }
        })
        .catch((error: any) => {
          console.error("Ошибка при получении изображений:", error);
        });
    } else if (
      typeof window !== "undefined" &&
      typeof window.qt !== "undefined" &&
      typeof window.QWebChannel !== "undefined" &&
      typeof window.qt.webChannelTransport !== "undefined"
    ) {
      console.log("Инициализация веб-канала для загрузки изображений");
      // Инициализируем WebChannel, если он еще не создан
      new window.QWebChannel(window.qt.webChannelTransport, (channel: any) => {
        this.backend = channel.objects.backend;
        window.backend = this.backend; // Сохраняем глобально для повторного использования

        this.backend
          .getDrawingImages()
          .then((images: any) => {
            console.log("Получены изображения через новый канал:", images);
            if (Array.isArray(images)) {
              this.imagesCard = images;
            }
          })
          .catch((error: any) => {
            console.error("Ошибка при получении изображений:", error);
          });
      });
    } else {
      console.log("Режим разработки: загрузка тестовых изображений");
      // В режиме разработки используем предопределенный список
      this.imagesCard = [
        "sketch.jpg",
        "sketch2.jpg",
        "sketch3.jpg",
        "sketch4.jpg",
        "sketch5.jpg",
        "sketch6.jpg",
        "sketch7.jpg",
        "sketch8.jpg",
        "sketch9.jpg",
        "sketch10.jpg",
        "sketch11.jpg",
        "sketch12.jpg",
      ];

      // Проверяем наличие пользовательских изображений в localStorage
      for (let i = 0; i < localStorage.length; i++) {
        const key = localStorage.key(i);
        if (key && key.startsWith("userImage_")) {
          const imageName = key.replace("userImage_", "");
          if (!this.imagesCard.includes(imageName)) {
            this.imagesCard.unshift(imageName);
          }
        }
      }
    }
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

  // Изменения в методе uploadImage в vue-client/src/views/DrawView.vue
  uploadImage() {
    if (this.uploadPreview) {
      const timestamp = new Date().getTime();
      const date = new Date();
      const formattedDate = `${String(date.getDate()).padStart(2, "0")}${String(
        date.getMonth() + 1
      ).padStart(2, "0")}${String(date.getFullYear()).slice(-2)}`;
      const formattedTime = `${String(date.getHours()).padStart(
        2,
        "0"
      )}${String(date.getMinutes()).padStart(2, "0")}${String(
        date.getSeconds()
      ).padStart(2, "0")}`;
      const newImageName = `userfoto-${formattedDate}-${formattedTime}.png`;

      // Проверяем доступность C++ бэкенда
      if (window.backend) {
        // Используем C++ бэкенд для сохранения изображения
        window.backend
          .saveImage(this.uploadPreview, newImageName)
          .then((success: any) => {
            if (success) {
              // Добавляем новое изображение в список
              this.imagesCard.unshift(newImageName);

              // Сбрасываем состояние загрузки
              this.uploadPreview = null;
              this.showUpload = false;

              // Обновляем список изображений с сервера для синхронизации
              if (window.backend) {
                window.backend.getDrawingImages().then((images: any) => {
                  if (Array.isArray(images)) {
                    this.imagesCard = images;
                  }
                });
              }
            } else {
              alert("Произошла ошибка при сохранении изображения");
            }
          })
          .catch((error: any) => {
            console.error("Ошибка при сохранении изображения:", error);
            alert("Произошла ошибка при сохранении изображения");
          });
      } else {
        // Для режима разработки (без C++ бэкенда) - имитируем сохранение
        console.log(
          "Режим разработки: имитация сохранения изображения",
          newImageName
        );

        // Добавляем новое изображение в список
        this.imagesCard.unshift(newImageName);

        // Сохраняем в localStorage для демонстрации
        localStorage.setItem(`userImage_${newImageName}`, this.uploadPreview);

        // Сбрасываем состояние загрузки
        this.uploadPreview = null;
        this.showUpload = false;
      }
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
