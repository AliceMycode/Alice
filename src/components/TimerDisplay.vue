<script setup lang="ts">
import { computed } from 'vue'
import { useTimerStore } from '@/stores/timer'

const timer = useTimerStore()

const minutes = computed(() => Math.floor(timer.remainingSeconds / 60))
const seconds = computed(() => timer.remainingSeconds % 60)

const display = computed(() =>
  `${String(minutes.value).padStart(2, '0')}:${String(seconds.value).padStart(2, '0')}`
)

const circumference = 2 * Math.PI * 130
const dashOffset = computed(() => circumference * (1 - timer.progress))

const ringColor = computed(() => {
  switch (timer.currentSession) {
    case 'focus': return '#2dd4bf'
    case 'shortBreak': return '#34d399'
    case 'longBreak': return '#f59e0b'
  }
})
</script>

<template>
  <div class="timer-display">
    <svg class="ring" viewBox="0 0 300 300">
      <circle
        cx="150" cy="150" r="130"
        fill="none"
        stroke="rgba(255,255,255,0.05)"
        stroke-width="6"
      />
      <circle
        cx="150" cy="150" r="130"
        fill="none"
        :stroke="ringColor"
        stroke-width="6"
        stroke-linecap="round"
        :stroke-dasharray="circumference"
        :stroke-dashoffset="dashOffset"
        transform="rotate(-90 150 150)"
        class="progress-ring"
      />
    </svg>
    <div class="time-text">{{ display }}</div>
  </div>
</template>

<style scoped>
.timer-display {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  width: 300px;
  height: 300px;
}

.ring {
  position: absolute;
  width: 300px;
  height: 300px;
  filter: drop-shadow(0 0 12px rgba(20, 184, 166, 0.3));
}

.progress-ring {
  transition: stroke-dashoffset 0.9s linear;
}

.time-text {
  font-size: 52px;
  font-weight: 600;
  letter-spacing: 4px;
  background: linear-gradient(135deg, #2dd4bf, #34d399);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  font-variant-numeric: tabular-nums;
}
</style>
