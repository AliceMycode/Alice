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
