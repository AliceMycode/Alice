import { contextBridge } from 'electron'

contextBridge.exposeInMainWorld('electronAPI', {
  sendNotification: (_title: string, _body: string) => {},
  playSound: () => {}
})
