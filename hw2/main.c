/*
=======================
THIS FILE IS FOR TESTING PURPOSES ONLY!!!!
=======================
*/


#include "my_malloc.h"

int main() {
	print_free_list();
	double *i = (double *) my_malloc(100 * sizeof(double));
	print_free_list();
	int *j = (int *) my_malloc(100 * sizeof(int));
	print_free_list();
	char *k = (char *) my_malloc(1000 * sizeof(char));
	print_free_list();
	my_free(k);
	print_free_list();
	my_free(j);
	print_free_list();
	my_free(i);
	print_free_list();
}