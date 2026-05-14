<script setup lang="ts">
import { ref, onUnmounted } from 'vue'
import { useTimerStore } from '@/stores/timer'
import TimerDisplay from '@/components/TimerDisplay.vue'
import TimerControls from '@/components/TimerControls.vue'
import SessionBadge from '@/components/SessionBadge.vue'
import TaskPanel from '@/components/TaskPanel.vue'
import StatsPanel from '@/components/StatsPanel.vue'
import SettingsPanel from '@/components/SettingsPanel.vue'

const timer = useTimerStore()

const tabs = [
  { key: 'tasks', label: '任务' },
  { key: 'stats', label: '统计' },
  { key: 'settings', label: '设置' }
] as const

type TabKey = (typeof tabs)[number]['key']
const activeTab = ref<TabKey>('tasks')

onUnmounted(() => {
  timer.cleanup()
})
</script>

<template>
  <div class="app">
    <div class="main-area">
      <SessionBadge />
      <TimerDisplay />
      <TimerControls />
    </div>

    <div class="side-panel">
      <div class="tab-bar">
        <button
          v-for="tab in tabs"
          :key="tab.key"
          :class="['tab-btn', { active: activeTab === tab.key }]"
          @click="activeTab = tab.key"
        >
          {{ tab.label }}
        </button>
      </div>

      <div class="tab-content">
        <TaskPanel v-if="activeTab === 'tasks'" />
        <StatsPanel v-if="activeTab === 'stats'" />
        <SettingsPanel v-if="activeTab === 'settings'" />
      </div>
    </div>
  </div>
</template>

<style scoped>
.app {
  display: flex;
  height: 100vh;
  padding: 24px;
}

.main-area {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding-right: 24px;
}

.side-panel {
  width: 300px;
  min-width: 260px;
  border-left: 1px solid rgba(255, 255, 255, 0.06);
  display: flex;
  flex-direction: column;
  background: rgba(255, 255, 255, 0.015);
}

.tab-bar {
  display: flex;
  border-bottom: 1px solid rgba(255, 255, 255, 0.06);
}

.tab-btn {
  flex: 1;
  padding: 12px 0;
  background: none;
  border: none;
  color: #6b7280;
  font-size: 13px;
  cursor: pointer;
  font-family: inherit;
  position: relative;
  transition: color 0.2s;
}

.tab-btn.active {
  color: #a5b4fc;
}

.tab-btn.active::after {
  content: '';
  position: absolute;
  bottom: -1px;
  left: 25%;
  width: 50%;
  height: 2px;
  background: #6366f1;
  border-radius: 1px;
}

.tab-btn:hover {
  color: #9ca3af;
}

.tab-content {
  flex: 1;
  overflow-y: auto;
  padding: 16px;
}

.tab-content::-webkit-scrollbar {
  width: 4px;
}

.tab-content::-webkit-scrollbar-thumb {
  background: rgba(255, 255, 255, 0.1);
  border-radius: 2px;
}
</style>
