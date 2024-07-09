<script setup lang="ts">
import { ref } from 'vue'
interface WebSocketInstanceType {
  send: (res: any) => void
  close: (res: any) => void
  onOpen: (res: any) => void
  onClose: (res: any) => void
  onError: (res: any) => void
  onMessage: (res: any) => void
}
interface CallFunctionResultType {
  result: any // 这里假设 result 是一个任意类型的对象，具体类型可以根据实际情况调整
}
interface HistoryTestListType {
  role: string
  content: string
}
interface MessageDataType {
  role?: string
  content?: string
  isShowLoading?: boolean
  isShowCopyIcon?: boolean
}
interface TextType {
  content: string
  role: string
  index: number
}
// 获取屏幕边界到安全区域距离
const { safeAreaInsets } = uni.getSystemInfoSync()
// 进入页面的默认提示
const tip = ref(
  '你好，我是你的人工智能大模型，现在的我能够学习和理解人类的语言，进行多轮对话，回答问题，高效帮助人们获取信息，知识和灵感~快和我聊聊吧！如果你不确定从哪里开始，可以试试这样问我:',
)
const problemData = ref([
  '给我一份关于数字经济的毕业论文大纲',
  '帮我推荐几个送给女朋友的生日礼物',
  '帮我推荐几款好用的国货口红',
  '我要在夏天去云南旅游，有什么美食推荐吗',
  '帮我推荐几款流行的家装风格',
])
declare const uniCloud: any
// 用户输入的问题
const text = ref('')
// 存储Socket挂载的方法
const SocketTask = ref<WebSocketInstanceType>()
// 存储用户发送的历史信息
const historyTestList = ref<HistoryTestListType[]>([])
// 存储AI和用户的历史数据
const messageData = ref<MessageDataType[]>([])
// AI回复的内容
const sparkResult = ref('')
const sendIngState = ref(false)
const sendMessage = async () => {
  if (text.value.trim().length > 0) {
    text.value = text.value.trim()
  } else {
    wx.showToast({ title: '请输入询问内容', icon: 'none' })
    return false
  }
  if (sendIngState.value) {
    wx.showToast({ title: '等待AI回复完毕', icon: 'none' })
    return false
  }
  // 用于页面展示
  messageData.value.push({
    role: 'user',
    content: text.value,
  })
  messageData.value.push({
    role: 'assistant',
    content: '',
    isShowLoading: true,
    isShowCopyIcon: false,
  })
  sparkResult.value = ''
  sendIngState.value = true
  // 连接
  let wssUrl = await uniCloud.callFunction({ name: 'chatgpt' })
  console.log(11112222, wssUrl)
  SocketTask.value = uni.connectSocket({
    url: wssUrl.result.url,
    success: (res) => {
      console.log(res, 'ws连接成功')
    },
    fail: (err) => {
      console.log(err, 'ws连接失败')
      wx.showToast({ title: '出现异常错误', icon: 'error' })
      sendIngState.value = false
    },
  })
  // 连接失败的回调
  SocketTask.value.onError((res: { errMsg: string }) => {
    console.log('监听到错误', res)
    wx.showToast({ title: '出现异常错误', icon: 'error' })
    sendIngState.value = false
  })
  // 连接成功会触发此方法
  SocketTask.value.onOpen((data: { [key: string]: string }) => {
    console.log('连接成功,接下来可以发送消息了', data)
    historyTestList.value.push({
      role: 'user',
      content: text.value,
    })
    text.value = ''
    // 讯飞星火大模型API需要传的值
    let params = {
      header: {
        app_id: wssUrl.result.APPID,
      },
      parameter: {
        chat: {
          domain: '4.0Ultra',
          temperature: 1,
        },
      },
      payload: {
        message: {
          text: historyTestList.value,
        },
      },
    }
    // 向讯飞星火大模型发送消息
    SocketTask!.value!.send({
      data: JSON.stringify(params),
      success: (res: { [key: string]: string }) => {
        console.log('发送消息成功', res)
      },
      fail: (err: { [key: string]: string }) => {
        console.log('发送消息失败', err)
        wx.showToast({ title: '出现异常错误', icon: 'error' })
        sendIngState.value = false
      },
    })
  }) //  调用接收消息的接口
  returnMessage()
}

