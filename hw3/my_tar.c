#include <stdio.h>
#include <string.h>

void my_tar() {

}

int main(int argc, char **argv) {
	if((strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-x") != 0)) {
		fprintf(stderr, "Usage: ./my_tar -c file1 [file2 ...] or ./my_tar -x\n");
	}
}