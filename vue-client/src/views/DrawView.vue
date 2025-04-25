<template>
  <div class="draw">
    <Header />
    <p class="draw__title">Рисование</p>
    <button class="draw__button"><AddImg /> Добавить изображение</button>
    <div class="draw__card">
      <router-link
        class="draw__card-item"
        to="/"
        active-class="draw__card-item--active"
        v-for="(item, index) in imagesCard"
        :key="index"
      >
        <img
          :src="getImageUrl(item)"
          :alt="`Изображение ${index + 1}`"
          class="draw__image"
        />
      </router-link>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Header from "@/components/Header.vue";
import initialCard from "../assets/human/images";
import AddImg from "@/assets/icons/addImg.vue";

@Options({
  components: {
    Header,
    AddImg,
  },
})
export default class DrawView extends Vue {
  imagesCard = initialCard;

  getImageUrl(name: string): string {
    try {
      return require(`@/assets/Drawings/${name}`);
    } catch (e) {
      console.error(e);
      return "";
    }
  }
}
</script>

<style scoped lang="scss">
.draw {
  &__title {
    text-align: center;
    margin-top: 30px;
    font-style: normal;
    font-weight: normal;
    font-size: 36px;
    line-height: 42px;
    color: #ffffff;
  }

  &__button {
    font-style: normal;
    font-weight: normal;
    font-size: 36px;
    line-height: 42px;
    color: #ffffff;
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-left: auto;
    margin-right: auto;
  }

  &__card {
    width: 90%;
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
    gap: 20px;
    margin: 20px;
    &-item {
      display: block;
      width: 307px;
      height: 240px;
      overflow: hidden;
      &:hover {
        border: 2px solid #a74040;
        blur: 20px
      }
    }
  }

  &__image {
    width: 307px;
    height: 240px;
    object-fit: cover;
    border-radius: 8px;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
  }
}
</style>
