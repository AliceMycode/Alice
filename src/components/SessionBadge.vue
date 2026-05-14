<script setup lang="ts">
import { computed } from 'vue'
import { useTimerStore } from '@/stores/timer'

const timer = useTimerStore()

const label = computed(() => {
  switch (timer.currentSession) {
    case 'focus': return '专注中'
    case 'shortBreak': return '短休息'
    case 'longBreak': return '长休息'
  }
})

const badgeClass = computed(() => `badge ${timer.currentSession}`)
</script>

<template>
  <div class="session-section">
    <span :class="badgeClass">
      <span class="dot"></span>
      {{ label }}
    </span>
    <span class="round-info">第 {{ timer.completedPomodoros + 1 }} 轮</span>
  </div>
</template>

<style scoped>
.session-section {
  display: flex;
  align-items: center;
  gap: 16px;
}

.badge {
  display: flex;
  align-items: center;
  gap: 6px;
  padding: 6px 16px;
  border-radius: 20px;
  font-size: 12px;
  letter-spacing: 1px;
  text-transform: uppercase;
}

.badge.focus {
  background: rgba(20, 184, 166, 0.15);
  color: #5eead4;
}

.badge.shortBreak {
  background: rgba(52, 211, 153, 0.15);
  color: #6ee7b7;
}

.badge.longBreak {
  background: rgba(245, 158, 11, 0.15);
  color: #fcd34d;
}

.dot {
  width: 6px;
  height: 6px;
  border-radius: 50%;
  background: currentColor;
  box-shadow: 0 0 6px currentColor;
}

.round-info {
  font-size: 13px;
  color: #6b7280;
}
</style>
