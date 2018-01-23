#include "my_malloc.h"

free_list_node *head = NULL;

void *my_malloc(int size) {
	long *mem, *next_split = NULL;
	//point head to initial free list
	if(head == NULL) {
		head = (free_list_node *) sbrk(SIZE);
		head->size = SIZE - sizeof(free_list_node);
		head->next = NULL;
	}
	//Scan for blocks on the free list
	//begin with initial head node condition

	// if(head->size == SIZE - 16) {
	// 	mem = head + 17;
	// 	head->size = head->size - size;
	// 	printf("%d\n", head->size);
	// 	return mem;
	// }

	//update sizes code



	do {
		if(head->size == size) {
			mem = head + sizeof(free_list_node);
			head = head->next;
			return mem;
		}
		else if(head->size > size) {
			mem = head + sizeof(free_list_node);
			head = head->next;
			//update sizes code

		}
	} while(head->next != NULL);

}

void my_free(void *ptr) {

}

void print_free_list() {

}


