/*
=======================
THIS FILE IS FOR TESTING PURPOSES ONLY!!!!
=======================
*/


#include "my_malloc.h"

int main() {
	// void *p;
	// print_free_list();
	double *i = (double *) my_malloc(3 * sizeof(double));
	int *x = (int *) my_malloc(3 * sizeof(int));
	// my_free(p);
}