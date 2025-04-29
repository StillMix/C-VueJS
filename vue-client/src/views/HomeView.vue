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
    // Устанавливаем таймаут для отображения статуса инициализации
    this.errr = "Начало инициализации...";
    console.log("Начало инициализации HomeView");

    // Индикатор загрузки
    this.loading = true;

    // Проверяем, доступен ли бэкенд сразу
    if (window.backend) {
      this.errr = "Бэкенд доступен сразу, загружаем изображения";
      console.log("Бэкенд доступен сразу");
      this.loadAlbumImages();

      // Подписываемся на сигнал обновления альбома
      try {
        window.backend.albumImagesChanged.connect(() => {
          this.errr = "Получен сигнал об изменении изображений альбома";
          this.loadAlbumImages();
        });
        this.errr = "Подписка на сигнал выполнена (прямая инициализация)";
      } catch (error) {
        this.errr = `Ошибка при подписке на сигнал: ${error}`;
        console.error("Ошибка при подписке на сигнал:", error);
      }
    } else {
      this.errr = "Бэкенд недоступен сразу, ожидаем инициализацию";
      console.log("Бэкенд недоступен, ожидаем инициализацию");

      // Устанавливаем таймаут для инициализации, чтобы не зависнуть навсегда
      const initializationTimeout = setTimeout(() => {
        if (this.loading) {
          this.errr =
            "Превышено время ожидания инициализации, переходим в режим разработки";
          console.warn("Превышено время ожидания инициализации");
          this.useDevMode();
        }
      }, 5000); // 5 секунд на инициализацию

      // Проверяем наличие WebChannel
      if (window.QWebChannel && window.qt && window.qt.webChannelTransport) {
        this.errr = "WebChannel обнаружен, инициализируем соединение";
        console.log("WebChannel обнаружен, инициализируем соединение");
        this.initQtConnection();

        // Очищаем таймаут, если инициализация началась
        clearTimeout(initializationTimeout);
      } else {
        this.errr =
          "WebChannel не обнаружен, пробуем повторную инициализацию через 1 секунду";
        console.log("WebChannel не обнаружен, пробуем повторную инициализацию");

        // Пробуем еще раз через 1 секунду
        setTimeout(() => {
          this.errr = "Повторная проверка наличия WebChannel";
          console.log("Повторная проверка наличия WebChannel");

          if (
            window.QWebChannel &&
            window.qt &&
            window.qt.webChannelTransport
          ) {
            this.errr = "WebChannel обнаружен при повторной проверке";
            console.log("WebChannel обнаружен при повторной проверке");
            clearTimeout(initializationTimeout);
            this.initQtConnection();
          } else {
            this.errr =
              "WebChannel не обнаружен при повторной проверке, переходим в режим разработки";
            console.log("WebChannel не обнаружен при повторной проверке");
            clearTimeout(initializationTimeout);
            this.useDevMode();
          }
        }, 1000);

        // Добавляем слушатель события, который выполнится, когда окно загрузится полностью
        window.addEventListener("load", () => {
          this.errr = "Окно полностью загружено, повторная проверка бэкенда";
          console.log("Окно полностью загружено, повторная проверка бэкенда");

          if (window.backend) {
            clearTimeout(initializationTimeout);
            this.loadAlbumImages();
          }
        });
      }
    }
  }

  // Улучшенный метод для инициализации Qt соединения
  initQtConnection() {
    this.errr = "Начинаем инициализацию WebChannel...";

    try {
      if (!window.qt || !window.qt.webChannelTransport) {
        this.errr = "Ошибка: webChannelTransport недоступен!";
        console.error("webChannelTransport недоступен!");
        this.useDevMode();
        return;
      }

      this.errr = "Создаем новый QWebChannel...";
      new window.QWebChannel(
        window.qt.webChannelTransport,
        (channel: any) => {
          this.errr = "WebChannel создан, получаем объект backend...";

          if (!channel || !channel.objects || !channel.objects.backend) {
            this.errr = "Ошибка: объект backend не найден в канале!";
            console.error("Объект backend не найден в канале!");
            this.useDevMode();
            return;
          }

          window.backend = channel.objects.backend;
          this.errr = "Бэкенд успешно инициализирован!";
          console.log("WebChannel инициализирован, бэкенд доступен");

          // Загружаем изображения после инициализации канала
          this.loadAlbumImages();

          // Подписываемся на сигнал с обработкой ошибок
          try {
            if (
              typeof window.backend.albumImagesChanged.connect === "function"
            ) {
              window.backend.albumImagesChanged.connect(() => {
                this.errr = "Получен сигнал об изменении изображений альбома";
                console.log("Получен сигнал об изменении изображений альбома");
                this.loadAlbumImages();
              });
              this.errr =
                "Подписка на сигнал albumImagesChanged выполнена успешно";
            } else {
              this.errr = "Предупреждение: метод connect для сигнала не найден";
              console.warn("Метод connect для сигнала не найден");
            }
          } catch (error) {
            this.errr = `Ошибка при подписке на сигнал: ${error}`;
            console.error("Ошибка при подписке на сигнал:", error);
          }
        },
        (error: any) => {
          // Обработка ошибки при создании WebChannel
          this.errr = `Ошибка при создании WebChannel: ${error}`;
          console.error("Ошибка при создании WebChannel:", error);
          this.useDevMode();
        }
      );
    } catch (error) {
      // Общая обработка ошибок
      this.errr = `Критическая ошибка в initQtConnection: ${error}`;
      console.error("Критическая ошибка в initQtConnection:", error);
      this.useDevMode();
    }
  }

  // Улучшенный метод для режима разработки
  useDevMode() {
    this.errr = "Переход в режим разработки...";
    console.log("Переход в режим разработки");

    try {
      const albumImages = [];
      for (let i = 0; i < localStorage.length; i++) {
        const key = localStorage.key(i);
        if (key && key.startsWith("album_")) {
          albumImages.push(key.replace("album_", ""));
        }
      }

      this.albumImages = albumImages;
      this.loading = false;
      this.errr = `Режим разработки: загружено ${albumImages.length} изображений`;
      console.log(
        `Режим разработки: загружено ${albumImages.length} изображений`
      );

      // Добавляем тестовое изображение, если альбом пуст
      if (albumImages.length === 0) {
        this.errr = "Альбом пуст, добавляем тестовое изображение";
        console.log("Альбом пуст, добавляем тестовое изображение");

        // Создаем простой SVG как тестовое изображение
        const testSvg = `<svg xmlns="http://www.w3.org/2000/svg" width="200" height="200" viewBox="0 0 200 200">
        <rect x="10" y="10" width="180" height="180" fill="#f0f0f0" stroke="#333" stroke-width="2"/>
        <text x="100" y="100" font-family="Arial" font-size="16" text-anchor="middle">Тестовое изображение</text>
        <circle cx="100" cy="70" r="30" fill="#ff9999"/>
        <path d="M50,150 Q100,100 150,150" stroke="#333" stroke-width="3" fill="none"/>
      </svg>`;

        const testFileName = `usersavedraw-010124-120000.svg`;
        localStorage.setItem(`album_${testFileName}`, testSvg);

        // Обновляем список
        this.albumImages = [testFileName];
        this.errr = "Добавлено тестовое изображение в режиме разработки";
        console.log("Добавлено тестовое изображение в режиме разработки");
      }
    } catch (error) {
      this.errr = `Ошибка в режиме разработки: ${error}`;
      console.error("Ошибка в режиме разработки:", error);
      this.albumImages = [];
      this.loading = false;
    }
  }

  // Улучшенный метод загрузки изображений
  // Улучшенный метод загрузки изображений
  loadAlbumImages() {
    this.errr = "Вызван метод loadAlbumImages()";
    console.log("Вызван метод loadAlbumImages()");
    this.loading = true;

    if (window.backend) {
      this.errr = "Используем бэкенд для загрузки изображений";
      console.log("Используем бэкенд для загрузки изображений");

      try {
        // Проверяем, что метод существует
        if (typeof window.backend.getAlbumImages !== "function") {
          this.errr = "Ошибка: метод getAlbumImages не найден в backend";
          console.error("Метод getAlbumImages не найден в backend");
          this.useDevMode();
          return;
        }

        const loadTimeout = setTimeout(() => {
          this.errr = "Превышено время ожидания загрузки изображений";
          console.warn("Превышено время ожидания загрузки изображений");
          this.loading = false;
        }, 10000); // 10 секунд на загрузку

        window.backend
          .getAlbumImages()
          .then((images: any) => {
            clearTimeout(loadTimeout);
            this.errr = `Получены изображения альбома: ${JSON.stringify(
              images
            )}`;
            console.log("Получены изображения альбома:", images);

            if (Array.isArray(images)) {
              this.albumImages = images;
              this.errr = `Загружено ${images.length} изображений`;
            } else {
              this.errr =
                "Предупреждение: полученные данные не являются массивом";
              console.warn("Полученные данные не являются массивом:", images);
              this.albumImages = [];
            }
            this.loading = false;
          })
          .catch((error: any) => {
            clearTimeout(loadTimeout);
            this.errr = `Ошибка при получении изображений: ${error}`;
            console.error("Ошибка при получении изображений альбома:", error);
            this.loading = false;
            this.useDevMode();
          });
      } catch (error) {
        this.errr = `Критическая ошибка при вызове getAlbumImages: ${error}`;
        console.error("Критическая ошибка при вызове getAlbumImages:", error);
        this.loading = false;
        this.useDevMode();
      }
    } else {
      this.errr =
        "Backend недоступен для загрузки изображений, используем режим разработки";
      console.warn("Backend недоступен для загрузки изображений");
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
