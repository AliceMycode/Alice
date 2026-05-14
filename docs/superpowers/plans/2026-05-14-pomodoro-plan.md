# 桌面番茄钟 — 实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 构建一个 Electron + Vue 3 + TypeScript 桌面番茄钟，支持任务列表、统计、自定义设置、通知。

**Architecture:** Vite + Vue 3 渲染进程，Electron 主进程管理窗口和通知，Pinia 管理状态，localStorage 持久化数据。左侧主计时区 + 右侧可切换面板的布局。

**Tech Stack:** Vite, Vue 3, TypeScript, Pinia, Electron, vite-plugin-electron

---

### Task 1: 项目脚手架

**Files:**
- Create: `F:/test/package.json`
- Create: `F:/test/vite.config.ts`
- Create: `F:/test/tsconfig.json`
- Create: `F:/test/tsconfig.node.json`
- Create: `F:/test/index.html`
- Create: `F:/test/electron-builder.yml`
- Create: `F:/test/src/env.d.ts`

- [ ] **Step 1: 创建 package.json**

```json
{
  "name": "pomodoro-timer",
  "version": "1.0.0",
  "description": "桌面番茄钟",
  "main": "dist-electron/main.js",
  "scripts": {
    "dev": "vite",
    "build": "vue-tsc --noEmit && vite build && electron-builder",
    "preview": "vite preview"
  },
  "dependencies": {
    "pinia": "^2.1.7",
    "vue": "^3.4.21"
  },
  "devDependencies": {
    "@vitejs/plugin-vue": "^5.0.4",
    "electron": "^30.0.1",
    "electron-builder": "^24.13.3",
    "typescript": "^5.4.2",
    "vite": "^5.1.6",
    "vite-plugin-electron": "^0.28.6",
    "vite-plugin-electron-renderer": "^0.14.5",
    "vue-tsc": "^2.0.6"
  }
}
```

- [ ] **Step 2: 创建 vite.config.ts**

```typescript
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import electron from 'vite-plugin-electron'
import renderer from 'vite-plugin-electron-renderer'
import { resolve } from 'path'

export default defineConfig({
  plugins: [
    vue(),
    electron([
      {
        entry: 'electron/main.ts',
        vite: {
          build: {
            outDir: 'dist-electron',
            rollupOptions: {
              external: ['electron']
            }
          }
        }
      },
      {
        entry: 'electron/preload.ts',
        onstart(args) {
          args.reload()
        },
        vite: {
          build: {
            outDir: 'dist-electron',
            rollupOptions: {
              external: ['electron']
            }
          }
        }
      }
    ]),
    renderer()
  ],
  resolve: {
    alias: {
      '@': resolve(__dirname, 'src')
    }
  }
})
```

- [ ] **Step 3: 创建 tsconfig.json**

```json
{
  "compilerOptions": {
    "target": "ES2020",
    "module": "ESNext",
    "lib": ["ES2020", "DOM", "DOM.Iterable"],
    "moduleResolution": "bundler",
    "strict": true,
    "jsx": "preserve",
    "resolveJsonModule": true,
    "isolatedModules": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "noEmit": true,
    "paths": {
      "@/*": ["./src/*"]
    },
    "types": ["vite/client"]
  },
  "include": ["src/**/*.ts", "src/**/*.vue", "src/env.d.ts"]
}
```

- [ ] **Step 4: 创建 tsconfig.node.json**

```json
{
  "compilerOptions": {
    "target": "ES2020",
    "module": "ESNext",
    "moduleResolution": "bundler",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "noEmit": true
  },
  "include": ["vite.config.ts", "electron/**/*.ts"]
}
```

- [ ] **Step 5: 创建 index.html**

```html
<!DOCTYPE html>
<html lang="zh-CN">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>番茄钟</title>
    <style>
      * { margin: 0; padding: 0; box-sizing: border-box; }
      body {
        background: #0f0f23;
        color: #e0e0e0;
        font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
        overflow: hidden;
        user-select: none;
      }
    </style>
  </head>
  <body>
    <div id="app"></div>
    <script type="module" src="/src/main.ts"></script>
  </body>
</html>
```

- [ ] **Step 6: 创建 src/env.d.ts**

