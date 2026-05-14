<script setup lang="ts">
import { useTimerStore } from '@/stores/timer'

const timer = useTimerStore()
</script>

<template>
  <div class="controls">
    <button
      v-if="!timer.isRunning || timer.isPaused"
      class="btn btn-primary"
      @click="timer.start()"
    >
      {{ timer.isPaused ? '继续' : '开始' }}
    </button>
    <button
      v-else
      class="btn btn-warning"
      @click="timer.pause()"
    >
      暂停
    </button>
    <button class="btn btn-ghost" @click="timer.reset()">重置</button>
    <button class="btn btn-ghost" @click="timer.skip()">跳过</button>
  </div>
</template>

<style scoped>
.controls {
  display: flex;
  gap: 12px;
  margin-top: 32px;
}

.btn {
  padding: 10px 28px;
  border: none;
  border-radius: 24px;
  font-size: 14px;
  cursor: pointer;
  font-family: inherit;
  transition: all 0.2s;
}

.btn:active {
  transform: scale(0.96);
}

.btn-primary {
  background: linear-gradient(135deg, #6366f1, #818cf8);
  color: #fff;
  box-shadow: 0 0 20px rgba(99, 102, 241, 0.3);
}

.btn-primary:hover {
  box-shadow: 0 0 30px rgba(99, 102, 241, 0.5);
}

.btn-warning {
  background: linear-gradient(135deg, #f59e0b, #fbbf24);
  color: #1a1a2e;
  box-shadow: 0 0 20px rgba(245, 158, 11, 0.3);
}

.btn-ghost {
  background: rgba(255, 255, 255, 0.05);
  color: #9ca3af;
  border: 1px solid rgba(255, 255, 255, 0.1);
}

.btn-ghost:hover {
  background: rgba(255, 255, 255, 0.1);
  color: #e0e0e0;
}
</style>
