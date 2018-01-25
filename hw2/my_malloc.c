#include "my_malloc.h"

free_list_node *head = NULL;

void *my_malloc(int size) {
	printf("my_malloc: called with size: %d\n", size);
	//some error checking on sizes
	if(size < 0) {
		fprintf(stderr, "Usage error: Size must be non-negative\n");
		return NULL;
	}
	if(size > 2048) {
		fprintf(stderr, "Usage error: Size cannot be greater than SIZE constant 2048\n");
	}
	if(SIZE > 2048) {
		fprintf(stderr, "Header Error: SIZE constant greater than 2048\n");
		return NULL;
	}

	free_list_node *previous = NULL;
	free_list_node *current = NULL;
	free_list_node *newNode = NULL;
	void *mem;
	//point head to initial free list
	if(head == NULL) {
		printf("Head Node is NULL, expanding heap\n");
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
				printf("Scanning free list...space found in free list\n");
				return mem;
			}
			else {
				//handle head edge case
				mem = current + sizeof(free_list_node);
				current = current->next;
				head = current;
				printf("Scanning free list...space found in free list\n");
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
				printf("Scanning free list...space found in free list\n");
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
				printf("Scanning free list...space found in free list\n");
				return mem;
			}
		}
		previous = current;
		current = current->next;
	} while(current != NULL);

	//expand heap if no memory available for allocation
	printf("Scanning free list...no space in free list\n");
	printf("Expanding heap with sbrk(), recursively calling my_malloc()\n");
	newNode = (free_list_node *) sbrk(SIZE);
	current = newNode;
	current->size = SIZE - 16;
	current->next = NULL;
	previous->next = current;
	mem = (void *) my_malloc(size);
	return mem;

}

void my_free(void *ptr) {
	free_list_node *freeNode = (free_list_node *) ptr - sizeof(free_list_node);
	free_list_node *current = head;
	int freeSize = ((int*)freeNode)[0];

	fprintf(stderr, "my_free: called with 0x%x, size = %d\n", (unsigned int) ptr, freeSize);

	//find end of free list
	do {
		if(current->next == NULL) {
			break;
		}
		else {
			current = current->next;
		}
	} while(1);

	//append freed memory to free list
	current->next = freeNode;
	freeNode->next = NULL;
	printf("my_free: next = 0x%x\n", (unsigned int) freeNode->next);
}

void print_free_list() {
	int counter = 0;
	int nd, addr, sz, nx;
	free_list_node *current = head;

	printf("===============FREE LIST==============\n");
	printf("NODE # |   ADDRESS |   SIZE |   NEXT\n");

	do {
		if(current == NULL) {
			break;
		}
		nd = counter;
		addr = (long) current;
		sz = current->size;
		nx = (long) current->next;
		printf("%7d|  0x%x|%8d|    0x%x\n", nd, addr, sz, nx);

		++counter;
		if(current->next != NULL) {
			current = current->next;
		}
		else {
			break;
		}

	} while(1);
	printf("======================================\n");
}