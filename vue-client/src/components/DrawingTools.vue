<template>
  <div class="drawing-tools">
    <div class="drawing-tools__group">
      <label class="drawing-tools__label">Цвет:</label>
      <div class="drawing-tools__color-picker">
        <div
          v-for="color in colors"
          :key="color"
          class="drawing-tools__color"
          :style="{ backgroundColor: color }"
          :class="{ 'drawing-tools__color--active': selectedColor === color }"
          @click="selectColor(color)"
        ></div>
        <input
          type="color"
          v-model="customColor"
          class="drawing-tools__custom-color"
          @input="selectColor(customColor)"
        />
      </div>
    </div>

    <div class="drawing-tools__group">
      <label class="drawing-tools__label">Толщина:</label>
      <div class="drawing-tools__thickness-slider">
        <input
          type="range"
          min="1"
          max="30"
          v-model="thickness"
          class="drawing-tools__slider"
          @input="$emit('thickness-change', parseInt(thickness))"
        />
        <span class="drawing-tools__slider-value">{{ thickness }}px</span>
      </div>
    </div>

    <div class="drawing-tools__group">
      <button
        class="drawing-tools__btn drawing-tools__btn--clear"
        @click="$emit('clear')"
      >
        Очистить
      </button>
      <button
        class="drawing-tools__btn drawing-tools__btn--finish"
        @click="$emit('finish')"
      >
        Готово
      </button>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  emits: ["color-change", "thickness-change", "clear", "finish"],
})
export default class DrawingTools extends Vue {
  // Предопределенные цвета
  colors = [
    "#000000", // Черный
    "#ff0000", // Красный
    "#00ff00", // Зеленый
    "#0000ff", // Синий
    "#ffff00", // Желтый
    "#ff00ff", // Пурпурный
    "#00ffff", // Голубой
  ];

  selectedColor = "#000000";
  customColor = "#000000";
  thickness = "3";

  selectColor(color: string) {
    this.selectedColor = color;
    this.$emit("color-change", color);
  }
}
</script>

<style scoped lang="scss">
.drawing-tools {
  display: flex;
  flex-wrap: wrap;
  gap: 20px;
  padding: 15px;
  background-color: var(--card-background, #fff);
  border-radius: 10px;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);

  &__group {
    display: flex;
    flex-direction: column;
    flex: 1;
    min-width: 150px;
  }

  &__label {
    font-size: 14px;
    margin-bottom: 8px;
    color: var(--card-text, #333);
    font-weight: 500;
  }

  &__color-picker {
    display: flex;
    gap: 8px;
    flex-wrap: wrap;
  }

  &__color {
    width: 30px;
    height: 30px;
    border-radius: 50%;
    cursor: pointer;
    border: 2px solid transparent;
    transition: all 0.2s ease;

    &:hover {
      transform: scale(1.1);
    }

    &--active {
      border-color: var(--primary-dark, #a74040);
      transform: scale(1.1);
    }
  }

  &__custom-color {
    width: 30px;
    height: 30px;
    border: none;
    border-radius: 50%;
    overflow: hidden;
    cursor: pointer;
    background-color: transparent;
    transition: all 0.2s ease;

    &::-webkit-color-swatch-wrapper {
      padding: 0;
    }

    &::-webkit-color-swatch {
      border: none;
      border-radius: 50%;
    }

    &:hover {
      transform: scale(1.1);
    }
  }

  &__thickness-slider {
    display: flex;
    align-items: center;
    gap: 10px;
  }

  &__slider {
    flex: 1;
    height: 5px;
    background: #ddd;
    border-radius: 5px;
    appearance: none;
    outline: none;

    &::-webkit-slider-thumb {
      appearance: none;
      width: 15px;
      height: 15px;
      background-color: var(--primary-color, #cf6a6a);
      border-radius: 50%;
      cursor: pointer;
    }

    &::-moz-range-thumb {
      width: 15px;
      height: 15px;
      background-color: var(--primary-color, #cf6a6a);
      border-radius: 50%;
      cursor: pointer;
      border: none;
    }
  }

  &__slider-value {
    min-width: 40px;
    text-align: center;
    font-size: 14px;
    color: var(--card-text, #333);
  }

  &__btn {
    padding: 8px 16px;
    border-radius: 20px;
    font-size: 14px;
    font-weight: 500;
    cursor: pointer;
    transition: all 0.2s ease;
    margin-top: 10px;

    &--clear {
      background-color: #f5f5f5;
      color: #333;

      &:hover {
        background-color: #e0e0e0;
      }
    }

    &--finish {
      background-color: var(--primary-color, #cf6a6a);
      color: white;

      &:hover {
        background-color: var(--primary-dark, #a74040);
      }
    }
  }
}

// Адаптивный дизайн
@media (max-width: 768px) {
  .drawing-tools {
    flex-direction: column;

    &__group {
      width: 100%;
    }
  }
}
</style>