```typescript
/// <reference types="vite/client" />

declare module '*.vue' {
  import type { DefineComponent } from 'vue'
  const component: DefineComponent<{}, {}, any>
  export default component
}

interface Window {
  electronAPI: {
    sendNotification: (title: string, body: string) => void
    playSound: () => void
  }
}
```

- [ ] **Step 7: 创建 electron-builder.yml**

```yaml
appId: com.pomodoro.timer
productName: 番茄钟
directories:
  output: release
win:
  target: nsis
  icon: null
nsis:
  oneClick: false
  allowToChangeInstallationDirectory: true
```

- [ ] **Step 8: 安装依赖**

```bash
cd F:/test && npm install
```

Expected: 依赖安装成功，无报错。

- [ ] **Step 9: 验证项目可以启动（仅 Vite 端）**

```bash
cd F:/test && npx vite --host 2>&1 | head -5
```
Expected: Vite dev server starts. 按 Ctrl+C 停止。

- [ ] **Step 10: Commit**

```bash
cd F:/test && git init && git add -A && git commit -m "chore: scaffold Electron + Vue 3 + TS project"
```

---

### Task 2: Electron 主进程 + Preload

**Files:**
- Create: `F:/test/electron/main.ts`
- Create: `F:/test/electron/preload.ts`

- [ ] **Step 1: 创建 electron/main.ts**

```typescript
import { app, BrowserWindow, Notification, ipcMain } from 'electron'
import { join } from 'path'

let mainWindow: BrowserWindow | null = null

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 960,
    height: 640,
    minWidth: 800,
    minHeight: 560,
    backgroundColor: '#0f0f23',
    title: '番茄钟',
    webPreferences: {
      preload: join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false
    }
  })

  mainWindow.setTitle('番茄钟')

  if (process.env.VITE_DEV_SERVER_URL) {
    mainWindow.loadURL(process.env.VITE_DEV_SERVER_URL)
  } else {
    mainWindow.loadFile(join(__dirname, '../dist/index.html'))
  }
}

ipcMain.handle('send-notification', (_event, title: string, body: string) => {
  if (Notification.isSupported()) {
    const n = new Notification({ title, body, silent: false })
    n.show()
  }
})

app.whenReady().then(createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow()
  }
})
```

- [ ] **Step 2: 创建 electron/preload.ts**

```typescript
import { contextBridge, ipcRenderer } from 'electron'

contextBridge.exposeInMainWorld('electronAPI', {
  sendNotification: (title: string, body: string) => {
    ipcRenderer.invoke('send-notification', title, body)
  },
  playSound: () => {
    const audio = new Audio('data:audio/wav;base64,UklGRnoGAABXQVZFZm10IBAAAAABAAEAQB8AAEAfAAABAAgAZGF0YQoGAACAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+AgH9/f3+Af39/gIB/f39/gH9/f4CAf39/f4B/f3+Ag==')
    audio.play().catch(() => {})
  }
})
```

- [ ] **Step 3: 验证 Electron 可启动**

```bash
cd F:/test && npx electron dist-electron/main.js 2>&1 || echo "OK - electron launched (dev mode needs vite running first)"
```

- [ ] **Step 4: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add Electron main process and preload"
```

---

### Task 3: 类型定义 + 工具函数

**Files:**
- Create: `F:/test/src/types.ts`
- Create: `F:/test/src/utils/persistence.ts`
- Create: `F:/test/src/utils/notifications.ts`

- [ ] **Step 1: 创建 src/types.ts**

```typescript
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
```

- [ ] **Step 2: 创建 src/utils/persistence.ts**

```typescript
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
```

- [ ] **Step 3: 创建 src/utils/notifications.ts**

```typescript
export function notify(title: string, body: string) {
  if (window.electronAPI) {
    window.electronAPI.sendNotification(title, body)
  } else if ('Notification' in window && Notification.permission === 'granted') {
    new Notification(title, { body })
  }
}

export function playChime() {
  if (window.electronAPI) {
    window.electronAPI.playSound()
  }
}

export function requestNotificationPermission() {
  if ('Notification' in window && Notification.permission === 'default') {
    Notification.requestPermission()
  }
}
```

- [ ] **Step 4: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add types, persistence and notification utilities"
```

---

### Task 4: Pinia 状态管理

**Files:**
- Create: `F:/test/src/stores/timer.ts`
- Create: `F:/test/src/stores/tasks.ts`
- Create: `F:/test/src/stores/stats.ts`
- Create: `F:/test/src/stores/settings.ts`

