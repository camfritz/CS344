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
	for(int s = 0; s < 3; s++) {
		i[s] = s;
		printf("%lf\n", i[s]);
	}
	// my_free(p);
}