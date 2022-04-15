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
/*͸����ͼ*/
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
/*����������ʽ*/
void setTextStyle(int height, int width, const char* faceName) {
	LOGFONT font = { 0 };
	font.lfHeight = height;
	font.lfWidth = width;
	font.lfQuality = ANTIALIASED_QUALITY;
	strcpy(font.lfFaceName, faceName);
	settextstyle(&font);
}
/*����ͼƬ*/
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
/*��ȡϵͳʱ��*/
const char* week[7] = { "��","һ","��","��","��","��","��" };
const char* mon[12] = { "12","01","02","03","04","05","06","07","08","09","10","11" };
void gameDraw() {
	/*����ͼ�δ��ڱ�����ɫ*/
	setbkcolor(RGB(223, 230, 240));//�������õ�ǰ��ͼ����ɫ
	cleardevice();									//���õ�ǰ����ɫ�����Ļ��������ǰ������ (0, 0)��EASYX�Դ�
	setbkmode(TRANSPARENT);		//���ñ���ģʽΪ͸��
	/*����Բ�α���*/
	setlinecolor(RGB(68, 68, 68));		//���ñ߿���ɫ
	setlinestyle(PS_SOLID, 25);			//�߿���ʽ
	setfillcolor(RGB(223, 230, 240));	//Բ�����ɫ
	fillellipse(WIN_X1, WIN_Y1, WIN_X2, WIN_Y2);					//����Բ��
	/*��ȡϵͳʱ��*/
	time_t currentTime = time(NULL);
	struct tm* times = localtime(&currentTime);
	char buf[40] = { 0 };
	/*Сʱ�����ӡ���*/
	settextcolor(BLACK);
	setTextStyle(100, 40, "Arial");
	sprintf(buf, "%02d:%02d", times->tm_hour, times->tm_min);
	outtextxy(WIN_HALF - 134, WIN_HALF - 131, buf);
	setTextStyle(55, 23, "Arial");
	sprintf(buf, "%02d", times->tm_sec);
	outtextxy(WIN_HALF + 100, 160, buf);
	/*����*/
	setTextStyle(25, 12, "����");
	sprintf(buf, "��%s", week[times->tm_wday]);
	outtextxy(WIN_HALF + 110, WIN_HALF + 17, buf);
	setTextStyle(25, 12, "����");
	sprintf(buf, "%d��%s��%d��", times->tm_year + 1900, mon[times->tm_mon], times->tm_mday);
	outtextxy(WIN_HALF + 50, WIN_HALF + 80, buf);
	/*��������*/
	setlinecolor(BLACK);		//���ñ߿���ɫ
	setlinestyle(PS_SOLID, 5);
	/*��������*/
	line(WIN_HALF - 30, 12, WIN_HALF - 30, 130);
	line(WIN_HALF - 59, WIN_HALF + 230, WIN_HALF - 59, WIN_HALF + 180);
	line(WIN_HALF - 59, WIN_HALF + 180, WIN_HALF + 78, WIN_HALF + 180);
	line(WIN_HALF + 78, WIN_HALF + 180, WIN_HALF + 78, WIN_HALF + 124);
	/*���������ߺ�һ���̺���*/
	line(WIN_HALF - 205, WIN_HALF - 120, WIN_HALF + 205, WIN_HALF - 120);
	line(WIN_HALF - 205, WIN_HALF + 120, WIN_HALF + 205, WIN_HALF + 120);
	/*���*/
	setTextStyle(55, 23, "Arial");
	outtextxy(80, 80, "90%");
	/*̫��*/
	setTextStyle(25, 15, "����");
	outtextxy(WIN_HALF - 22, 35, "��������");
	setTextStyle(25, 12, "����");
	outtextxy(WIN_HALF - 22, 65, "����");
	outtextxy(WIN_HALF + 40, 65, "26��");
	outtextxy(WIN_HALF - 22, 95, "25��");
	outtextxy(WIN_HALF + 40, 95, "17��");
	/*����*/
	setTextStyle(25, 15, "����");
	outtextxy(WIN_HALF - 190, WIN_HALF + 35, "80~128");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 120, WIN_HALF + 65, "90");
	/*�������*/
	setTextStyle(25, 12, "����");
	outtextxy(WIN_HALF + 110, WIN_HALF - 17, "���³���");
	/*����*/
	setTextStyle(30, 15, "Arial");
	outtextxy(WIN_HALF - 135, WIN_HALF - 20, "1024");
	/*˯��*/
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 146, WIN_HALF + 130, "˯��");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 50, WIN_HALF + 130, "7h30m");
	/*����*/
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF - 50, WIN_SIZE - 60, "9.88km");
	setTextStyle(40, 15, "Arial");
	outtextxy(WIN_HALF + 90, WIN_HALF + 130, "����");
}
/*̫���˶���Ч��*/
void animation() {
	static DWORD t1 = clock();
	DWORD t2 = clock();
	static int  i = 0;
	putimage(WIN_HALF - 60, WIN_HALF - 50, spaceMan + i);
	//printf("ʱ��%d - %d = %d\n", t2, t1, t2 - t1);
	if (t2 - t1 > 20) {
		i = (i + 1) % 59;
		t1 = t2;
	}
	drawAlpha_s(WIN_HALF - 100, 30, others);//���
	drawAlpha_s(WIN_HALF + 70, 60, &others[1]);//�ϼ�ͷ
	drawAlpha_s(WIN_HALF + 70, 90, &others[2]);//�¼�ͷ
	drawAlpha_s(WIN_HALF + 105, 65, &others[3]);//̫��
	drawAlpha_s(WIN_HALF - 190, WIN_HALF + 55, &others[4]);//����
	drawAlpha_s(WIN_HALF - 190, WIN_HALF - 40, &others[5]);//����
}
/*��ʼ��ͼ�δ����Լ��޸Ĵ�����ʽ*/
void setWindowStyle() {
	initgraph(WIN_SIZE, WIN_SIZE);
	HWND hwnd = GetHWnd();//��ȡ���ھ��
	RECT rcClient, rcWind;
	GetClientRect(hwnd, &rcClient);
	GetWindowRect(hwnd, &rcWind);
	int cx = ((rcWind.right - rcWind.left) - rcClient.right) / 2;//��ȡ���ڱ߿��С
	int cy = GetSystemMetrics(SM_CYCAPTION) + cx;				
	HRGN rgn = CreateEllipticRgn(WIN_X1 + cx, WIN_Y1 + cy, WIN_X2 + cx, WIN_Y2 + cy);//CreateEllipticRgn->����Բ�δ���/CreateRectRgn->�������δ�����
	SetWindowRgn(hwnd, rgn, true);
}
/*�϶�����*/
void MOVE_WIN() {
	ExMessage m;
	m = getmessage(EM_MOUSE);
	HWND hwnd = GetHWnd();//��ȡ���ھ��
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