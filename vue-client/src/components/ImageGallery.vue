<template>
  <transition-group name="gallery" tag="div" class="image-gallery">
    <ImageCard
      v-for="(item, index) in images"
      :key="item"
      :image="item"
      :index="index"
      :is-favorite="favorites.includes(item)"
      :style="{ animationDelay: `${index * 0.05}s` }"
      @click="$emit('image-click', item, index)"
      @toggle-favorite="$emit('toggle-favorite', item)"
    />
  </transition-group>

  <div v-if="images.length === 0" class="image-gallery__empty">
    <p>Изображения не найдены</p>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import ImageCard from "./ImageCard.vue";

@Options({
  components: {
    ImageCard,
  },
  props: {
    images: Array,
    favorites: Array,
  },
  emits: ["image-click", "toggle-favorite"],
})
export default class ImageGallery extends Vue {
  images!: string[];
  favorites!: string[];
  loadAlbumImages() {
    if (window.backend) {
      window.backend
        .getAlbumImages()
        .then((images: string[]) => {
          if (Array.isArray(images)) {
            this.albumImages = images;
          }
        })
        .catch((error: any) => {
          console.error("Ошибка при получении изображений альбома:", error);
        });
    } else {
      // Режим разработки - загружаем из localStorage
      const albumImages: string[] = [];
      for (let i = 0; i < localStorage.length; i++) {
        const key = localStorage.key(i);
        if (key && key.startsWith("album_")) {
          albumImages.push(key.replace("album_", ""));
        }
      }
      this.albumImages = albumImages;
    }
  }
}
</script>

<style scoped lang="scss">
.image-gallery {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
  gap: 25px;
  margin-top: 20px;

  &__empty {
    text-align: center;
    padding: 50px 0;
    color: var(--text-color, #ffffff);
    opacity: 0.7;
  }
}

// Анимации для transition-group
.gallery-enter-active,
.gallery-leave-active {
  transition: all 0.5s ease;
}

.gallery-enter-from,
.gallery-leave-to {
  opacity: 0;
  transform: translateY(30px);
}

// Адаптивный дизайн
@media (max-width: 768px) {
  .image-gallery {
    grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
  }
}
</style>
