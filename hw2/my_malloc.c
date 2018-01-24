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

	current = head;

	//scan for blocks in free list
	do {
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
				newNode->size = current->size;
				current->size = size;
				previous->next = newNode;
				return mem;
			}
			else {
				mem = current + sizeof(free_list_node);
				newNode = current + sizeof(free_list_node) + size;
				current->size = current->size - size - sizeof(free_list_node);
				newNode->next = current->next;
				newNode->size = current->size;
				current->size = size;
				head = newNode;
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
	int counter = 0;
	long nd, addr, sz, nx;
	free_list_node *current = head;

	do {
		nd = counter;
		addr = current;
		sz = current->size;
		nx = current->next;
		printf("=================FREE LIST==============\n");
		printf("NODE # |   ADDRESS |   SIZE |   NEXT\n");
		printf("%7d|  0x%x|%8d|    0x%x\n", nd, addr, sz, nx);
		printf("========================================\n");

		++counter;
		if(current->next != NULL) {
			current = current->next;
		}

	} while(current->next != NULL);
}


