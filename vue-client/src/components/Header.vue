<template>
  <div class="Header">
    <router-link class="Header__add" to="/"
      ><AddImg customClass="Header__add__img"
    /></router-link>
    <router-link class="Header__draw" to="/draw"
      ><DrawingImg customClass="Header__draw__img"
    /></router-link>
    <button class="Header__settings">
      <SettingsApp customClass="Header__settings__img" />
    </button>
  </div>
</template>

<script lang="ts">
import AddImg from "@/assets/icons/addImg.vue";
import DrawingImg from "@/assets/icons/drawingImg.vue";
import SettingsApp from "@/assets/icons/settingsApp.vue";
import { Options, Vue } from "vue-class-component";

@Options({
  components: {
    DrawingImg,
    AddImg,
    SettingsApp,
  },
})
export default class Header extends Vue {}
</script>

<style lang="scss">
.Header {
  display: flex;
  width: 100%;
  background-color: #cf6a6a;
  height: 47px;
  align-items: center;
  &__add {
    width: 30px;
    height: 30px;
    border-radius: 100%;
    border: 1px solid white;
    display: flex;
    align-items: center;
    justify-content: center;
    margin-left: 10px;
    &__img {
      width: 30px;
      height: 30px;
    }
  }
  &__draw {
    width: 30px;
    height: 30px;
    margin-left: 10px;
    border-radius: 100%;
    border: 1px solid white;
    display: flex;
    align-items: center;
    justify-content: center;
    &__img {
      width: 20px;
      height: 20px;
    }
  }
  &__settings {
    width: 30px;
    margin-left: auto;
    margin-right: 10px;
    height: 30px;
    border-radius: 100%;
    border: 1px solid white;
    display: flex;
    align-items: center;
    justify-content: center;
    &__img {
      width: 20px;
      height: 20px;
    }
  }
}
</style>
