<template>
  <div class="replay-controls">
    <div class="replay-controls__title">Воспроизведение</div>

    <div class="replay-controls__actions">
      <button
        class="replay-controls__btn replay-controls__btn--play"
        @click="startReplay"
        :disabled="isReplaying"
      >
        <span class="replay-controls__icon">▶</span> Воспроизвести
      </button>

      <button
        class="replay-controls__btn replay-controls__btn--stop"
        @click="stopReplay"
        :disabled="!isReplaying"
      >
        <span class="replay-controls__icon">■</span> Остановить
      </button>
    </div>

    <div class="replay-controls__speed">
      <span class="replay-controls__speed-label">Скорость:</span>
      <div class="replay-controls__speed-buttons">
        <button
          v-for="speedOption in speedOptions"
          :key="speedOption.value"
          class="replay-controls__speed-btn"
          :class="{
            'replay-controls__speed-btn--active': speed === speedOption.value,
          }"
          @click="setSpeed(speedOption.value)"
        >
          {{ speedOption.label }}
        </button>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    isReplaying: {
      type: Boolean,
      default: false,
    },
  },
  emits: ["replay", "stop-replay", "speed-change"],
})
export default class ReplayControls extends Vue {
  isReplaying!: boolean;

  speed = 1;

  speedOptions = [
    { value: 0.5, label: "0.5x" },
    { value: 1, label: "1x" },
    { value: 2, label: "2x" },
    { value: 5, label: "5x" },
  ];

  startReplay() {
    this.$emit("replay", this.speed);
  }

  stopReplay() {
    this.$emit("stop-replay");
  }

  setSpeed(speed: number) {
    this.speed = speed;
    this.$emit("speed-change", speed);
  }
}
</script>

<style scoped lang="scss">
.replay-controls {
  background-color: var(--card-background, #fff);
  border-radius: 10px;
  padding: 15px;
  margin-top: 20px;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);

  &__title {
    font-size: 16px;
    font-weight: 500;
    color: var(--card-text, #333);
    margin-bottom: 12px;
  }

  &__actions {
    display: flex;
    gap: 10px;
    margin-bottom: 15px;
  }

  &__btn {
    display: flex;
    align-items: center;
    gap: 5px;
    padding: 8px 16px;
    border-radius: 20px;
    font-size: 14px;
    font-weight: 500;
    cursor: pointer;
    transition: all 0.2s ease;

    &--play {
      background-color: var(--primary-color, #cf6a6a);
      color: white;

      &:hover:not(:disabled) {
        background-color: var(--primary-dark, #a74040);
      }
    }

    &--stop {
      background-color: #f5f5f5;
      color: #333;

      &:hover:not(:disabled) {
        background-color: #e0e0e0;
      }
    }

    &:disabled {
      opacity: 0.6;
      cursor: not-allowed;
    }
  }

  &__icon {
    font-size: 12px;
  }

  &__speed {
    display: flex;
    align-items: center;
    flex-wrap: wrap;
    gap: 10px;
  }

  &__speed-label {
    font-size: 14px;
    color: var(--card-text, #333);
  }

  &__speed-buttons {
    display: flex;
    gap: 5px;
  }

  &__speed-btn {
    padding: 5px 10px;
    border-radius: 15px;
    font-size: 12px;
    background-color: #f0f0f0;
    color: #555;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: #e0e0e0;
    }

    &--active {
      background-color: var(--accent-color, #f8d0d0);
      color: var(--primary-dark, #a74040);
      font-weight: 500;
    }
  }
}

// Адаптивный дизайн
@media (max-width: 480px) {
  .replay-controls {
    &__actions {
      flex-direction: column;
    }

    &__btn {
      width: 100%;
      justify-content: center;
    }

    &__speed {
      flex-direction: column;
      align-items: flex-start;
    }
  }
}
</style>
