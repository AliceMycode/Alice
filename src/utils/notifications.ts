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
