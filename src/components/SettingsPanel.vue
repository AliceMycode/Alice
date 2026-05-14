<script setup lang="ts">
import { ref, watch } from 'vue'
import { useSettingsStore } from '@/stores/settings'

const settings = useSettingsStore()

const focus = ref(settings.focusDuration)
const shortBreak = ref(settings.shortBreakDuration)
const longBreak = ref(settings.longBreakDuration)
const interval = ref(settings.longBreakInterval)

watch([focus, shortBreak, longBreak, interval], () => {
  settings.update({
    focusDuration: focus.value,
    shortBreakDuration: shortBreak.value,
    longBreakDuration: longBreak.value,
    longBreakInterval: interval.value
  })
}, { deep: false })
</script>

<template>
  <div class="settings-panel">
    <h3>设置</h3>

    <div class="setting-group">
      <label class="setting-label">
        <span>专注时长</span>
        <span class="value">{{ focus }} 分钟</span>
      </label>
      <input type="range" v-model.number="focus" min="5" max="60" step="5" class="slider" />
    </div>

    <div class="setting-group">
      <label class="setting-label">
        <span>短休息时长</span>
        <span class="value">{{ shortBreak }} 分钟</span>
      </label>
      <input type="range" v-model.number="shortBreak" min="1" max="30" step="1" class="slider" />
    </div>

    <div class="setting-group">
      <label class="setting-label">
        <span>长休息时长</span>
        <span class="value">{{ longBreak }} 分钟</span>
      </label>
      <input type="range" v-model.number="longBreak" min="5" max="60" step="5" class="slider" />
    </div>

    <div class="setting-group">
      <label class="setting-label">
        <span>长休息间隔</span>
        <span class="value">每 {{ interval }} 轮</span>
      </label>
      <input type="range" v-model.number="interval" min="2" max="8" step="1" class="slider" />
    </div>
  </div>
</template>

<style scoped>
.settings-panel {
  padding: 8px;
}

h3 {
  font-size: 16px;
  font-weight: 600;
  color: #d1d5db;
  margin-bottom: 20px;
}

.setting-group {
  margin-bottom: 20px;
}

.setting-label {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 13px;
  color: #9ca3af;
  margin-bottom: 8px;
}

.value {
  color: #818cf8;
  font-weight: 500;
}

.slider {
  width: 100%;
  height: 4px;
  -webkit-appearance: none;
  appearance: none;
  background: rgba(255, 255, 255, 0.08);
  border-radius: 2px;
  outline: none;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 16px;
  height: 16px;
  border-radius: 50%;
  background: #818cf8;
  cursor: pointer;
  box-shadow: 0 0 10px rgba(99, 102, 241, 0.4);
  transition: box-shadow 0.2s;
}

.slider::-webkit-slider-thumb:hover {
  box-shadow: 0 0 16px rgba(99, 102, 241, 0.6);
}
</style>