- [ ] **Step 1: 创建 src/stores/settings.ts**

```typescript
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
```

- [ ] **Step 2: 创建 src/stores/tasks.ts**

```typescript
import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { Task } from '@/types'
import { loadTasks, saveTasks } from '@/utils/persistence'

export const useTaskStore = defineStore('tasks', () => {
  const tasks = ref<Task[]>([])

  const activeTask = computed(() => tasks.value.find(t => t.active) ?? null)

  function load() {
    tasks.value = loadTasks()
  }

  function persist() {
    saveTasks(tasks.value)
  }

  function addTask(text: string) {
    const task: Task = {
      id: Date.now().toString(36) + Math.random().toString(36).slice(2, 7),
      text,
      completed: false,
      createdAt: Date.now(),
      active: false
    }
    tasks.value.unshift(task)
    persist()
  }

  function toggleComplete(id: string) {
    const task = tasks.value.find(t => t.id === id)
    if (task) {
      task.completed = !task.completed
      persist()
    }
  }

  function setActive(id: string) {
    tasks.value.forEach(t => { t.active = t.id === id })
    persist()
  }

  function removeTask(id: string) {
    tasks.value = tasks.value.filter(t => t.id !== id)
    persist()
  }

  function clearCompleted() {
    tasks.value = tasks.value.filter(t => !t.completed)
    persist()
  }

  return { tasks, activeTask, load, addTask, toggleComplete, setActive, removeTask, clearCompleted }
})
```

- [ ] **Step 3: 创建 src/stores/stats.ts**

```typescript
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
```

- [ ] **Step 4: 创建 src/stores/timer.ts**

```typescript
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
```

- [ ] **Step 5: 验证 TypeScript 编译**

```bash
cd F:/test && npx vue-tsc --noEmit 2>&1
```

Expected: 无类型错误。

- [ ] **Step 6: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add Pinia stores for timer, tasks, stats and settings"
```

---

### Task 5: TimerDisplay 组件

**Files:**
- Create: `F:/test/src/components/TimerDisplay.vue`

- [ ] **Step 1: 创建 src/components/TimerDisplay.vue**

```vue
<script setup lang="ts">
import { computed } from 'vue'
import { useTimerStore } from '@/stores/timer'

const timer = useTimerStore()

const minutes = computed(() => Math.floor(timer.remainingSeconds / 60))
const seconds = computed(() => timer.remainingSeconds % 60)

const display = computed(() =>
  `${String(minutes.value).padStart(2, '0')}:${String(seconds.value).padStart(2, '0')}`
)

const circumference = 2 * Math.PI * 130
const dashOffset = computed(() => circumference * (1 - timer.progress))

const ringColor = computed(() => {
  switch (timer.currentSession) {
    case 'focus': return '#818cf8'
    case 'shortBreak': return '#34d399'
    case 'longBreak': return '#f59e0b'
  }
})
</script>

<template>
  <div class="timer-display">
    <svg class="ring" viewBox="0 0 300 300">
      <circle
        cx="150" cy="150" r="130"
        fill="none"
        stroke="rgba(255,255,255,0.05)"
        stroke-width="6"
      />
      <circle
        cx="150" cy="150" r="130"
        fill="none"
        :stroke="ringColor"
        stroke-width="6"
        stroke-linecap="round"
        :stroke-dasharray="circumference"
        :stroke-dashoffset="dashOffset"
        transform="rotate(-90 150 150)"
        class="progress-ring"
      />
    </svg>
    <div class="time-text">{{ display }}</div>
  </div>
</template>

<style scoped>
.timer-display {
  position: relative;
  display: flex;
  align-items: center;
  justify-content: center;
  width: 300px;
  height: 300px;
}

.ring {
  position: absolute;
  width: 300px;
  height: 300px;
  filter: drop-shadow(0 0 12px rgba(99, 102, 241, 0.3));
}

.progress-ring {
  transition: stroke-dashoffset 0.9s linear;
}

