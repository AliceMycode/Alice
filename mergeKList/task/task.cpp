#include <stdio.h>
#include <malloc.h>
#define NUM 6 //创建多少个链表
#define SIZE 5 //每个链表的长度
typedef struct ListNode {
	int data;
	struct ListNode* next;
}Node;
//合并两个升序链表
Node* mergeTwo(Node* L1, Node* L2) {
	if (L1->next == NULL) { return L2; }
	if (L2->next == NULL) { return L1; }
	if (L1->next->data > L2->next->data) {
		L2->next = mergeTwo(L1, L2->next);
		return L2;
	}
	else {
		L1->next = mergeTwo(L1->next, L2);
		return L1;
	}

}
Node** createList(int num) {
	Node** head;
	head = (Node**)malloc(sizeof(Node*) * num);
	for (int i = 0; i < num; i++) {
		head[i] = (Node*)malloc(sizeof(Node));
		head[i]->next = NULL;
	}
	return head;
}
int main(void) {
	int k = 1;
	Node** head = createList(NUM);
	for (int i = 0; i < NUM; i++) {
		Node* rear = head[i];
		for (int j = 0; j < SIZE; j++) {
			Node* pMoveNew = (Node*)malloc(sizeof(Node));
			pMoveNew->data = k;
			k += 2;
			rear->next = pMoveNew;
			rear = pMoveNew;
		}
		rear->next = NULL;
	}
	for (int k = 0; k < NUM; k++) {
		Node* P = head[k]->next;
		if (P == NULL) {
			printf("此链表暂无数据\n");
		}
		while (P!= NULL) {
			if (P->next != NULL) {
				printf("%d->", P->data);
				P = P->next;
			}
			else {
				printf("%d", P->data);
				P = P->next;
			}
		}
		printf("\n");
	}




	return 0;
}

