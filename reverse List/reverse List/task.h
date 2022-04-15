#include <stdio.h>
#include <stdlib.h>
//节点结构
typedef struct ListNode
{
		int data;
		struct ListNode* next;
}node;

//创建链表即创建头肩点
node* createList(){
	node* head = (node*)malloc(sizeof(node));
	head->data = NULL;
	head->next = NULL;
	return head;
}
//创建新节点
node* createNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
//头插法建立单链表
void createListByHead(node* head, int data) {
	node* pMove = head->next;
	node* newNode = createNode(data);
	head->next = newNode;
	newNode->next = pMove;
}
//打印链表
void printList(node* head) {
	node* pMove = head->next;
	if (pMove == NULL) {
		printf("此表暂无数据\n");
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

//迭代反转单链表
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
//递归反转单链表
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



			





