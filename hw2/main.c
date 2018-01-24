/*
=======================
THIS FILE IS FOR TESTING PURPOSES ONLY!!!!
=======================
*/


#include "my_malloc.h"

int main() {
	// void *p;
	// print_free_list();
	double *i = (double *) my_malloc(100 * sizeof(double));
	print_free_list();
	int *j = (int *) my_malloc(100 * sizeof(int));
	print_free_list();
	// my_free(p);
}