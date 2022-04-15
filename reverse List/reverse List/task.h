#include <stdio.h>
#include <stdlib.h>
//�ڵ�ṹ
typedef struct ListNode
{
		int data;
		struct ListNode* next;
}node;

//������������ͷ���
node* createList(){
	node* head = (node*)malloc(sizeof(node));
	head->data = NULL;
	head->next = NULL;
	return head;
}
//�����½ڵ�
node* createNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
//ͷ�巨����������
void createListByHead(node* head, int data) {
	node* pMove = head->next;
	node* newNode = createNode(data);
	head->next = newNode;
	newNode->next = pMove;
}
//��ӡ����
void printList(node* head) {
	node* pMove = head->next;
	if (pMove == NULL) {
		printf("�˱���������\n");
		return;
	}
	while (pMove != NULL) {
		if (pMove->next == NULL) {
			printf("%d", pMove->data);
		}
		else {
			printf("%d->", pMove->data);
		}
		pMove = pMove->next;
	}
}

//������ת������
node* reverseListIteration(node* head) {
	if (head->next == NULL||head->next->next==NULL) { ;return head; }
	node* pre = NULL;
	node* cur = head->next;
	node* next = NULL;
	while (cur != NULL) {
		if (cur->next == NULL) { head->next = cur; }
		next = cur->next;
		cur->next = pre;
		pre = cur;
		cur = next;
	}
	return head;
}
//�ݹ鷴ת������
node* reverseListRecursive(node* head,node* pMove) {
	if (pMove == NULL|| pMove->next ==NULL) {  return pMove; }
	node* LastNode = reverseListRecursive(head, pMove->next);
	if (pMove != head->next) {
		pMove->next->next = pMove;
		pMove->next = NULL;
		return LastNode;
	}
	else {
		pMove->next->next = pMove;
		pMove->next = NULL;
		head->next = LastNode;
		return  head;
	}
}



			