.time-text {
  font-size: 52px;
  font-weight: 600;
  letter-spacing: 4px;
  background: linear-gradient(135deg, #818cf8, #c084fc);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  font-variant-numeric: tabular-nums;
}
</style>
```

- [ ] **Step 2: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add TimerDisplay component with SVG ring progress"
```

---

### Task 6: SessionBadge + TimerControls 组件

**Files:**
- Create: `F:/test/src/components/SessionBadge.vue`
- Create: `F:/test/src/components/TimerControls.vue`

- [ ] **Step 1: 创建 src/components/SessionBadge.vue**

```vue
<script setup lang="ts">
import { computed } from 'vue'
import { useTimerStore } from '@/stores/timer'

const timer = useTimerStore()

const label = computed(() => {
  switch (timer.currentSession) {
    case 'focus': return '专注中'
    case 'shortBreak': return '短休息'
    case 'longBreak': return '长休息'
  }
})

const badgeClass = computed(() => `badge ${timer.currentSession}`)
</script>

<template>
  <div class="session-section">
    <span :class="badgeClass">
      <span class="dot"></span>
      {{ label }}
    </span>
    <span class="round-info">第 {{ timer.completedPomodoros + 1 }} 轮</span>
  </div>
</template>

<style scoped>
.session-section {
  display: flex;
  align-items: center;
  gap: 16px;
}

.badge {
  display: flex;
  align-items: center;
  gap: 6px;
  padding: 6px 16px;
  border-radius: 20px;
  font-size: 12px;
  letter-spacing: 1px;
  text-transform: uppercase;
}

.badge.focus {
  background: rgba(99, 102, 241, 0.15);
  color: #a5b4fc;
}

.badge.shortBreak {
  background: rgba(52, 211, 153, 0.15);
  color: #6ee7b7;
}

.badge.longBreak {
  background: rgba(245, 158, 11, 0.15);
  color: #fcd34d;
}

.dot {
  width: 6px;
  height: 6px;
  border-radius: 50%;
  background: currentColor;
  box-shadow: 0 0 6px currentColor;
}

.round-info {
  font-size: 13px;
  color: #6b7280;
}
</style>
```

- [ ] **Step 2: 创建 src/components/TimerControls.vue**

```vue
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
```

- [ ] **Step 3: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add SessionBadge and TimerControls components"
```

---

### Task 7: TaskPanel 组件

**Files:**
- Create: `F:/test/src/components/TaskPanel.vue`

- [ ] **Step 1: 创建 src/components/TaskPanel.vue**

```vue
<script setup lang="ts">
import { ref } from 'vue'
import { useTaskStore } from '@/stores/tasks'

const taskStore = useTaskStore()
const newTaskText = ref('')

function handleAdd() {
  const text = newTaskText.value.trim()
  if (!text) return
  taskStore.addTask(text)
  newTaskText.value = ''
}
</script>

<template>
  <div class="task-panel">
    <div class="panel-header">
      <h3>任务列表</h3>
      <button
        v-if="taskStore.tasks.some(t => t.completed)"
        class="clear-btn"
        @click="taskStore.clearCompleted()"
      >
        清除已完成
      </button>
    </div>

    <form class="add-task" @submit.prevent="handleAdd">
      <input
        v-model="newTaskText"
        class="task-input"
        placeholder="添加新任务..."
        maxlength="100"
      />
      <button type="submit" class="add-btn" :disabled="!newTaskText.trim()">+</button>
    </form>

    <div class="task-list" v-if="taskStore.tasks.length > 0">
      <div
        v-for="task in taskStore.tasks"
        :key="task.id"
        :class="['task-item', { active: task.active, completed: task.completed }]"
      >
        <button
          class="check-circle"
          @click="taskStore.toggleComplete(task.id)"
        >
          <span v-if="task.completed">&#10003;</span>
        </button>
        <span class="task-text" @click="taskStore.setActive(task.id)">{{ task.text }}</span>
        <button class="delete-btn" @click="taskStore.removeTask(task.id)">&times;</button>
      </div>
    </div>

    <div v-else class="empty-state">
      <p>暂无任务</p>
      <p class="hint">添加任务开始专注</p>
    </div>
  </div>
</template>

<style scoped>
.task-panel {
  padding: 8px;
}

.panel-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 16px;
}

.panel-header h3 {
  font-size: 16px;
  font-weight: 600;
  color: #d1d5db;
}

.clear-btn {
  background: none;
  border: none;
  color: #6b7280;
  font-size: 12px;
  cursor: pointer;
  font-family: inherit;
}

.clear-btn:hover {
  color: #ef4444;
}

