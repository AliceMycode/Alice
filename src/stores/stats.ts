import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { DailyStats } from '@/types'
import { loadStats, saveStats, getTodayKey } from '@/utils/persistence'

export const useStatsStore = defineStore('stats', () => {
  const stats = ref<DailyStats>({})

  const todayCount = computed(() => stats.value[getTodayKey()] ?? 0)

  const totalCount = computed(() =>
    Object.values(stats.value).reduce((sum, c) => sum + c, 0)
  )

  function load() {
    stats.value = loadStats()
  }

  function recordPomodoro() {
    const key = getTodayKey()
    stats.value[key] = (stats.value[key] ?? 0) + 1
    saveStats(stats.value)
  }

  return { stats, todayCount, totalCount, load, recordPomodoro }
})
