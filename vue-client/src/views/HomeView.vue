<template>
  <div class="home">
    <Header />
    <div class="home__content">
      <h1 class="home__title">Альбом</h1>
      <p class="home__subtitle">Сохраненные рисунки</p>

      <div v-if="loading" class="home__loading">{{ errr }}</div>

      <div v-else-if="albumImages.length === 0" class="home__empty">
        <p>В альбоме пока нет изображений</p>
        <router-link to="/draw" class="home__create-btn">
          Перейти к рисованию
        </router-link>
      </div>

      <div v-else class="home__gallery">
        <AlbumImageCard
          v-for="image in albumImages"
          :key="image"
          :image="image"
          @click="openAlbumImage"
          @delete="deleteAlbumImage"
        />
      </div>
    </div>

    <!-- Модальное окно просмотра SVG из альбома -->
    <transition name="modal">
      <div v-if="selectedImage" class="home__modal" @click="closeAlbumImage">
        <div class="home__modal-content" @click.stop>
          <button class="home__modal-close" @click="closeAlbumImage">×</button>
          <div class="home__modal-image-container" v-html="svgContent"></div>
          <div class="home__modal-controls">
            <button
              class="home__modal-btn home__modal-btn--delete"
              @click="deleteAlbumImage(selectedImage)"
            >
              <span class="home__modal-btn-icon">🗑️</span>
              Удалить
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
import AlbumImageCard from "@/components/AlbumImageCard.vue";

@Options({
  components: {
    Header,
    AlbumImageCard,
  },
})
export default class HomeView extends Vue {
  albumImages: string[] = [];
  loading = true;
  selectedImage: string | null = null;
  svgContent = "";
  errr = "";

  // Изменения в HomeView.vue
  // Метод mounted для более надежной инициализации

  mounted() {
    // Индикатор загрузки
    this.loading = true;

    // Проверяем, доступен ли бэкенд сразу
    if (window.backend) {
      this.errr = "Бэкенд доступен сразу, загружаем изображения";
      this.loadAlbumImages();

      // Подписываемся на сигнал обновления альбома
      window.backend.albumImagesChanged.connect(() => {
        this.errr = "Получен сигнал об изменении изображений альбома";
        this.loadAlbumImages();
      });
    } else {
      console.log("Бэкенд недоступен, ожидаем инициализацию");

      // Проверяем наличие WebChannel
      if (window.QWebChannel && window.qt && window.qt.webChannelTransport) {
        this.errr = "WebChannel обнаружен, инициализируем соединение";
        this.initQtConnection();
      } else {
        this.errr = "WebChannel не обнаружен, используем режим разработки";
        // Для режима разработки загружаем из localStorage
        this.useDevMode();

        // Добавляем слушатель события, который выполнится, когда окно загрузится полностью
        window.addEventListener("load", () => {
          this.errr = "Окно полностью загружено, повторная проверка бэкенда";
          if (window.backend) {
            this.loadAlbumImages();
          }
        });
      }
    }
  }

  // Новый метод для инициализации Qt соединения
  initQtConnection() {
    new window.QWebChannel(
      window.qt && window.qt.webChannelTransport,
      (channel: any) => {
        window.backend = channel.objects.backend;
        console.log("WebChannel инициализирован, бэкенд доступен");

        // Загружаем изображения после инициализации канала
        this.loadAlbumImages();

        // Подписываемся на сигнал
        window.backend.albumImagesChanged.connect(() => {
          console.log("Получен сигнал об изменении изображений альбома");
          this.loadAlbumImages();
        });
      }
    );
  }

  // Новый метод для режима разработки
  useDevMode() {
    setTimeout(() => {
      const albumImages = [];
      for (let i = 0; i < localStorage.length; i++) {
        const key = localStorage.key(i);
        if (key && key.startsWith("album_")) {
          albumImages.push(key.replace("album_", ""));
        }
      }
      this.albumImages = albumImages;
      this.loading = false;
      this.errr = ` "Режим разработки: загружено",
        ${albumImages.length},
        "изображений"`;
    }, 500);
  }

