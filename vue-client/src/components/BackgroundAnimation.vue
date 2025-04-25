<template>
  <div class="background-animation" :class="currentTheme">
    <!-- Space Theme Background -->
    <div v-if="currentTheme === 'Space'" class="space-background">
      <div class="stars-container">
        <div
          v-for="n in 50"
          :key="`star-${n}`"
          class="star"
          :style="randomStarStyle()"
        ></div>
      </div>

      <svg class="planet planet-1" viewBox="0 0 100 100" width="80" height="80">
        <circle cx="50" cy="50" r="45" fill="#6a89cc" />
        <ellipse cx="50" cy="30" rx="20" ry="10" fill="#546de5" />
        <ellipse cx="30" cy="60" rx="15" ry="10" fill="#546de5" />
      </svg>

      <svg class="planet planet-2" viewBox="0 0 100 100" width="40" height="40">
        <circle cx="50" cy="50" r="45" fill="#f0932b" />
        <circle cx="30" cy="40" r="10" fill="#ffbe76" />
        <circle cx="70" cy="60" r="8" fill="#ffbe76" />
      </svg>

      <svg class="spaceship" viewBox="0 0 100 50" width="60" height="30">
        <ellipse cx="50" cy="25" rx="40" ry="20" fill="#dfe4ea" />
        <circle cx="80" cy="25" r="10" fill="#ced6e0" />
        <rect x="10" y="20" width="5" height="10" fill="#a4b0be" />
        <rect x="90" y="20" width="5" height="10" fill="#a4b0be" />
        <circle
          cx="30"
          cy="25"
          r="8"
          fill="#57606f"
          stroke="#2f3542"
          stroke-width="2"
        />
        <path d="M 10,30 L 0,40 L 0,10 Z" fill="#ff6b6b" />
      </svg>

      <div class="meteor-container">
        <div
          v-for="n in 5"
          :key="`meteor-${n}`"
          class="meteor"
          :style="randomMeteorStyle()"
        ></div>
      </div>
    </div>

    <!-- Flow Of Roses Theme Background -->
    <div v-else-if="currentTheme === 'FlowOfRoses'" class="roses-background">
      <svg class="rose rose-1" viewBox="0 0 100 100" width="60" height="60">
        <path
          d="M50,10 C60,20 90,20 90,50 C90,80 60,90 50,90 C40,90 10,80 10,50 C10,20 40,20 50,10"
          fill="#dd8a8a"
        />
        <path
          d="M50,20 C58,28 80,28 80,50 C80,72 58,80 50,80 C42,80 20,72 20,50 C20,28 42,28 50,20"
          fill="#f8d0d0"
        />
        <circle cx="50" cy="50" r="10" fill="#a74040" />
      </svg>

      <svg class="rose rose-2" viewBox="0 0 100 100" width="40" height="40">
        <path
          d="M50,10 C60,20 90,20 90,50 C90,80 60,90 50,90 C40,90 10,80 10,50 C10,20 40,20 50,10"
          fill="#dd8a8a"
        />
        <path
          d="M50,20 C58,28 80,28 80,50 C80,72 58,80 50,80 C42,80 20,72 20,50 C20,28 42,28 50,20"
          fill="#f8d0d0"
        />
        <circle cx="50" cy="50" r="10" fill="#a74040" />
      </svg>

      <svg class="rose rose-3" viewBox="0 0 100 100" width="50" height="50">
        <path
          d="M50,10 C60,20 90,20 90,50 C90,80 60,90 50,90 C40,90 10,80 10,50 C10,20 40,20 50,10"
          fill="#dd8a8a"
        />
        <path
          d="M50,20 C58,28 80,28 80,50 C80,72 58,80 50,80 C42,80 20,72 20,50 C20,28 42,28 50,20"
          fill="#f8d0d0"
        />
        <circle cx="50" cy="50" r="10" fill="#a74040" />
      </svg>

      <div class="petal-container">
        <div
          v-for="n in 20"
          :key="`petal-${n}`"
          class="petal"
          :style="randomPetalStyle()"
        ></div>
      </div>
    </div>

    <!-- Cotton Candy Theme Background -->
    <div v-else-if="currentTheme === 'CottonCandy'" class="candy-background">
      <svg class="candy candy-1" viewBox="0 0 100 100" width="60" height="60">
        <circle cx="50" cy="50" r="45" fill="#f8a5c2" />
        <path
          d="M30,30 Q50,10 70,30 T80,50 T70,70 T50,80 T30,70 T20,50 T30,30"
          fill="#f78fb3"
          stroke="#fbc8d4"
          stroke-width="2"
        />
      </svg>

      <svg class="candy candy-2" viewBox="0 0 100 100" width="40" height="40">
        <rect x="20" y="20" width="60" height="60" rx="15" fill="#f8a5c2" />
        <circle cx="40" cy="40" r="10" fill="#ffebf0" />
        <circle cx="60" cy="60" r="10" fill="#ffebf0" />
      </svg>

      <svg class="cotton cotton-1" viewBox="0 0 100 100" width="80" height="80">
        <circle cx="30" cy="30" r="20" fill="#ffebf0" />
        <circle cx="50" cy="20" r="18" fill="#ffebf0" />
        <circle cx="70" cy="30" r="20" fill="#ffebf0" />
        <circle cx="20" cy="50" r="15" fill="#ffebf0" />
        <circle cx="50" cy="50" r="25" fill="#ffebf0" />
        <circle cx="75" cy="50" r="15" fill="#ffebf0" />
        <circle cx="30" cy="70" r="20" fill="#ffebf0" />
        <circle cx="50" cy="75" r="18" fill="#ffebf0" />
        <circle cx="70" cy="70" r="20" fill="#ffebf0" />
      </svg>

      <div class="bubble-container">
        <div
          v-for="n in 20"
          :key="`bubble-${n}`"
          class="bubble"
          :style="randomBubbleStyle()"
        ></div>
      </div>
    </div>

    <!-- Anime Theme Background -->
    <div v-else-if="currentTheme === 'Anime'" class="anime-background">
      <svg class="sakura sakura-1" viewBox="0 0 100 100" width="30" height="30">
        <path
          d="M50,15 C60,25 75,10 50,40 C75,65 60,75 50,60 C40,75 25,65 50,40 C25,10 40,25 50,15"
          fill="#6a89cc"
        />
      </svg>

      <svg class="sakura sakura-2" viewBox="0 0 100 100" width="20" height="20">
        <path
          d="M50,15 C60,25 75,10 50,40 C75,65 60,75 50,60 C40,75 25,65 50,40 C25,10 40,25 50,15"
          fill="#82ccdd"
        />
      </svg>

      <svg class="anime-character" viewBox="0 0 100 120" width="50" height="60">
        <circle cx="50" cy="30" r="20" fill="#ced6e0" />
        <ellipse cx="42" cy="25" rx="3" ry="4" fill="#2f3542" />
        <ellipse cx="58" cy="25" rx="3" ry="4" fill="#2f3542" />
        <path
          d="M45,35 Q50,40 55,35"
          stroke="#2f3542"
          stroke-width="2"
          fill="none"
        />
        <path d="M20,65 L50,50 L80,65 L50,100 Z" fill="#b8e994" />
        <rect x="35" y="100" width="10" height="20" fill="#ced6e0" />
        <rect x="55" y="100" width="10" height="20" fill="#ced6e0" />
      </svg>

      <div class="sparkle-container">
        <div
          v-for="n in 30"
          :key="`sparkle-${n}`"
          class="sparkle"
          :style="randomSparkleStyle()"
        ></div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { Options, Vue } from "vue-class-component";

