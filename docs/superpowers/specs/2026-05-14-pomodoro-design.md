# 桌面番茄钟 — 设计方案

**日期**: 2026-05-14  
**技术栈**: Electron + Vue 3 + TypeScript  
**视觉风格**: 现代科技（暗色渐变 + 荧光点缀）

---

## 功能范围

- 基础番茄钟循环（专注 25min → 短休 5min）
- 每 4 轮专注后触发长休息（15min）
- 任务列表（添加、选择当前任务、标记完成）
- 统计记录（今日/历史完成番茄数）
- 所有时长可在设置面板自定义
- 阶段切换时系统通知 + 声音提示
- 窗口标题栏实时显示倒计时

## 窗口行为

标准窗口模式，可最小化到任务栏。

## 整体架构

```
electron-main/          # Electron 主进程
  main.ts               # 窗口创建、IPC、系统通知
  preload.ts            # 预加载脚本（contextBridge）
src/                    # Vue 3 渲染进程
  App.vue               # 根组件 + 布局骨架
  components/
    TimerDisplay.vue    # 圆形进度环 + 倒计时
    TimerControls.vue   # 开始/暂停/重置/跳过
    SessionBadge.vue    # 当前轮次指示（专注/短休/长休）
    TaskPanel.vue       # 侧边任务面板
    StatsPanel.vue      # 统计面板
    SettingsPanel.vue   # 设置面板
  stores/
    timer.ts            # 计时器状态（Pinia）
    tasks.ts            # 任务列表状态
    stats.ts            # 统计数据状态
    settings.ts         # 设置状态
  utils/
    notifications.ts    # 通知封装
    persistence.ts      # localStorage 读写
```

## 布局

左侧约 70%：主计时区（圆形进度环 + 控制按钮 + 轮次标识）  
右侧约 30%：可切换面板（任务 / 统计 / 设置三个 Tab）

## 计时逻辑

1. 专注阶段 → 自动切换至休息阶段 → 自动切回专注 → 循环
2. 每累积 4 轮专注后触发长休息（默认 15min）
3. 用户可随时暂停/继续/跳过当前阶段
4. 阶段切换时触发系统通知

## 数据存储

使用 `localStorage` 持久化：

| 数据 | 内容 |
|------|------|
| 设置 | 专注时长、短休时长、长休时长、长休间隔轮数 |
| 任务 | 任务列表（文本、完成状态、创建时间） |
| 统计 | 每日完成番茄数，保留最近 90 天 |

## 视觉风格

- 暗色背景（#0f0f23）
- 渐变计时数字（indigo → violet）
- 荧光色点缀（#6366f1）+ box-shadow 发光效果
- 圆角按钮、胶囊形标签
- 圆形进度环（SVG ring）
