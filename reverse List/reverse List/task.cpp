#include <stdio.h>
#include "task.h"
int main(void) {
	node* List = createList();
	createListByHead(List, 4);
	createListByHead(List, 3);
	createListByHead(List, 2);
	createListByHead(List, 1);
	printf("原始单链表:"); printList(List);
	printf("\n");
	//迭代反转单链表
	node* List_iteration = reverseListIteration(List);
	printf("迭代反转单链表:"); printList(List_iteration);
	//递归反转单链表
	printf("\n");
	node* List_recursive = reverseListRecursive(List,List->next);
	printf("递归反转单链表:"); printList(List_recursive);





	return 0;
}