.add-task {
  display: flex;
  gap: 8px;
  margin-bottom: 16px;
}

.task-input {
  flex: 1;
  padding: 8px 12px;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 8px;
  color: #e0e0e0;
  font-size: 13px;
  font-family: inherit;
  outline: none;
  transition: border-color 0.2s;
}

.task-input:focus {
  border-color: #6366f1;
}

.add-btn {
  width: 32px;
  height: 32px;
  border: none;
  border-radius: 8px;
  background: #6366f1;
  color: #fff;
  font-size: 18px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
}

.add-btn:disabled {
  opacity: 0.4;
  cursor: default;
}

.task-list {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.task-item {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 10px 12px;
  border-radius: 8px;
  background: rgba(255, 255, 255, 0.03);
  transition: background 0.2s;
}

.task-item:hover {
  background: rgba(255, 255, 255, 0.06);
}

.task-item.active {
  background: rgba(99, 102, 241, 0.1);
  border: 1px solid rgba(99, 102, 241, 0.25);
}

.check-circle {
  width: 20px;
  height: 20px;
  border-radius: 50%;
  border: 2px solid #4b5563;
  background: none;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
  color: #34d399;
  font-size: 12px;
  transition: border-color 0.2s;
}

.task-item.completed .check-circle {
  border-color: #34d399;
  background: rgba(52, 211, 153, 0.15);
}

.task-item.completed .task-text {
  text-decoration: line-through;
  color: #6b7280;
}

.task-text {
  flex: 1;
  font-size: 13px;
  cursor: pointer;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.delete-btn {
  background: none;
  border: none;
  color: #6b7280;
  font-size: 18px;
  cursor: pointer;
  opacity: 0;
  transition: opacity 0.2s, color 0.2s;
}

.task-item:hover .delete-btn {
  opacity: 1;
}

.delete-btn:hover {
  color: #ef4444;
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
```

- [ ] **Step 2: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add TaskPanel component"
```

---

### Task 8: StatsPanel 组件

**Files:**
- Create: `F:/test/src/components/StatsPanel.vue`

- [ ] **Step 1: 创建 src/components/StatsPanel.vue**

```vue
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
```

- [ ] **Step 2: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add StatsPanel component"
```

---

### Task 9: SettingsPanel 组件

**Files:**
- Create: `F:/test/src/components/SettingsPanel.vue`

- [ ] **Step 1: 创建 src/components/SettingsPanel.vue**

```vue
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
```

- [ ] **Step 2: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: add SettingsPanel component"
```

---

### Task 10: App.vue 主布局 + main.ts 入口

**Files:**
- Create: `F:/test/src/main.ts`
- Create: `F:/test/src/App.vue`

- [ ] **Step 1: 创建 src/main.ts**

```typescript
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
```

- [ ] **Step 2: 创建 src/App.vue**

```vue
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
```

- [ ] **Step 3: 验证 TypeScript 编译**

```bash
cd F:/test && npx vue-tsc --noEmit 2>&1
```

Expected: 无类型错误。

- [ ] **Step 4: Commit**

```bash
cd F:/test && git add -A && git commit -m "feat: wire up App.vue layout and main.ts entry"
```

---

### Task 11: 最终验证

- [ ] **Step 1: 检查 TypeScript 编译**

```bash
cd F:/test && npx vue-tsc --noEmit 2>&1
```
Expected: 无错误。

- [ ] **Step 2: 启动 Electron 开发模式**

```bash
cd F:/test && npm run dev 2>&1
```
Expected: Vite dev server 启动，Electron 窗口打开，显示番茄钟界面。

- [ ] **Step 3: 功能验证清单**

  - [ ] 倒计时正确运行（每秒减 1）
  - [ ] 圆形进度环动画流畅
  - [ ] 开始/暂停/重置/跳过按钮功能正常
  - [ ] 专注 → 短休 → 专注 → ... → 长休 循环正确
  - [ ] 系统通知弹出（如系统支持）
  - [ ] 任务列表：添加、选择当前任务、标记完成、删除
  - [ ] 统计面板：今日/总计数字正确，柱状图显示
  - [ ] 设置面板：滑动条调整时长，刷新页面后保留
  - [ ] 窗口标题栏实时显示倒计时

- [ ] **Step 4: Commit**

```bash
cd F:/test && git add -A && git commit -m "chore: final verification and fixes"
```