const returnMessage = () => {
  SocketTask.value?.onMessage((res: { data: string }) => {
    messageData.value[messageData.value.length - 1].isShowLoading = false
    const obj = JSON.parse(res.data)
    // 出现错误
    if (obj.header.code !== 0) {
      sparkResult.value += obj.header.message.replace(/↵/g, '\n')
      messageData.value[messageData.value.length - 1].content = sparkResult.value
      sendIngState.value = false
      messageData.value[messageData.value.length - 1].isShowCopyIcon = true
      return false
    }
    const dataArray = obj.payload.choices.text
    dataArray.forEach((item: TextType) => {
      sparkResult.value += item.content.replace(/↵/g, '\n')
      messageData.value[messageData.value.length - 1].content = sparkResult.value
      autoScroll()
    })

    if (obj.header.code === 0 && obj.header.status === 2) {
      historyTestList.value.push({
        role: 'assistant',
        content: sparkResult.value,
      })
      // 打开复制按钮
      messageData.value[messageData.value.length - 1].isShowCopyIcon = true
      sendIngState.value = false
    }
  })
}
const scrollTop = ref(0)
const autoScroll = () => {
  let query = wx.createSelectorQuery()
  // 通过class选择器定位到scorll-view
  query.select('.scroll-view__content').boundingClientRect((res) => {
    scrollTop.value = res.height
  })
  query.exec((res) => {})
}

// 复制内容
const copyData = (val: string) => {
  wx.setClipboardData({ data: val })
}

// 清空
const clearMessage = () => {
  if (sendIngState.value) {
    wx.showToast({ title: '等待AI回复完毕', icon: 'none' })
    return false
  }
  sparkResult.value = ''
  historyTestList.value = []
  messageData.value = []
}
// 选择默认的发送
const selectText = (val: string) => {
  text.value = val
  sendMessage()
}
</script>

