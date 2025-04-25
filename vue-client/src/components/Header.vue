<template>
  <div class="header" :class="{ 'header--scrolled': isScrolled }">
    <div class="header__brand">
      <span class="header__logo">АП</span>
    </div>

    <div class="header__nav">
      <router-link
        class="header__nav-item"
        to="/"
        active-class="header__nav-item--active"
      >
        <AddImg class="header__icon" />
        <span class="header__nav-text">Главная</span>
      </router-link>

      <router-link
        class="header__nav-item"
        to="/draw"
        active-class="header__nav-item--active"
      >
        <DrawingImg class="header__icon" />
        <span class="header__nav-text">Рисовать</span>
      </router-link>
    </div>

    <div class="header__actions">
      <button
        class="header__action-btn"
        @click="toggleSettings"
        :class="{ 'header__action-btn--active': isSettingsOpen }"
      >
        <SettingsApp class="header__icon header__icon--settings" />
      </button>
    </div>

    <!-- Выпадающее меню настроек -->
    <transition name="fade-slide">
      <div v-if="isSettingsOpen" class="header__settings-menu">
        <div class="header__settings-title">
          <span>Выбор темы оформления</span>
        </div>
        <div
          v-for="theme in themes"
          :key="theme.id"
          @click="setTheme(theme.id)"
          class="header__settings-item"
          :class="{
            'header__settings-item--active': currentTheme === theme.id,
          }"
        >
          <span
            class="header__settings-theme-color"
            :style="{ backgroundColor: theme.color }"
          ></span>
          <span>{{ theme.name }}</span>
          <span
            v-if="currentTheme === theme.id"
            class="header__settings-item-active"
            >✓</span
          >
        </div>
      </div>
    </transition>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import AddImg from "@/assets/icons/addImg.vue";
import DrawingImg from "@/assets/icons/drawingImg.vue";
import SettingsApp from "@/assets/icons/settingsApp.vue";

// Определяем интерфейс для темы
interface Theme {
  id: string;
  name: string;
  color: string;
}

// Определяем тип для события смены темы
interface ThemeEventDetail {
  theme: string;
}

@Options({
  components: {
    DrawingImg,
    AddImg,
    SettingsApp,
  },
})
export default class Header extends Vue {
  isScrolled = false;
  isSettingsOpen = false;
  currentTheme = "FlowOfRoses";

  // Список доступных тем
  themes: Theme[] = [
    { id: "FlowOfRoses", name: "Flow Of Roses", color: "#a74040" },
    { id: "CottonCandy", name: "Cotton Candy", color: "#f5a9cb" },
    { id: "Space", name: "Space", color: "#1a1a2e" },
    { id: "Anime", name: "Anime", color: "#6a89cc" },
  ];

  mounted() {
    // Отслеживаем скролл страницы
    window.addEventListener("scroll", this.handleScroll);

    // Проверяем сохраненную тему
    const savedTheme = localStorage.getItem("appTheme");
    if (savedTheme && this.themes.some((theme) => theme.id === savedTheme)) {
      this.currentTheme = savedTheme;
      document.body.setAttribute("data-theme", savedTheme);
    } else {
      // По умолчанию используем FlowOfRoses
      document.body.setAttribute("data-theme", "FlowOfRoses");
    }
  }

  beforeUnmount() {
    window.removeEventListener("scroll", this.handleScroll);
  }

  handleScroll() {
    // Изменяем стиль хедера при скролле
    this.isScrolled = window.scrollY > 10;
  }

  setTheme(themeId: string) {
    this.currentTheme = themeId;
    document.body.setAttribute("data-theme", themeId);
    localStorage.setItem("appTheme", themeId);

    // Отправляем событие изменения темы для App.vue
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    const themeEvent = new CustomEvent("themeChanged", {
      detail: { theme: themeId },
    });
    window.dispatchEvent(themeEvent);

    // Закрываем меню после выбора
    this.isSettingsOpen = false;
  }

  toggleSettings() {
    this.isSettingsOpen = !this.isSettingsOpen;
  }
}
</script>

<style lang="scss">
// Переменные для тем
:root {
  // Flow Of Roses (красная тема)
  --roses-primary: #cf6a6a;
  --roses-dark: #a74040;
  --roses-light: #dd8a8a;
  --roses-accent: #f8d0d0;

  // Cotton Candy (розовая тема)
  --candy-primary: #f8a5c2;
  --candy-dark: #f78fb3;
  --candy-light: #fbc8d4;
  --candy-accent: #ffebf0;

  // Space (темная тема)
  --space-primary: #303952;
  --space-dark: #1a1a2e;
  --space-light: #546de5;
  --space-accent: #6a89cc;

  // Anime (яркая тема)
  --anime-primary: #6a89cc;
  --anime-dark: #4a69bd;
  --anime-light: #82ccdd;
  --anime-accent: #b8e994;

  // Общие переменные для всех тем
  --text-light: #ffffff;
  --text-dark: #333333;
  --shadow-color: rgba(0, 0, 0, 0.1);
  --transition-time: 0.3s;
  --border-radius: 30px;
}

// Установка основной темы (FlowOfRoses по умолчанию)
body[data-theme="FlowOfRoses"] {
  --primary-color: var(--roses-primary);
  --primary-dark: var(--roses-dark);
  --primary-light: var(--roses-light);
  --accent-color: var(--roses-accent);
  --background-color: var(--roses-dark);

  background-color: var(--background-color);
}

