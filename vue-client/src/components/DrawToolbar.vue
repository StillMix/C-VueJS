<template>
  <div class="draw-toolbar">
    <button class="draw-toolbar__upload-btn" @click="$emit('upload')">
      <AddImg :width="28" :height="28" />
      <span>Добавить изображение</span>
    </button>

    <div class="draw-toolbar__filters">
      <button
        v-for="filter in filters"
        :key="filter.id"
        class="draw-toolbar__filter-btn"
        :class="{
          'draw-toolbar__filter-btn--active': currentFilter === filter.id,
        }"
        @click="$emit('filter-change', filter.id)"
      >
        {{ filter.name }}
      </button>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import AddImg from "@/assets/icons/addImg.vue";

@Options({
  components: {
    AddImg,
  },
  props: {
    currentFilter: String,
  },
  emits: ["upload", "filter-change"],
})
export default class DrawToolbar extends Vue {
  currentFilter!: string;

  filters = [
    { id: "all", name: "Все" },
    { id: "recent", name: "Недавние" },
    { id: "favorite", name: "Избранные" },
  ];
}
</script>

<style scoped lang="scss">
.draw-toolbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 30px;
  flex-wrap: wrap;
  gap: 15px;

  &__upload-btn {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 10px 16px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    border-radius: 30px;
    font-size: 16px;
    font-weight: 500;
    cursor: pointer;
    transition: all 0.2s ease;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);

    &:hover {
      background-color: var(--primary-dark, #a74040);
      transform: translateY(-2px);
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
    }
  }

  &__filters {
    display: flex;
    gap: 10px;
  }

  &__filter-btn {
    padding: 8px 14px;
    border-radius: 20px;
    background-color: rgba(255, 255, 255, 0.1);
    color: var(--text-color, #ffffff);
    font-size: 14px;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: rgba(255, 255, 255, 0.2);
    }

    &--active {
      background-color: var(--accent-color, #f8d0d0);
      color: var(--primary-dark, #a74040);
      font-weight: 500;
    }
  }
}

// Адаптивный дизайн
@media (max-width: 768px) {
  .draw-toolbar {
    flex-direction: column;
    align-items: flex-start;

    &__filters {
      width: 100%;
      justify-content: center;
    }
  }
}
</style>