<template>
  <view class="viewport">
    <!-- 自定义导航栏 -->
    <view class="navbar-box" :style="{ paddingTop: safeAreaInsets!.top  + 'px' }">
      <view class="navbar-box__logo">
        <image class="navbar-box__image" src="/static/images/logo-a.png" mode="widthFix"></image>
      </view>
      <view class="navbar-box__description">
        <text class="navbar-box__title">AI助手</text>
        <text class="navbar-box__subtitle">你的智能助手,帮助你获取知识</text>
      </view>
    </view>

    <scroll-view
      :scroll-y="true"
      :scroll-top="scrollTop"
      :scroll-with-animation="true"
      class="scroll-view"
    >
      <view class="scroll-view__content">
        <!-- 进入页面的默认文本 -->
        <view v-if="messageData.length <= 0" class="tip-box box-animation">{{ tip }}</view>
        <view v-if="messageData.length <= 0" class="problem-box box-animation">
          <view class="problem-box__top">
            <image
              class="problem-box__image"
              src="/static/images/wenwo.png"
              mode="widthFix"
            ></image>
            <text class="problem-box__text">你可以这样问我</text>
          </view>
          <view
            class="problem-box__problems"
            v-for="(item, index) in problemData"
            :key="index"
            @tap="selectText(item)"
          >
            {{ item }}
          </view>
        </view>
        <!-- 用户和ai的对话区域 -->
        <template v-for="(item, index) in messageData" :key="index">
          <!-- 用户： -->
          <view class="user-box" v-if="item.role === 'user'">
            <view class="user-box__content">
              <view class="user-box__result"
                ><text user-select>
                  {{ item.content }}
                </text>
                <view class="user-box__copy">
                  <image
                    class="ai-box__image"
                    src="/static/images/fuzhi.png"
                    mode="widthFix"
                    @tap="copyData(item.content as string)"
                  ></image>
                </view>
              </view>
            </view>
            <view class="user-box__avatar">
              <image
                class="user-box__image"
                src="/static/images/avatar.png"
                mode="widthFix"
              ></image>
            </view>
          </view>
          <!-- AI： -->
          <view class="ai-box" v-if="item.role === 'assistant'">
            <!-- AI未回复时的显示loading -->
            <template v-if="item.isShowLoading">
              <view class="ai-box__loading">
                <view class="ai-box__loader"></view>
                <view class="ai-box__text">AI正在思考中...</view>
              </view>
            </template>

            <template v-else>
              <view class="ai-box__content">
                <view class="ai-box__avatar">
                  <image
                    class="ai-box__image"
                    src="/static/images/logo.png"
                    mode="widthFix"
                  ></image>
                </view>
                <view class="ai-box__result">
                  <text user-select>
                    {{ item.content }}
                  </text>
                  <view class="ai-box__copy">
                    <image
                      v-show="item.isShowCopyIcon"
                      class="ai-box__image"
                      src="/static/images/fuzhi.png"
                      mode="widthFix"
                      @tap="copyData(item.content as string)"
                    ></image>
                  </view>
                </view>
              </view>
            </template>
          </view>
        </template>
      </view>
    </scroll-view>

    <view class="bottom-box" :style="{ paddingBottom: safeAreaInsets!.bottom  + 'px' }">
      <image
        class="bottom-box__image"
        src="/static/images/qingkong.png"
        mode="aspectFit"
        @tap="clearMessage"
      ></image>
      <input
        class="bottom-box__input"
        placeholder="你可以问题任何问题"
        maxlength="-1"
        cursor-spacing="40"
        confirm-type="send"
        auto-blur
        @confirm="sendMessage"
        v-model="text"
        :disabled="sendIngState"
      />
      <image
        class="bottom-box__image"
        src="/static/images/fasong.png"
        @tap="sendMessage"
        mode="aspectFit"
      ></image>
    </view>
  </view>
</template>

<style lang="scss">
page {
  height: 100%;
  overflow: hidden;
  background-color: #f6f8fe;
}
/* 没有消息时的两段文本提示动画 */
@keyframes fadeInFromTop {
  0% {
    opacity: 0;
    transform: translateY(30px);
  }
  100% {
    opacity: 1;
    transform: translateY(0);
  }
}
.box-animation {
  animation-name: fadeInFromTop;
  animation-duration: 0.7s;
  animation-timing-function: ease-in;
  animation-fill-mode: forwards;
}
.viewport {
  height: 100%;
  display: flex;
  flex-direction: column;
}
.scroll-view {
  flex: 1;
  overflow: hidden;
  padding-bottom: 10rpx;
}

/* ai未回复时的loading加载 */
@keyframes spin {
  0% {
    transform: rotate(0deg);
  }
  100% {
    transform: rotate(360deg);
  }
}