body[data-theme="CottonCandy"] {
  --primary-color: var(--candy-primary);
  --primary-dark: var(--candy-dark);
  --primary-light: var(--candy-light);
  --accent-color: var(--candy-accent);
  --background-color: var(--candy-dark);

  background-color: var(--background-color);
}

body[data-theme="Space"] {
  --primary-color: var(--space-primary);
  --primary-dark: var(--space-dark);
  --primary-light: var(--space-light);
  --accent-color: var(--space-accent);
  --background-color: var(--space-dark);

  background-color: var(--background-color);
}

body[data-theme="Anime"] {
  --primary-color: var(--anime-primary);
  --primary-dark: var(--anime-dark);
  --primary-light: var(--anime-light);
  --accent-color: var(--anime-accent);
  --background-color: var(--anime-dark);

  background-color: var(--background-color);
}

.header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  width: 100%;
  background-color: var(--primary-color);
  height: 60px;
  padding: 0 16px;
  position: sticky;
  top: 0;
  z-index: 100;
  transition: all var(--transition-time) ease-in-out,
    box-shadow var(--transition-time) ease-in-out;

  &--scrolled {
    height: 50px;
    box-shadow: 0 4px 10px var(--shadow-color);

    .header__logo {
      width: 32px;
      height: 32px;
      font-size: 16px;
    }

    .header__title {
      font-size: 14px;
    }

    .header__icon {
      transform: scale(0.9);
    }
  }

  &__brand {
    display: flex;
    align-items: center;
  }

  &__logo {
    display: flex;
    align-items: center;
    justify-content: center;
    width: 36px;
    height: 36px;
    background-color: var(--primary-dark);
    color: var(--text-light);
    border-radius: 50%;
    font-weight: bold;
    font-size: 18px;
    transition: all var(--transition-time) ease-in-out;
  }

  &__title {
    margin-left: 10px;
    font-size: 16px;
    font-weight: 500;
    color: var(--text-light);
    transition: all var(--transition-time) ease-in-out;
    display: none;

    @media (min-width: 768px) {
      display: block;
    }
  }

  &__nav {
    display: flex;
    align-items: center;
    gap: 10px;
  }

  &__nav-item {
    display: flex;
    align-items: center;
    padding: 8px 12px;
    border-radius: var(--border-radius);
    color: var(--text-light);
    text-decoration: none;
    transition: all var(--transition-time) ease;
    position: relative;

    &:hover {
      background-color: rgba(255, 255, 255, 0.1);
    }

    &--active {
      background-color: rgba(255, 255, 255, 0.2);

      &::after {
        content: "";
        position: absolute;
        bottom: 2px;
        left: 50%;
        transform: translateX(-50%);
        width: 4px;
        height: 4px;
        background-color: var(--text-light);
        border-radius: 50%;
      }
    }
  }

  &__nav-text {
    margin-left: 6px;
    font-size: 14px;
    display: none;

    @media (min-width: 768px) {
      display: block;
    }
  }

  &__icon {
    width: 24px;
    height: 24px;
    transition: all var(--transition-time) ease-in-out;

    &--settings {
      animation: pulse 2s infinite;
      animation-play-state: paused;
    }
  }

  &__actions {
    display: flex;
    align-items: center;
    gap: 10px;
  }

  &__action-btn {
    display: flex;
    align-items: center;
    justify-content: center;
    width: 36px;
    height: 36px;
    border-radius: 50%;
    background-color: transparent;
    border: 1px solid rgba(255, 255, 255, 0.3);
    cursor: pointer;
    transition: all var(--transition-time) ease;

    &:hover {
      background-color: rgba(255, 255, 255, 0.1);
      border-color: rgba(255, 255, 255, 0.5);

      .header__icon--settings {
        animation-play-state: running;
      }
    }

    &--active {
      background-color: rgba(255, 255, 255, 0.2);
      border-color: rgba(255, 255, 255, 0.7);

      .header__icon--settings {
        animation: none;
        transform: rotate(180deg);
      }
    }
  }

  &__settings-menu {
    position: absolute;
    top: 60px;
    right: 16px;
    width: 220px;
    background-color: white;
    border-radius: 8px;
    box-shadow: 0 4px 12px var(--shadow-color);
    z-index: 10;
    overflow: hidden;

    .header--scrolled & {
      top: 50px;
    }
  }

  &__settings-title {
    padding: 12px 16px;
    font-size: 15px;
    font-weight: 500;
    color: #555;
    border-bottom: 1px solid #eee;
  }

  &__settings-item {
    padding: 12px 16px;
    font-size: 14px;
    color: var(--text-dark);
    cursor: pointer;
    transition: background-color var(--transition-time) ease;
    display: flex;
    align-items: center;

    &:hover {
      background-color: rgba(0, 0, 0, 0.05);
    }

    &--active {
      background-color: rgba(0, 0, 0, 0.03);
    }
  }

  &__settings-theme-color {
    display: inline-block;
    width: 14px;
    height: 14px;
    border-radius: 50%;
    margin-right: 10px;
    border: 1px solid rgba(0, 0, 0, 0.1);
  }

  &__settings-item-active {
    margin-left: auto;
    font-weight: bold;
    color: var(--primary-dark);
  }
}

// Анимации
@keyframes pulse {
  0% {
    transform: scale(1);
  }
  50% {
    transform: scale(1.1);
  }
  100% {
    transform: scale(1);
  }
}

// Transition для выпадающего меню
.fade-slide-enter-active,
.fade-slide-leave-active {
  transition: all var(--transition-time) ease;
}

.fade-slide-enter-from,
.fade-slide-leave-to {
  opacity: 0;
  transform: translateY(-10px);
}
</style>
