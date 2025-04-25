<template>
  <div id="app">
    <BackgroundAnimation :theme="currentTheme" />
    <div v-if="showDevNotification" class="dev-notification">
      Режим разработки (без Qt)
      <button class="dev-notification__close" @click="closeDevNotification">
        ×
      </button>
    </div>
    <router-view v-slot="{ Component }">
      <transition name="page" mode="out-in">
        <component :is="Component" />
      </transition>
    </router-view>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import BackgroundAnimation from "@/components/BackgroundAnimation.vue";

// Определяем интерфейс для нашего пользовательского события
interface ThemeChangeEvent extends Event {
  detail?: {
    theme: string;
  };
}

// Расширяем глобальный интерфейс Window для TypeScript
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
    BackgroundAnimation,
  },
})
export default class App extends Vue {
  // Устанавливаем начальную тему
  currentTheme = "FlowOfRoses";
  // Добавляем свойство для сообщения из C++
  messageFromCpp = "";
  // Добавляем свойство для управления видимостью уведомления
  showDevNotification = false;

  mounted() {
    // Проверяем сохраненную тему при загрузке приложения
    const savedTheme = localStorage.getItem("appTheme");
    if (savedTheme) {
      this.currentTheme = savedTheme;
      document.body.setAttribute("data-theme", savedTheme);
    } else {
      // По умолчанию используем FlowOfRoses
      document.body.setAttribute("data-theme", "FlowOfRoses");
    }

    // Подписываемся на событие изменения темы
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    window.addEventListener("themeChanged", this.handleThemeChange as any);

    // Ожидаем, когда QWebChannel будет доступен
    window.onload = () => {
      if (window.QWebChannel && window.qt && window.qt.webChannelTransport) {
        this.initQtConnection();
      } else {
        // Если QWebChannel не доступен, предположим, что мы в режиме разработки
        console.log(
          "QWebChannel не найден, используем заглушки для разработки"
        );
        this.messageFromCpp = "Режим разработки (без Qt)";
        this.showDevNotification = true;
      }
    };
  }

  beforeUnmount() {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    window.removeEventListener("themeChanged", this.handleThemeChange as any);
  }

  handleThemeChange(event: ThemeChangeEvent) {
    if (event.detail && event.detail.theme) {
      this.currentTheme = event.detail.theme;
    }
  }

  // Метод для инициализации соединения с Qt
  async initQtConnection() {
    if (!window.QWebChannel || !window.qt || !window.qt.webChannelTransport) {
      console.error("QWebChannel или WebChannelTransport не доступны");
      return;
    }

    new window.QWebChannel(
      window.qt.webChannelTransport,
      async (channel: any) => {
        const backend = channel.objects.backend;
        window.backend = backend;

        try {
          const message = await backend.getMessage();
          this.messageFromCpp = message;
          console.log("Сообщение из C++:", message);
        } catch (error) {
          console.error("Ошибка при получении сообщения:", error);
        }
      }
    );
  }

  // Метод для закрытия уведомления о режиме разработки
  closeDevNotification() {
    this.showDevNotification = false;
  }
}
</script>

<style>
/* Базовые стили */
#app {
  font-family: "Roboto", "Segoe UI", Tahoma, Geneva, Verdana, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  margin: 0;
  padding: 0;
  min-height: 100vh;
  transition: background-color 0.3s ease, color 0.3s ease;
  position: relative;
  overflow-x: hidden;
}

/* Анимации для переходов между страницами */
.page-enter-active,
.page-leave-active {
  transition: opacity 0.3s, transform 0.3s;
}

.page-enter-from {
  opacity: 0;
  transform: translateX(20px);
}

.page-leave-to {
  opacity: 0;
  transform: translateX(-20px);
}

/* Стиль для уведомления о режиме разработки */
.dev-notification {
  position: fixed;
  top: 70px;
  right: 20px;
  background-color: rgba(255, 193, 7, 0.9);
  color: #333;
  padding: 10px 15px;
  border-radius: 5px;
  z-index: 1000;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.2);
  display: flex;
  align-items: center;
  animation: slide-in 0.3s ease-out;
}

.dev-notification__close {
  margin-left: 10px;
  background: none;
  border: none;
  font-size: 20px;
  line-height: 1;
  cursor: pointer;
  color: #333;
  padding: 0 5px;
}

@keyframes slide-in {
  from {
    transform: translateX(100%);
    opacity: 0;
  }
  to {
    transform: translateX(0);
    opacity: 1;
  }
}
</style>
