<template>
  <div class="home">
    <Header />
    <p class="home__title">Альбом</p>
    <div class="home__card"></div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";
import Header from "@/components/Header.vue";

@Options({
  components: {
    Header,
  },
})
export default class HomeView extends Vue {}
</script>

<style scoped lang="scss">
.home {
  &__title {
    text-align: center;
    margin-top: 30px;
    font-style: normal;
    font-weight: normal;
    font-size: 36px;
    line-height: 42px;
    color: #ffffff;
  }
}
</style>
