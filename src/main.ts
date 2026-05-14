import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'
import { useSettingsStore } from './stores/settings'
import { useTaskStore } from './stores/tasks'
import { useStatsStore } from './stores/stats'
import { useTimerStore } from './stores/timer'
import { requestNotificationPermission } from './utils/notifications'

const app = createApp(App)
const pinia = createPinia()
app.use(pinia)

const settings = useSettingsStore()
settings.load()

const tasks = useTaskStore()
tasks.load()

const stats = useStatsStore()
stats.load()

const timer = useTimerStore()
timer.init()

requestNotificationPermission()

app.mount('#app')