/* 自定义导航栏样式 */
.navbar-box {
  display: flex;
  align-items: center;
  background: linear-gradient(to bottom, #b3cffa, #f6f8fe);
  padding-bottom: 10rpx;
  &__logo {
    margin: 0 30rpx;
  }
  &__image {
    width: 80rpx;
  }
  &__description {
    display: flex;
    flex-direction: column;
  }
  &__title {
    font-size: 50rpx;
    font-weight: bold;
  }
  &__subtitle {
    font-size: 25rpx;
    color: #798189;
  }
  .scroll-view {
    flex: 1;
    overflow: hidden;
    padding-bottom: 40rpx;
  }
}

.tip-box {
  padding: 10rpx;
  margin: 20rpx;
  line-height: 1.5;
  border-radius: 10rpx;
  font-size: 29rpx;
  background-color: #fefefe;
  width: auto;
}
/* 没有消息时的第二段文本样式 */
.problem-box {
  padding: 10rpx;
  margin: 20rpx;
  line-height: 1.5;
  border-radius: 10rpx;
  font-size: 29rpx;
  background-color: #fefefe;
  width: auto;
  &__top {
    display: flex;
    align-items: center;
  }
  &__text {
    font-weight: bold;
  }
  &__image {
    width: 40rpx;
    margin-right: 10rpx;
  }
  &__problems {
    border: 1rpx solid #e8f0fc;
    border-radius: 40rpx;
    padding: 15rpx 0;
    text-align: center;
    margin: 20rpx 0;
    color: #3875f6;
    font-weight: bold;
  }
}
.user-box {
  color: #555d92;
  margin: 20rpx;
  display: flex;
  justify-content: flex-end;
  &__content {
    display: flex;
    justify-content: flex-end;
  }
  &__image {
    width: 40rpx;
    display: block;
    border-radius: 50%;
  }
  &__avatar {
    padding: 10rpx;
    box-sizing: border-box;
  }
  &__result {
    line-height: 1.5;
    background-color: #fefefe;
    border-radius: 10rpx;
    padding: 10rpx 10px 10rpx 20rpx;
    align-self: auto;
    box-sizing: border-box;
    position: relative;
    &::after {
      content: '';
      width: 30rpx;
      height: 30rpx;
      background-color: #fefefe;
      position: absolute;
      right: -3rpx;
      transform: rotate(45deg);
      top: 20rpx;
      z-index: -10;
    }
  }
  &__copy {
    border-top: 1px solid #f3f3f4;
    display: flex;
    justify-content: flex-end;
    padding-top: 10rpx;
    margin-top: 20rpx;
  }
}
.ai-box {
  margin: 20rpx;
  border-radius: 10rpx;
  font-size: 30rpx;
  &__loading {
    display: flex;
    align-items: center;
    border-radius: 10rpx;
    background-color: #fefefe;
    padding: 10rpx;
  }
  &__loader {
    border: 5rpx solid #f3f3f3;
    border-top: 5rpx solid #3498db;
    border-radius: 50%;
    width: 40rpx;
    height: 40rpx;
    animation: spin 1s linear infinite;
    margin-right: 10rpx;
  }
  &__content {
    display: flex;
    justify-content: flex-end;
  }
  &__result {
    line-height: 1.5;
    background-color: #fefefe;
    border-radius: 10rpx;
    padding: 10rpx 10px 10rpx 20rpx;
    align-self: auto;
    box-sizing: border-box;
    position: relative;
    flex: 1;
    &::after {
      content: '';
      width: 30rpx;
      height: 30rpx;
      background-color: #fefefe;
      position: absolute;
      left: -3rpx;
      transform: rotate(45deg);
      top: 20rpx;
      z-index: -10;
    }
  }
  &__image {
    width: 40rpx;
    display: block;
    border-radius: 50%;
  }
  &__avatar {
    // border: 1rpx solid red;
    padding: 10rpx;
    box-sizing: border-box;
  }
  &__copy {
    border-top: 1px solid #f3f3f4;
    display: flex;
    justify-content: flex-end;
    padding-top: 10rpx;
    margin-top: 20rpx;
  }
}
.bottom-box {
  height: 100rpx;
  display: flex;
  align-items: center;
  justify-content: space-around;
  border-top: 1px solid #eee;
  background-color: #f6f8fe;
  padding: 10rpx;
  &__image {
    height: 50rpx;
    width: 50rpx;
    display: block;
  }
  &__input {
    width: 100%;
    margin: 0 10rpx;
    height: 40rpx;
    background-color: #fff;
    border-radius: 10rpx;
    padding: 20rpx;
  }
}
</style>
