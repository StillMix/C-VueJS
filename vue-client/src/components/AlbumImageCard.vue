<template>
  <div class="album-image-card">
    <div class="album-image-card__inner">
      <div class="album-image-card__svg-container" v-html="svgContent"></div>
      <div class="album-image-card__overlay">
        <button
          class="album-image-card__btn album-image-card__btn--view"
          @click.prevent="$emit('click', image)"
        >
          <span class="album-image-card__btn-icon">👁️</span> Просмотр
        </button>
        <button
          class="album-image-card__btn album-image-card__btn--delete"
          @click.stop.prevent="$emit('delete', image)"
        >
          <span class="album-image-card__btn-icon">🗑️</span>
        </button>
      </div>
    </div>
    <div class="album-image-card__caption">
      {{ imageCaption }}
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    image: String,
  },
  emits: ["click", "delete"],
})
export default class AlbumImageCard extends Vue {
  image!: string;
  svgContent: string = "";

  mounted() {
    this.loadSvgContent();
  }

  // В методе loadSvgContent() в файле vue-client/src/components/AlbumImageCard.vue:

  async loadSvgContent() {
    try {
      if (window.backend) {
        // В режиме с Qt-бэкендом загружаем SVG через URL
        this.svgContent = `<img src="drawings://album/${this.image}" alt="${this.imageCaption}" class="album-image-card__svg" />`;
      } else {
        // В режиме разработки используем localStorage
        const svgData = localStorage.getItem(`album_${this.image}`);
        if (svgData) {
          this.svgContent = svgData;
        }
      }
    } catch (error) {
      console.error("Ошибка при загрузке SVG:", error);
    }
  }

  get imageCaption(): string {
    // Убираем префикс и расширение, форматируем дату
    const fileName = this.image
      .replace(/^usersavedraw-/, "")
      .replace(/\.svg$/, "");

    // Извлекаем дату и время из имени файла (формат: DDMMYY-HHMMSS)
    const dateTimeParts = fileName.split("-");
    if (dateTimeParts.length === 2) {
      const datePart = dateTimeParts[0];
      const timePart = dateTimeParts[1];

      // Форматируем дату и время
      if (datePart.length === 6 && timePart.length === 6) {
        const day = datePart.substring(0, 2);
        const month = datePart.substring(2, 4);
        const year = datePart.substring(4, 6);

        const hours = timePart.substring(0, 2);
        const minutes = timePart.substring(2, 4);

        return `${day}.${month}.20${year} ${hours}:${minutes}`;
      }
    }

    // Если формат не соответствует ожидаемому, возвращаем исходное имя
    return fileName;
  }
}
</script>

<style scoped lang="scss">
.album-image-card {
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
    background-color: white;

    &:hover {
      transform: translateY(-5px);
      box-shadow: 0 8px 20px rgba(0, 0, 0, 0.15);

      .album-image-card__overlay {
        opacity: 1;
      }
    }
  }

  &__svg-container {
    width: 100%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;

    :deep(svg) {
      width: 100%;
      height: 100%;
      object-fit: contain;
    }

    :deep(img) {
      width: 100%;
      height: 100%;
      object-fit: contain;
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

    &--view {
      background-color: white;
      color: #333;

      &:hover {
        background-color: var(--accent-color, #f8d0d0);
      }
    }

    &--delete {
      width: 36px;
      height: 36px;
      display: flex;
      align-items: center;
      justify-content: center;
      border-radius: 50%;
      background-color: rgba(255, 255, 255, 0.3);
      color: white;
      font-size: 18px;
      padding: 0;

      &:hover {
        background-color: #ff5252;
        transform: scale(1.1);
      }
    }
  }

  &__btn-icon {
    font-size: 18px;
  }
}

@keyframes cardAppear {
  to {
    opacity: 1;
    transform: translateY(0);
  }
}
</style>
