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
  border-color: #14b8a6;
}

.add-btn {
  width: 32px;
  height: 32px;
  border: none;
  border-radius: 8px;
  background: #14b8a6;
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
  background: rgba(20, 184, 166, 0.1);
  border: 1px solid rgba(20, 184, 166, 0.25);
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
