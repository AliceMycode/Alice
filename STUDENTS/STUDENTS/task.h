#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*ѧ���ṹ*/
struct students {
	char name[20];
	int num;
	char sex[10];
	int age;
};
/*�ڵ�ṹ*/
struct Node {
	//int data;
	struct students data;
	struct Node* next;
};
/*��������*/
struct Node* createList() {
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	headNode->next = NULL;
	return headNode;
}
/*�����ڵ�*/
struct Node* createNode(struct students data) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
/*ͷ�巨����������*/
void insertByHead(struct Node* headNode, struct students data) {
	struct Node* newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
/*��ӡ����*/
void printList(struct Node* headNode) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL)
		printf("��ϵͳ����ѧ������:\n");
	printf("\t����\t\tѧ��\t\t�Ա�\t����\n\n");
	while (pMove != NULL) {
		printf("\t%s\t\t%d\t\t%s\t%d\n", pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		pMove = pMove->next;
		printf("\n");
	}
}
/*ɾ����¼*/
void DeleteData(struct Node* headNode, char* name) {
	struct Node* pMoveFront = headNode->next;
	struct Node* pMoveAfter = headNode;
	if (pMoveFront == NULL) {
		printf("�������󣬴�ϵͳ������ؼ�¼\n");
	}
	while (strcmp(pMoveFront->data.name, name)) {
		pMoveAfter = pMoveFront;
		pMoveFront = pMoveFront->next;
	}
	pMoveAfter->next = pMoveFront->next;
	free(pMoveFront);
}
/*�޸ļ�¼*/
void  ModifyRecord(struct Node* headNode, char* name, struct students data) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL) {
		printf("�������󣬴˱���������\n");
	}
	while (strcmp(pMove->data.name, name)) {
		pMove = pMove->next;
	}
	pMove->data = data;
}
/*��ѯ��¼*/
struct Node* searchData(struct Node* headNode, char* name) {
	struct Node* pMove = headNode->next;
	if (pMove == NULL) {
		printf("�������󣬴˱���������\n");
	}
	while (strcmp(pMove->data.name, name)) {
		pMove = pMove->next;
	}
	return pMove;
}
void menu() {
	printf("\t\t\t\t*********************************************************\n");
	printf("\t\t\t\t*\t\t\t0���˳�ϵͳ\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t1��¼����Ϣ\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t2��ɾ����¼\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t3���޸ļ�¼\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t4����ѯ��¼\t\t\t*\n");
	printf("\n");
	printf("\t\t\t\t*\t\t\t5�������¼\t\t\t*\n");
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
		printf("�˳�ϵͳ�ɹ�\n");
		exit(0);
		break;
	case 1:
		printf("������ѧ����������ѧ�š��Ա�����\n");
		scanf("%s%d%s%d", data.name, &data.num, data.sex, &data.age);
		insertByHead(List, data);
		break;
	case 2:
		printf("������Ҫɾ����¼��ѧ������\n");
		scanf("%s", data.name);
		DeleteData(List, data.name);
		break;
	case 3:
		printf("������Ҫ�޸ļ�¼��ѧ������\n\n");
		scanf("%s", data.name);
		pMove = searchData(List, data.name);
		printf("�ü�¼��ϢΪ��������%s��ѧ�ţ�%d���Ա�%s�����䣺%d\n\n"
			, pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		printf("������Ҫ�޸ĵ��ֶα���(������ѧ�š��Ա�����)���س�ȷ����");
		scanf("%s", argu);
		if (!strcmp(argu, "����")) {
			printf("�������µ�����:");
			scanf("%s", para);
			strcpy(pMove->data.name, para);
		}
		else if (!strcmp(argu, "ѧ��")) {
			printf("�������µ�ѧ��:");
			scanf("%d", &paras);
			pMove->data.num = paras;
		}
		else if (!strcmp(argu, "�Ա�")) {
			printf("�������µ��Ա�:");
			scanf("%s", para);
			strcpy(pMove->data.sex, para);
		}
		else {
			printf("�������µ�����:");
			scanf("%d", &paras);
			pMove->data.age = paras;
		}
		break;
	case 4:
		printf("������Ҫ�޸ļ�¼��ѧ������\n\n");
		scanf("%s", data.name);
		pMove = searchData(List, data.name);
		printf("�ü�¼��ϢΪ��������%s��ѧ�ţ�%d���Ա�%s�����䣺%d\n\n"
			, pMove->data.name, pMove->data.num, pMove->data.sex, pMove->data.age);
		break;
	case 5:
		printf("��ϵͳ���м�¼����:\n\n");
		printList(List);
		break;
	default:
		printf("������������������\n");
		break;
	}
}