  // Улучшенный метод загрузки изображений
  loadAlbumImages() {
    this.errr = "Вызван метод loadAlbumImages()";
    this.loading = true;

    if (window.backend) {
      this.errr = "Используем бэкенд для загрузки изображений";
      window.backend
        .getAlbumImages()
        .then((images: string) => {
          this.errr = `Получены изображения альбома: ${images}`;
          if (Array.isArray(images)) {
            this.albumImages = images;
          }
          this.loading = false;
        })
        .catch((error: string) => {
          console.error("Ошибка при получении изображений альбома:", error);
          this.loading = false;
          // Показываем пользователю сообщение об ошибке
          alert(
            "Не удалось загрузить изображения. Пожалуйста, попробуйте обновить страницу."
          );
        });
    } else {
      console.error("Backend недоступен для загрузки изображений");
      this.useDevMode();
    }
  }

  openAlbumImage(imageName: string) {
    this.selectedImage = imageName;
    this.loadSvgContent(imageName);
    document.body.classList.add("modal-open");
  }

  closeAlbumImage() {
    this.selectedImage = null;
    this.svgContent = "";
    document.body.classList.remove("modal-open");
  }

  // В методе loadSvgContent() в файле vue-client/src/views/HomeView.vue:

  async loadSvgContent(imageName: string) {
    try {
      if (window.backend) {
        // В режиме с Qt-бэкендом загружаем SVG через URL
        this.svgContent = `<img src="drawings://album/${imageName}" alt="SVG изображение" class="home__modal-svg" />`;
      } else {
        // В режиме разработки используем localStorage
        const svgData = localStorage.getItem(`album_${imageName}`);
        if (svgData) {
          this.svgContent = svgData;
        }
      }
    } catch (error) {
      console.error("Ошибка при загрузке SVG:", error);
    }
  }

  deleteAlbumImage(imageName: string) {
    if (confirm("Вы уверены, что хотите удалить это изображение из альбома?")) {
      if (window.backend) {
        window.backend
          .deleteImage(imageName)
          .then((success: boolean) => {
            if (success) {
              // Обновляем список изображений
              this.loadAlbumImages();
              // Если было открыто модальное окно, закрываем его
              if (this.selectedImage === imageName) {
                this.closeAlbumImage();
              }
              console.log("Изображение успешно удалено");
            } else {
              alert("Не удалось удалить изображение");
            }
          })
          .catch((error: any) => {
            console.error("Ошибка при удалении изображения:", error);
            alert("Произошла ошибка при удалении изображения");
          });
      } else {
        // Режим разработки - удаляем из localStorage
        localStorage.removeItem(`album_${imageName}`);
        this.loadAlbumImages();
        // Если было открыто модальное окно, закрываем его
        if (this.selectedImage === imageName) {
          this.closeAlbumImage();
        }
        console.log("Изображение удалено (режим разработки)");
      }
    }
  }
}
</script>

<style scoped lang="scss">
.home {
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

  &__loading {
    text-align: center;
    padding: 50px 0;
    color: var(--text-color, #ffffff);
    font-size: 18px;
  }

  &__empty {
    text-align: center;
    padding: 50px 0;
    color: var(--text-color, #ffffff);

    p {
      margin-bottom: 20px;
      font-size: 18px;
      opacity: 0.8;
    }
  }

  &__create-btn {
    display: inline-block;
    padding: 12px 24px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    text-decoration: none;
    border-radius: 30px;
    font-size: 16px;
    font-weight: 500;
    transition: all 0.2s ease;

    &:hover {
      background-color: var(--primary-dark, #a74040);
      transform: translateY(-2px);
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
    }
  }

  &__gallery {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 25px;
    margin-top: 20px;
  }

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
      transform: scale(1.1);
    }
  }

  &__modal-image-container {
    flex: 1;
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
    background-color: #f5f5f5;
    min-height: 400px;
    padding: 20px;

    :deep(svg) {
      max-width: 100%;
      max-height: 70vh;
    }

    :deep(img) {
      max-width: 100%;
      max-height: 70vh;
    }
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
    font-size: 16px;
    cursor: pointer;
    transition: all 0.2s ease;
    display: flex;
    align-items: center;
    gap: 5px;

    &--delete {
      background-color: #ff5252;
      color: white;

      &:hover {
        background-color: #d32f2f;
        transform: translateY(-2px);
      }

      &:active {
        transform: translateY(0);
      }
    }
  }

  &__modal-btn-icon {
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

.modal-enter-from .home__modal-content,
.modal-leave-to .home__modal-content {
  transform: scale(0.9);
  transition: transform 0.3s ease;
}

// Адаптивный дизайн
@media (max-width: 768px) {
  .home {
    &__gallery {
      grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
    }
  }
}
</style>
