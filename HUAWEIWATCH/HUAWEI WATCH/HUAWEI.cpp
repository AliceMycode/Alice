#define _CRT_SECURE_NO_WARNINGS
#define WIN_SIZE 500
#define WIN_HALF (WIN_SIZE/2)
#include <stdio.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <mmsystem.h>
#include <easyx.h>
#include "hpp.h"
#pragma comment(lib,"winmm.lib")
int main(void) {
	setWindowStyle();
	loadImg();
	BeginBatchDraw();//Ë«»º³å
	while (true) {
		gameDraw();
		animation();
		FlushBatchDraw();
	}
	return 0;
}
