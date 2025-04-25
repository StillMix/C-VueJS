<template>
  <div id="app">
    <BackgroundAnimation :theme="currentTheme" />
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

@Options({
  components: {
    BackgroundAnimation,
  },
})
export default class App extends Vue {
  // Устанавливаем начальную тему
  currentTheme = "FlowOfRoses";

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
</style>
