import { defineStore } from 'pinia'
import { ref } from 'vue'
import type { Settings } from '@/types'
import { loadSettings, saveSettings } from '@/utils/persistence'

const defaults: Settings = {
  focusDuration: 25,
  shortBreakDuration: 5,
  longBreakDuration: 15,
  longBreakInterval: 4
}

export const useSettingsStore = defineStore('settings', () => {
  const focusDuration = ref(defaults.focusDuration)
  const shortBreakDuration = ref(defaults.shortBreakDuration)
  const longBreakDuration = ref(defaults.longBreakDuration)
  const longBreakInterval = ref(defaults.longBreakInterval)

  function load() {
    const saved = loadSettings()
    if (saved) {
      focusDuration.value = saved.focusDuration
      shortBreakDuration.value = saved.shortBreakDuration
      longBreakDuration.value = saved.longBreakDuration
      longBreakInterval.value = saved.longBreakInterval
    }
  }

  function persist() {
    saveSettings({
      focusDuration: focusDuration.value,
      shortBreakDuration: shortBreakDuration.value,
      longBreakDuration: longBreakDuration.value,
      longBreakInterval: longBreakInterval.value
    })
  }

  function update(settings: Partial<Settings>) {
    if (settings.focusDuration !== undefined) focusDuration.value = settings.focusDuration
    if (settings.shortBreakDuration !== undefined) shortBreakDuration.value = settings.shortBreakDuration
    if (settings.longBreakDuration !== undefined) longBreakDuration.value = settings.longBreakDuration
    if (settings.longBreakInterval !== undefined) longBreakInterval.value = settings.longBreakInterval
    persist()
  }

  return { focusDuration, shortBreakDuration, longBreakDuration, longBreakInterval, load, update }
})