@Options({
  props: {
    theme: String,
  },
})
export default class BackgroundAnimation extends Vue {
  theme!: string;

  get currentTheme() {
    return this.theme || localStorage.getItem("appTheme") || "FlowOfRoses";
  }

  randomStarStyle() {
    return {
      top: `${Math.random() * 100}%`,
      left: `${Math.random() * 100}%`,
      width: `${Math.random() * 3 + 1}px`,
      height: `${Math.random() * 3 + 1}px`,
      animationDelay: `${Math.random() * 5}s`,
      animationDuration: `${Math.random() * 3 + 2}s`,
    };
  }

  randomMeteorStyle() {
    return {
      top: `${Math.random() * 100}%`,
      left: `${Math.random() * 100}%`,
      width: `${Math.random() * 50 + 50}px`,
      height: `${Math.random() * 1 + 1}px`,
      animationDelay: `${Math.random() * 10}s`,
      animationDuration: `${Math.random() * 2 + 1}s`,
      transform: `rotate(${Math.random() * 45 + 15}deg)`,
    };
  }

  randomPetalStyle() {
    const size = Math.random() * 15 + 5;
    return {
      top: `${Math.random() * 100}%`,
      left: `${Math.random() * 100}%`,
      width: `${size}px`,
      height: `${size * 1.5}px`,
      backgroundColor: Math.random() > 0.5 ? "#f8d0d0" : "#dd8a8a",
      animationDelay: `${Math.random() * 10}s`,
      animationDuration: `${Math.random() * 10 + 10}s`,
      transform: `rotate(${Math.random() * 360}deg)`,
    };
  }

