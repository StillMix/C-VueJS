<template>
  <svg
    :width="width"
    :height="height"
    :class="customClass"
    viewBox="0 0 24 24"
    xmlns="http://www.w3.org/2000/svg"
  >
    <title />

    <g id="Complete">
      <g data-name="add" id="add-2">
        <g>
          <line
            fill="none"
            :stroke="color"
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-width="2"
            x1="12"
            x2="12"
            y1="19"
            y2="5"
          />

          <line
            fill="none"
            :stroke="color"
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-width="2"
            x1="5"
            x2="19"
            y1="12"
            y2="12"
          />
        </g>
      </g>
    </g>
  </svg>
</template>

<script lang="ts">
import { defineComponent } from "vue";

export default defineComponent({
  name: "addImg",
  props: {
    width: {
      type: [String, Number],
      default: "24",
    },
    height: {
      type: [String, Number],
      default: "24",
    },
    color: {
      type: String,
      default: "white",
    },
    customClass: {
      type: String,
      default: "",
    },
  },
});
</script>

<style>
svg {
  display: block;
  min-width: 0;
  min-height: 0;
}
</style>
