import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { SessionType } from '@/types'
import { useSettingsStore } from './settings'
import { useStatsStore } from './stats'
import { notify, playChime } from '@/utils/notifications'

export const useTimerStore = defineStore('timer', () => {
  const isRunning = ref(false)
  const isPaused = ref(false)
  const currentSession = ref<SessionType>('focus')
  const remainingSeconds = ref(0)
  const completedPomodoros = ref(0)
  let intervalId: ReturnType<typeof setInterval> | null = null

  const totalSeconds = computed(() => {
    const settings = useSettingsStore()
    switch (currentSession.value) {
      case 'focus': return settings.focusDuration * 60
      case 'shortBreak': return settings.shortBreakDuration * 60
      case 'longBreak': return settings.longBreakDuration * 60
    }
  })

  function init() {
    remainingSeconds.value = totalSeconds.value
  }

  const progress = computed(() => {
    if (totalSeconds.value === 0) return 0
    return 1 - remainingSeconds.value / totalSeconds.value
  })

  function start() {
    if (isRunning.value && !isPaused.value) return
    if (!isPaused.value) {
      init()
    }
    isRunning.value = true
    isPaused.value = false
    intervalId = setInterval(tick, 1000)
  }

  function pause() {
    isPaused.value = true
    if (intervalId) {
      clearInterval(intervalId)
      intervalId = null
    }
  }

  function reset() {
    pause()
    isRunning.value = false
    isPaused.value = false
    init()
  }

  function skip() {
    pause()
    isRunning.value = false
    isPaused.value = false
    switchToNext()
    init()
  }

  function tick() {
    if (remainingSeconds.value > 0) {
      remainingSeconds.value--
      updateTitle()
    } else {
      complete()
    }
  }

  function updateTitle() {
    const m = Math.floor(remainingSeconds.value / 60)
    const s = remainingSeconds.value % 60
    const label = currentSession.value === 'focus' ? '专注' : '休息'
    document.title = `${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')} - ${label}`
  }

  function complete() {
    pause()
    playChime()
    if (currentSession.value === 'focus') {
      completedPomodoros.value++
      useStatsStore().recordPomodoro()
      notify('番茄钟完成!', '休息一下吧 🎉')
    } else {
      notify('休息结束', '开始新的番茄钟吧!')
    }
    switchToNext()
    init()
  }

  function switchToNext() {
    const settings = useSettingsStore()
    if (currentSession.value === 'focus') {
      if (completedPomodoros.value > 0 && completedPomodoros.value % settings.longBreakInterval === 0) {
        currentSession.value = 'longBreak'
      } else {
        currentSession.value = 'shortBreak'
      }
    } else {
      currentSession.value = 'focus'
    }
  }

  function cleanup() {
    if (intervalId) {
      clearInterval(intervalId)
      intervalId = null
    }
  }

  return {
    isRunning, isPaused, currentSession, remainingSeconds, totalSeconds,
    completedPomodoros, progress, init, start, pause, reset, skip, cleanup
  }
})
