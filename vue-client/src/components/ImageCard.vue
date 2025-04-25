<template>
  <div class="image-card">
    <div class="image-card__inner">
      <img
        :src="imageUrl"
        :alt="imageCaption"
        class="image-card__img"
        loading="lazy"
        @click.prevent="$emit('click', image, index)"
      />

      <div class="image-card__overlay">
        <button
          class="image-card__btn image-card__btn--edit"
          @click.prevent="$emit('click', image, index)"
        >
          <DrawingImg :width="20" :height="20" /> Рисовать
        </button>
        <button
          class="image-card__btn image-card__btn--favorite"
          @click.stop.prevent="$emit('toggle-favorite', image)"
        >
          <span v-if="isFavorite">★</span>
          <span v-else>☆</span>
        </button>
      </div>
    </div>
    <div class="image-card__caption">
      {{ imageCaption }}
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import DrawingImg from "@/assets/icons/drawingImg.vue";

@Options({
  components: {
    DrawingImg,
  },
  props: {
    image: String,
    index: Number,
    isFavorite: Boolean,
  },
  emits: ["click", "toggle-favorite"],
})
export default class ImageCard extends Vue {
  image!: string;
  index!: number;
  isFavorite!: boolean;

  // Изменение в методе get imageUrl() в компоненте ImageCard.vue
  // Метод imageUrl в компоненте ImageCard.vue
  get imageUrl(): string {
    try {
      // Проверяем, начинается ли изображение с "data:" (для загруженных изображений)
      if (this.image && this.image.startsWith("data:")) {
        return this.image;
      }

      // Проверяем, начинается ли имя файла с "userfoto-" (для изображений от пользователя)
      if (this.image && this.image.startsWith("userfoto-")) {
        // В режиме разработки проверяем localStorage
        const savedImage = localStorage.getItem(`userImage_${this.image}`);
        if (savedImage) {
          console.log("Найдено изображение в localStorage:", this.image);
          return savedImage;
        }

        // Если изображение не найдено в localStorage, формируем путь для Qt
        if (window.backend) {
          console.log("Используем путь drawings для Qt:", this.image);
          return `drawings/${this.image}`;
        }
      }

      // Для обычных изображений из Assets
      console.log("Загружаем из assets:", this.image);
      return require(`@/assets/Drawings/${this.image}`);
    } catch (e) {
      console.error("Ошибка при загрузке изображения:", e, this.image);
      // Если не удалось загрузить, проверяем, есть ли путь drawings
      if (this.image && window.backend) {
        return `drawings/${this.image}`;
      }
      return "";
    }
  }

  get imageCaption(): string {
    // Проверяем, начинается ли изображение с "data:" (для загруженных изображений)
    if (this.image && this.image.startsWith("data:")) {
      return "Загруженное изображение";
    }

    // Убираем расширение файла и заменяем подчеркивания на пробелы
    return this.image.replace(/\.[^/.]+$/, "").replace(/_/g, " ");
  }
}
</script>

<style scoped lang="scss">
.image-card {
  opacity: 0;
  transform: translateY(20px);
  animation: cardAppear 0.5s forwards;
  position: relative;
  transition: all 0.3s ease;

  &__inner {
    position: relative;
    border-radius: 12px;
    overflow: hidden;
    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
    transition: all 0.3s ease;
    aspect-ratio: 4/3;

    &:hover {
      transform: translateY(-5px);
      box-shadow: 0 8px 20px rgba(0, 0, 0, 0.15);

      .image-card__overlay {
        opacity: 1;
      }
    }
  }

  &__caption {
    margin-top: 10px;
    font-size: 16px;
    color: var(--text-color, #ffffff);
    text-align: center;
  }

  &__overlay {
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

  &__btn {
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

  &__img {
    width: 100%;
    height: 100%;
    object-fit: cover;
    transition: transform 0.5s ease;
    cursor: pointer;

    &:hover {
      transform: scale(1.05);
    }
  }
}

@keyframes cardAppear {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}
</style>
