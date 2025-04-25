<template>
  <transition name="modal">
    <div v-if="show" class="delete-modal">
      <div class="delete-modal__content">
        <h3>Удалить изображение?</h3>
        <p>Это действие нельзя будет отменить.</p>
        <div class="delete-modal__buttons">
          <button class="delete-modal__btn" @click="$emit('cancel')">
            Отмена
          </button>
          <button
            class="delete-modal__btn delete-modal__btn--delete"
            @click="$emit('confirm')"
          >
            Удалить
          </button>
        </div>
      </div>
    </div>
  </transition>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    show: Boolean,
  },
  emits: ["cancel", "confirm"],
})
export default class DeleteConfirmModal extends Vue {
  show!: boolean;
}
</script>

<style scoped lang="scss">
.delete-modal {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  z-index: 1000;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;

  &__content {
    width: 400px;
    padding: 20px;
    background-color: white;
    border-radius: 12px;
    text-align: center;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);

    h3 {
      margin-bottom: 10px;
      color: #333;
    }

    p {
      color: #666;
      margin-bottom: 20px;
    }
  }

  &__buttons {
    display: flex;
    justify-content: center;
    gap: 15px;
  }

  &__btn {
    padding: 10px 20px;
    border-radius: 30px;
    background-color: var(--primary-color, #cf6a6a);
    color: white;
    font-size: 16px;
    cursor: pointer;
    transition: all 0.2s ease;

    &:hover {
      background-color: var(--primary-dark, #a74040);
    }

    &--delete {
      background-color: #ff5252;

      &:hover {
        background-color: #d32f2f;
      }
    }
  }
}

// Анимации для модального окна
.modal-enter-active,
.modal-leave-active {
  transition: all 0.3s ease;
}

.modal-enter-from,
.modal-leave-to {
  opacity: 0;
}

.modal-enter-from .delete-modal__content,
.modal-leave-to .delete-modal__content {
  transform: scale(0.9);
  transition: transform 0.3s ease;
}
</style>
