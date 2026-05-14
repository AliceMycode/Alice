import type { Task, Settings, DailyStats } from '@/types'

const KEYS = {
  settings: 'pomodoro-settings',
  tasks: 'pomodoro-tasks',
  stats: 'pomodoro-stats'
} as const

export function loadSettings(): Settings | null {
  const raw = localStorage.getItem(KEYS.settings)
  return raw ? JSON.parse(raw) : null
}

export function saveSettings(settings: Settings): void {
  localStorage.setItem(KEYS.settings, JSON.stringify(settings))
}

export function loadTasks(): Task[] {
  const raw = localStorage.getItem(KEYS.tasks)
  return raw ? JSON.parse(raw) : []
}

export function saveTasks(tasks: Task[]): void {
  localStorage.setItem(KEYS.tasks, JSON.stringify(tasks))
}

export function loadStats(): DailyStats {
  const raw = localStorage.getItem(KEYS.stats)
  return raw ? JSON.parse(raw) : {}
}

export function saveStats(stats: DailyStats): void {
  const cleaned = cleanOldStats(stats)
  localStorage.setItem(KEYS.stats, JSON.stringify(cleaned))
}

function cleanOldStats(stats: DailyStats): DailyStats {
  const cutoff = new Date()
  cutoff.setDate(cutoff.getDate() - 90)
  const cutoffKey = cutoff.toISOString().split('T')[0]

  const result: DailyStats = {}
  for (const [date, count] of Object.entries(stats)) {
    if (date >= cutoffKey) {
      result[date] = count
    }
  }
  return result
}

export function getTodayKey(): string {
  return new Date().toISOString().split('T')[0]
}