  randomBubbleStyle() {
    const size = Math.random() * 20 + 5;
    return {
      top: `${Math.random() * 100}%`,
      left: `${Math.random() * 100}%`,
      width: `${size}px`,
      height: `${size}px`,
      animationDelay: `${Math.random() * 10}s`,
      animationDuration: `${Math.random() * 8 + 7}s`,
      opacity: Math.random() * 0.5 + 0.1,
    };
  }

  randomSparkleStyle() {
    const size = Math.random() * 6 + 2;
    return {
      top: `${Math.random() * 100}%`,
      left: `${Math.random() * 100}%`,
      width: `${size}px`,
      height: `${size}px`,
      backgroundColor:
        Math.random() > 0.7
          ? "#b8e994"
          : Math.random() > 0.5
          ? "#82ccdd"
          : "#6a89cc",
      animationDelay: `${Math.random() * 5}s`,
      animationDuration: `${Math.random() * 2 + 1}s`,
    };
  }
}
</script>

<style scoped lang="scss">
.background-animation {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  z-index: -1;
  overflow: hidden;
  pointer-events: none;
}

/* Space Theme */
.space-background {
  background-color: var(--space-dark);

  .stars-container {
    position: absolute;
    width: 100%;
    height: 100%;
  }

  .star {
    position: absolute;
    background-color: white;
    border-radius: 50%;
    animation: twinkle ease-in-out infinite;
  }

  .planet {
    position: absolute;
    animation: orbit linear infinite;

    &-1 {
      top: 15%;
      right: 10%;
      animation-duration: 120s;
    }

    &-2 {
      bottom: 25%;
      left: 15%;
      animation-duration: 80s;
    }
  }

  .spaceship {
    position: absolute;
    left: -100px;
    top: 40%;
    animation: flyby 15s linear infinite;
  }

  .meteor-container {
    position: absolute;
    width: 100%;
    height: 100%;
  }

  .meteor {
    position: absolute;
    background: linear-gradient(to right, transparent, white);
    animation: meteor linear infinite;
  }
}

/* Flow Of Roses Theme */
.roses-background {
  .rose {
    position: absolute;
    animation: float ease-in-out infinite;

    &-1 {
      top: 15%;
      right: 10%;
      animation-duration: 8s;
    }

    &-2 {
      bottom: 25%;
      left: 15%;
      animation-duration: 10s;
    }

    &-3 {
      top: 70%;
      right: 20%;
      animation-duration: 7s;
    }
  }

  .petal-container {
    position: absolute;
    width: 100%;
    height: 100%;
  }

  .petal {
    position: absolute;
    border-radius: 50% 0 50% 0;
    animation: fallingPetal linear infinite;
  }
}

