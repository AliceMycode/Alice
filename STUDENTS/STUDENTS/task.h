#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*学生结构*/
struct students {
	char name[20];
	int num;
	char sex[10];
	int age;
};
/*节点结构*/
struct Node {
	//int data;
	struct students data;
	struct Node* next;
};
/*创建链表*/
struct Node* createList() {
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	headNode->next = NULL;
	return headNode;
}
/*创建节点*/
struct Node* createNode(struct students data) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
/*头插法建立单链表*/
void insertByHead(struct Node* headNode, struct students data) {
	struct Node* newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
/*打印链表*/
void printList(struct Node* headNode) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL)
		printf("此系统暂无学生数据:\n");
	printf("\t姓名\t\t学号\t\t性别\t年龄\n\n");
	while (pMove != NULL) {
		printf("\t%s\t\t%d\t\t%s\t%d\n", pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		pMove = pMove->next;
		printf("\n");
	}
}
/*删除记录*/
void DeleteData(struct Node* headNode, char* name) {
	struct Node* pMoveFront = headNode->next;
	struct Node* pMoveAfter = headNode;
	if (pMoveFront == NULL) {
		printf("操作错误，此系统暂无相关记录\n");
	}
	while (strcmp(pMoveFront->data.name, name)) {
		pMoveAfter = pMoveFront;
		pMoveFront = pMoveFront->next;
	}
	pMoveAfter->next = pMoveFront->next;
	free(pMoveFront);
}
/*修改记录*/
void  ModifyRecord(struct Node* headNode, char* name, struct students data) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL) {
		printf("操作错误，此表暂无数据\n");
	}
	while (strcmp(pMove->data.name, name)) {
		pMove = pMove->next;
	}
	pMove->data = data;
}
/*查询记录*/
struct Node* searchData(struct Node* headNode, char* name) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL) {
		printf("操作错误，此表暂无数据\n");
	}
	while (strcmp(pMove->data.name, name)) {
		pMove = pMove->next;
	}
	return pMove;
}
void menu() {
	printf("\t\t\t\t*********************************************************\n");
	printf("\t\t\t\t*\t\t\t0、退出系统\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t1、录入信息\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t2、删除记录\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t3、修改记录\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t4、查询记录\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t5、浏览记录\t\t\t*\n");
	printf("\t\t\t\t*********************************************************\n");
}
struct Node* List = createList();
void keyDown() {
	int choice = 0;
	int paras = 0;
	char argu[5];
	char  para[10];
	struct students data;
	struct Node* pMove = NULL;
	scanf("%d", &choice);
	switch (choice)
	{
	case 0:
		printf("退出系统成功\n");
		exit(0);
		break;
	case 1:
		printf("请输入学生的姓名、学号、性别、年龄\n");
		scanf("%s%d%s%d", data.name, &data.num, data.sex, &data.age);
		insertByHead(List, data);
		break;
	case 2:
		printf("请输入要删除记录的学生姓名\n");
		scanf("%s", data.name);
		DeleteData(List, data.name);
		break;
	case 3:
		printf("请输入要修改记录的学生姓名\n\n");
		scanf("%s", data.name);
		pMove = searchData(List, data.name);
		printf("该记录信息为：姓名：%s、学号：%d、性别：%s、年龄：%d\n\n"
			, pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		printf("请输入要修改的字段比如(姓名、学号、性别、年龄)按回车确定：");
		scanf("%s", argu);
		if (!strcmp(argu, "姓名")) {
			printf("请输入新的名字:");
			scanf("%s", para);
			strcpy(pMove->data.name, para);
		}
		else if (!strcmp(argu, "学号")) {
			printf("请输入新的学号:");
			scanf("%d", &paras);
			pMove->data.num = paras;
		}
		else if (!strcmp(argu, "性别")) {
			printf("请输入新的性别:");
			scanf("%s", para);
			strcpy(pMove->data.sex, para);
		}
		else {
			printf("请输入新的年龄:");
			scanf("%d", &paras);
			pMove->data.age = paras;
		}
		break;
	case 4:
		printf("请输入要修改记录的学生姓名\n\n");
		scanf("%s", data.name);
		pMove = searchData(List, data.name);
		printf("该记录信息为：姓名：%s、学号：%d、性别：%s、年龄：%d\n\n"
			, pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		break;
	case 5:
		printf("该系统所有记录如下:\n\n");
		printList(List);
		break;
	default:
		printf("输入有误，请重新输入\n");
		break;
	}
}




