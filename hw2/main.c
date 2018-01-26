/*
=======================
THIS FILE IS FOR TESTING PURPOSES ONLY!!!!
=======================
*/


#include "my_malloc.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int r;
	int c;
	int *l = (int*) my_malloc(508 * sizeof(int));
	srand(time(NULL));
	for(int i = 0; i < 5000; i++) {
		r = rand() % 508;
		int *p = (int*) my_malloc(r * sizeof(int));
		c = rand() % 2;
		if(c == 1) {
			my_free(p);
		}
		print_free_list();
	}
}