/* Cotton Candy Theme */
.candy-background {
  .candy {
    position: absolute;
    animation: wobble ease-in-out infinite;

    &-1 {
      top: 15%;
      right: 10%;
      animation-duration: 8s;
    }

    &-2 {
      bottom: 25%;
      left: 15%;
      animation-duration: 10s;
    }
  }

  .cotton {
    position: absolute;
    opacity: 0.3;
    animation: spin linear infinite;

    &-1 {
      top: 60%;
      right: 20%;
      animation-duration: 60s;
    }
  }

  .bubble-container {
    position: absolute;
    width: 100%;
    height: 100%;
  }

  .bubble {
    position: absolute;
    border-radius: 50%;
    background-color: rgba(255, 255, 255, 0.5);
    animation: bubbleRise ease-in-out infinite;
  }
}

/* Anime Theme */
.anime-background {
  .sakura {
    position: absolute;
    animation: spin linear infinite;
    opacity: 0.7;

    &-1 {
      top: 15%;
      right: 10%;
      animation-duration: 30s;
    }

    &-2 {
      bottom: 25%;
      left: 15%;
      animation-duration: 25s;
    }
  }

  .anime-character {
    position: absolute;
    bottom: 10%;
    right: 10%;
    animation: jump 1s ease-in-out infinite;
  }

  .sparkle-container {
    position: absolute;
    width: 100%;
    height: 100%;
  }

  .sparkle {
    position: absolute;
    transform: rotate(45deg);
    animation: sparkle ease-in-out infinite;
  }
}

/* Animations */
@keyframes twinkle {
  0%,
  100% {
    opacity: 0.2;
  }
  50% {
    opacity: 1;
  }
}

@keyframes orbit {
  0% {
    transform: rotate(0deg) translateX(150px) rotate(0deg);
  }
  100% {
    transform: rotate(360deg) translateX(150px) rotate(-360deg);
  }
}

@keyframes flyby {
  0% {
    transform: translateX(-100px);
  }
  100% {
    transform: translateX(calc(100vw + 100px));
  }
}

@keyframes meteor {
  0% {
    transform: translateX(-100px) translateY(-100px);
  }
  100% {
    transform: translateX(100vw) translateY(100vh);
    opacity: 0;
  }
}

@keyframes float {
  0%,
  100% {
    transform: translateY(0);
  }
  50% {
    transform: translateY(-15px);
  }
}

@keyframes fallingPetal {
  0% {
    transform: translateY(-10vh) rotate(0deg);
    opacity: 0;
  }
  10% {
    opacity: 1;
  }
  100% {
    transform: translateY(100vh) rotate(720deg);
    opacity: 0;
  }
}

@keyframes wobble {
  0%,
  100% {
    transform: translateY(0) rotate(0deg);
  }
  25% {
    transform: translateY(-10px) rotate(5deg);
  }
  75% {
    transform: translateY(10px) rotate(-5deg);
  }
}

@keyframes spin {
  0% {
    transform: rotate(0deg);
  }
  100% {
    transform: rotate(360deg);
  }
}

@keyframes bubbleRise {
  0% {
    transform: translateY(100vh) scale(0);
    opacity: 0;
  }
  10% {
    opacity: 1;
  }
  100% {
    transform: translateY(-100px) scale(1.5);
    opacity: 0;
  }
}

@keyframes jump {
  0%,
  100% {
    transform: translateY(0);
  }
  50% {
    transform: translateY(-10px);
  }
}

@keyframes sparkle {
  0%,
  100% {
    transform: scale(0) rotate(45deg);
    opacity: 0;
  }
  50% {
    transform: scale(1) rotate(45deg);
    opacity: 1;
  }
}
</style>
