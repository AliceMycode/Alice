#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "task.h"
#include <graphics.h>
int main(void) {
	while (true) {
		menu();
		keyDown();
		system("pause");//�����������
		system("cls");//����
	}
}