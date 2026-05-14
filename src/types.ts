export type SessionType = 'focus' | 'shortBreak' | 'longBreak'

export interface TimerState {
  isRunning: boolean
  isPaused: boolean
  currentSession: SessionType
  remainingSeconds: number
  totalSeconds: number
  completedPomodoros: number
}

export interface Task {
  id: string
  text: string
  completed: boolean
  createdAt: number
  active: boolean
}

export interface Settings {
  focusDuration: number
  shortBreakDuration: number
  longBreakDuration: number
  longBreakInterval: number
}

export interface DailyStats {
  [date: string]: number
}
