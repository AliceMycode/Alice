#define _CRT_SECURE_NO_WARNINGS
#define WIN_SIZE 500
#define WIN_HALF (WIN_SIZE/2)
#define WIN_X1 0
#define WIN_Y1 0
#define WIN_X2 500
#define WIN_Y2 500
#include <stdio.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
/*透明贴图*/
void drawAlpha_s(int x, int y, IMAGE* srcImg) {
	DWORD* pwin = GetImageBuffer();
	DWORD* pimg = GetImageBuffer(srcImg);
	int win_w = getwidth();
	int win_h = getheight();
	int img_w = srcImg->getwidth();
	int img_h = srcImg->getwidth();
	int real_w = (x + img_w) > win_w ? win_w - x : img_w;
	int real_h = (y + img_h) > win_h ? win_h - y : img_h;
	if (x < 0) {
		pimg += abs(x);
		real_w -= abs(x);
		x = 0;
	}
	if (y < 0) {
		pimg += img_w * abs(y);
		real_h -= abs(y);
		y = 0;
	}
	pwin += win_w * y + x;
	for (int iy = 0; iy < real_h; iy++) {
		for (int ix = 0; ix < real_w; ix++) {
			UCHAR Alpha = (UCHAR)(pimg[ix] >> 24);
			if (Alpha > 120) {
				pwin[ix] = pimg[ix];
			}
		}
		pwin += win_w;
		pimg += img_w;
	}
}
/*设置字体样式*/
void setTextStyle(int height, int width, const char* faceName) {
	LOGFONT font = { 0 };
	font.lfHeight = height;
	font.lfWidth = width;
	font.lfQuality = ANTIALIASED_QUALITY;
	strcpy(font.lfFaceName, faceName);
	settextstyle(&font);
}
/*加载图片*/
IMAGE spaceMan[59];
IMAGE others[6];
void loadImg() {
	mciSendString("play images/music.mp3 repeat", NULL, NULL, NULL);
	char fileName[50] = { 0 };
	for (int i = 1; i < 60; i++) {
		sprintf(fileName, "./images/human (%d).jpg", i + 1);
		loadimage(spaceMan + (i - 1), fileName, 140, 130);
	}
	loadimage(others, "./images/other6.png", 70, 70);
	loadimage(&others[1], "./images/other5.png", 30, 30);
	loadimage(&others[2], "./images/other4.png", 30, 30);
	loadimage(&others[3], "./images/other2.png", 60, 60);
	loadimage(&others[4], "./images/other1.png", 60, 60);
	loadimage(&others[5], "./images/other3.png", 60, 60);
}
/*获取系统时间*/
const char* week[7] = { "日","一","二","三","四","五","六" };
const char* mon[12] = { "12","01","02","03","04","05","06","07","08","09","10","11" };
void gameDraw() {
	/*设置图形窗口背景颜色*/
	setbkcolor(RGB(223, 230, 240));//用于设置当前绘图背景色
	cleardevice();									//是用当前背景色清空屏幕，并将当前点移至 (0, 0)，EASYX自带
	setbkmode(TRANSPARENT);		//设置背景模式为透明
	/*绘制圆形表盘*/
	setlinecolor(RGB(68, 68, 68));		//设置边框颜色
	setlinestyle(PS_SOLID, 25);			//边框样式
	setfillcolor(RGB(223, 230, 240));	//圆形填充色
	fillellipse(WIN_X1, WIN_Y1, WIN_X2, WIN_Y2);					//绘制圆形
	/*获取系统时间*/
	time_t currentTime = time(NULL);
	struct tm* times = localtime(&currentTime);
	char buf[40] = { 0 };
	/*小时、分钟、秒*/
	settextcolor(BLACK);
	setTextStyle(100, 40, "Arial");
	sprintf(buf, "%02d:%02d", times->tm_hour, times->tm_min);
	outtextxy(WIN_HALF - 134, WIN_HALF - 131, buf);
	setTextStyle(55, 23, "Arial");
	sprintf(buf, "%02d", times->tm_sec);
	outtextxy(WIN_HALF + 100, 160, buf);
	/*日期*/
	setTextStyle(25, 12, "宋体");
	sprintf(buf, "周%s", week[times->tm_wday]);
	outtextxy(WIN_HALF + 110, WIN_HALF + 17, buf);
	setTextStyle(25, 12, "宋体");
	sprintf(buf, "%d年%s月%d日", times->tm_year + 1900, mon[times->tm_mon], times->tm_mday);
	outtextxy(WIN_HALF + 50, WIN_HALF + 80, buf);
	/*绘制线条*/
	setlinecolor(BLACK);		//设置边框颜色
	setlinestyle(PS_SOLID, 5);
	/*三条竖线*/
	line(WIN_HALF - 30, 12, WIN_HALF - 30, 130);
	line(WIN_HALF - 59, WIN_HALF + 230, WIN_HALF - 59, WIN_HALF + 180);
	line(WIN_HALF - 59, WIN_HALF + 180, WIN_HALF + 78, WIN_HALF + 180);
	line(WIN_HALF + 78, WIN_HALF + 180, WIN_HALF + 78, WIN_HALF + 124);
	/*两条长横线和一条短横线*/
	line(WIN_HALF - 205, WIN_HALF - 120, WIN_HALF + 205, WIN_HALF - 120);
	line(WIN_HALF - 205, WIN_HALF + 120, WIN_HALF + 205, WIN_HALF + 120);
	/*火箭*/
	setTextStyle(55, 23, "Arial");
	outtextxy(80, 80, "90%");
	/*太阳*/
	setTextStyle(25, 15, "黑体");
	outtextxy(WIN_HALF - 22, 35, "空气良好");
	setTextStyle(25, 12, "宋体");
	outtextxy(WIN_HALF - 22, 65, "晴天");
	outtextxy(WIN_HALF + 40, 65, "26°");
	outtextxy(WIN_HALF - 22, 95, "25℃");
	outtextxy(WIN_HALF + 40, 95, "17°");
	/*心率*/
	setTextStyle(25, 15, "宋体");
	outtextxy(WIN_HALF - 190, WIN_HALF + 35, "80~128");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 120, WIN_HALF + 65, "90");
	/*日期相关*/
	setTextStyle(25, 12, "宋体");
	outtextxy(WIN_HALF + 110, WIN_HALF - 17, "腊月初九");
	/*步数*/
	setTextStyle(30, 15, "Arial");
	outtextxy(WIN_HALF - 135, WIN_HALF - 20, "1024");
	/*睡眠*/
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 146, WIN_HALF + 130, "睡眠");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 50, WIN_HALF + 130, "7h30m");
	/*距离*/
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 50, WIN_SIZE - 60, "9.88km");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF + 90, WIN_HALF + 130, "距离");
}
/*太空人动画效果*/
void animation() {
	static DWORD t1 = clock();
	DWORD t2 = clock();
	static int  i = 0;
	putimage(WIN_HALF - 60, WIN_HALF - 50, spaceMan + i);
	//printf("时间差：%d - %d = %d\n", t2, t1, t2 - t1);
	if (t2 - t1 > 20) {
		i = (i + 1) % 59;
		t1 = t2;
	}
	drawAlpha_s(WIN_HALF - 100, 30, others);//火箭
	drawAlpha_s(WIN_HALF + 70, 60, &others[1]);//上箭头
	drawAlpha_s(WIN_HALF + 70, 90, &others[2]);//下箭头
	drawAlpha_s(WIN_HALF + 105, 65, &others[3]);//太阳
	drawAlpha_s(WIN_HALF - 190, WIN_HALF + 55, &others[4]);//心率
	drawAlpha_s(WIN_HALF - 190, WIN_HALF - 40, &others[5]);//步数
}
/*初始化图形窗口以及修改窗体样式*/
void setWindowStyle() {
	initgraph(WIN_SIZE, WIN_SIZE);
	HWND hwnd = GetHWnd();//获取窗口句柄
	RECT rcClient, rcWind;
	GetClientRect(hwnd, &rcClient);
	GetWindowRect(hwnd, &rcWind);
	int cx = ((rcWind.right - rcWind.left) - rcClient.right) / 2;//获取窗口边框大小
	int cy = GetSystemMetrics(SM_CYCAPTION) + cx;				
	HRGN rgn = CreateEllipticRgn(WIN_X1 + cx, WIN_Y1 + cy, WIN_X2 + cx, WIN_Y2 + cy);//CreateEllipticRgn->创建圆形窗口/CreateRectRgn->创建矩形窗口有
	SetWindowRgn(hwnd, rgn, true);
}
/*拖动窗口*/
void MOVE_WIN() {
	ExMessage m;
	m = getmessage(EM_MOUSE);
	HWND hwnd = GetHWnd();//获取窗口句柄
	switch (m.message)
	{
	case WM_LBUTTONDOWN:
		PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(m.x, m.y));
		gameDraw();
		animation();
		break;
	case WM_RBUTTONUP:
		closegraph();
	default:
		break;
	}
}