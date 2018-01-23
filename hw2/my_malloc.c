#include "my_malloc.h"

free_list_node *head = NULL;

void *my_malloc(int size) {
	free_list_node *previous = NULL;
	free_list_node *current = NULL;
	free_list_node *newNode = NULL;
	void *mem;
	//point head to initial free list
	if(head == NULL) {
		current = (free_list_node *) sbrk(SIZE);
		current->size = SIZE - sizeof(free_list_node);
		current->next = NULL;
		head = current;
	}
	//Scan for blocks on the free list
	//begin with initial head node condition

	// if(head->size == SIZE - 16) {
	// 	mem = head + 17;
	// 	head->size = head->size - size;
	// 	printf("%d\n", head->size);
	// 	return mem;
	// }

	// do {
	// 	printf("%s\n", "CALLING");
	// 	if(head->size == size) {
	// 		mem = head + sizeof(free_list_node);
	// 		head = head->next;
	// 		return mem;
	// 	}
	// 	else if(head->size > size) {
	// 		mem = head + sizeof(free_list_node);
	// 		head = head->next;
	// 		//update sizes code
	// 	}
	// 	head = head->next;
	// } while(head->next != NULL);

	do {
		printf("%s\n", "CALLING");
		if(current->size == size) {
			if(current != head) {
				mem = current + sizeof(free_list_node);
				current = current->next;
				previous->next = current;
				return mem;
			}
			else {
				//handle head edge case
				mem = current + sizeof(free_list_node);
				previous = current;
				current = current->next;
				head = current;
				return mem;
			}
		}
		else if(current->size > size) {
			if(current != head) {
				mem = current + sizeof(free_list_node);
				newNode = current + sizeof(free_list_node) + size;
				current->size = current->size - size - sizeof(free_list_node);
				newNode->next = current->next;
				previous->next = newNode;
				return mem;
			}
			else {
				mem = current + sizeof(free_list_node);
				newNode = current + sizeof(free_list_node) + size;
				current->size = current->size - size - sizeof(free_list_node);
				newNode->next = current->next;
				head = current;
				return mem;
			}
		}
		previous = current;
		current = current->next;
	} while(current->next != NULL);

}

void my_free(void *ptr) {

}

void print_free_list() {

}


