#include <stdio.h>
#include "task.h"
int main(void) {
	node* List = createList();
	createListByHead(List, 4);
	createListByHead(List, 3);
	createListByHead(List, 2);
	createListByHead(List, 1);
	printf("ԭʼ������:"); printList(List);
	printf("\n");
	//������ת������
	node* List_iteration = reverseListIteration(List);
	printf("������ת������:"); printList(List_iteration);
	//�ݹ鷴ת������
	printf("\n");
	node* List_recursive = reverseListRecursive(List,List->next);
	printf("�ݹ鷴ת������:"); printList(List_recursive);





	return 0;
}