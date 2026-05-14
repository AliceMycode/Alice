<script setup lang="ts">
import { computed } from 'vue'
import { useStatsStore } from '@/stores/stats'
import { getTodayKey } from '@/utils/persistence'

const statsStore = useStatsStore()

const todayKey = computed(() => getTodayKey())

const recentDays = computed(() => {
  const entries = Object.entries(statsStore.stats)
    .sort(([a], [b]) => b.localeCompare(a))
    .slice(0, 14)
  return entries
})

function formatDate(dateKey: string): string {
  const d = new Date(dateKey)
  return `${d.getMonth() + 1}/${d.getDate()}`
}

function barHeight(count: number): number {
  const max = Math.max(...Object.values(statsStore.stats), 1)
  return Math.max((count / max) * 80, 4)
}
</script>

<template>
  <div class="stats-panel">
    <div class="stats-summary">
      <div class="stat-card">
        <div class="stat-number">{{ statsStore.todayCount }}</div>
        <div class="stat-label">今日番茄</div>
      </div>
      <div class="stat-card">
        <div class="stat-number">{{ statsStore.totalCount }}</div>
        <div class="stat-label">总计番茄</div>
      </div>
    </div>

    <h3 class="section-title">最近 14 天</h3>

    <div class="bar-chart" v-if="recentDays.length > 0">
      <div
        v-for="[date, count] in recentDays"
        :key="date"
        class="bar-column"
        :class="{ today: date === todayKey }"
      >
        <div class="bar-count">{{ count }}</div>
        <div
          class="bar"
          :style="{ height: barHeight(count) + 'px' }"
        ></div>
        <div class="bar-label">{{ formatDate(date) }}</div>
      </div>
    </div>

    <div v-else class="empty-state">
      <p>还没有统计数据</p>
      <p class="hint">完成番茄钟后这里会显示统计</p>
    </div>
  </div>
</template>

<style scoped>
.stats-panel {
  padding: 8px;
}

.stats-summary {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
  margin-bottom: 24px;
}

.stat-card {
  padding: 16px;
  border-radius: 12px;
  background: rgba(255, 255, 255, 0.03);
  border: 1px solid rgba(255, 255, 255, 0.06);
  text-align: center;
}

.stat-number {
  font-size: 32px;
  font-weight: 700;
  background: linear-gradient(135deg, #818cf8, #c084fc);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.stat-label {
  font-size: 12px;
  color: #6b7280;
  margin-top: 4px;
}

.section-title {
  font-size: 14px;
  font-weight: 600;
  color: #9ca3af;
  margin-bottom: 12px;
}

.bar-chart {
  display: flex;
  align-items: flex-end;
  gap: 8px;
  height: 120px;
  padding: 8px 0;
}

.bar-column {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
}

.bar-count {
  font-size: 11px;
  color: #9ca3af;
}

.bar {
  width: 100%;
  max-width: 24px;
  background: rgba(99, 102, 241, 0.4);
  border-radius: 4px 4px 0 0;
  min-height: 4px;
}

.bar-column.today .bar {
  background: linear-gradient(180deg, #818cf8, #6366f1);
}

.bar-label {
  font-size: 10px;
  color: #6b7280;
}

.empty-state {
  text-align: center;
  padding: 32px 0;
  color: #6b7280;
  font-size: 13px;
}

.hint {
  font-size: 12px;
  margin-top: 4px;
  color: #4b5563;
}
</style>
