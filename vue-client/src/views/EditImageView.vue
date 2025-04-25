<template>
  <div class="edit-image">
    <Header />

    <div class="edit-image__content">
      <h1 class="edit-image__title">Рисование</h1>

      <div class="edit-image__container">
        <div class="edit-image__preview">
          <img
            v-if="imageUrl"
            :src="imageUrl"
            alt="Изображение для рисования"
            class="edit-image__img"
          />
          <div v-else class="edit-image__placeholder">
            Изображение не найдено
          </div>
        </div>

        <div class="edit-image__tools">
          <h3 class="edit-image__tools-title">Инструменты</h3>

          <div class="edit-image__tool-group">
            <h4>Фильтры</h4>
            <div class="edit-image__filters">
              <button
                v-for="filter in filters"
                :key="filter.id"
                class="edit-image__filter-btn"
                :class="{
                  'edit-image__filter-btn--active': currentFilter === filter.id,
                }"
                @click="applyFilter(filter.id)"
              >
                {{ filter.name }}
              </button>
            </div>
          </div>

          <div class="edit-image__tool-group">
            <h4>Яркость</h4>
            <input
              type="range"
              min="0"
              max="200"
              v-model="brightness"
              class="edit-image__slider"
              @input="updateFilters"
            />
          </div>

          <div class="edit-image__tool-group">
            <h4>Контраст</h4>
            <input
              type="range"
              min="0"
              max="200"
              v-model="contrast"
              class="edit-image__slider"
              @input="updateFilters"
            />
          </div>

          <div class="edit-image__buttons">
            <button
              class="edit-image__btn edit-image__btn--reset"
              @click="resetFilters"
            >
              Сбросить
            </button>
            <button
              class="edit-image__btn edit-image__btn--save"
              @click="saveImage"
            >
              Сохранить
            </button>
            <button
              class="edit-image__btn edit-image__btn--cancel"
              @click="cancel"
            >
              Отмена
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Header from "@/components/Header.vue";

@Options({
  components: {
    Header,
  },
  props: {
    imageName: String,
  },
})
export default class EditImageView extends Vue {
  imageName!: string;
  imageUrl: string | null = null;
}
</script>
