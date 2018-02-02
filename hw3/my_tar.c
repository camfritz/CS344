#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
// void my_tar(char option, int args) {
// 	int fileArgumentCount = args - 2;
// 	FILE* filep = (FILE*) malloc(fileArgumentCount * sizeof(FILE*));

// 	switch (option) {
// 		case 'c':
// 		printf("Executing with 'C'\n");
// 		while(fileArgumentCount > 0) {
// 			filep =
// 		}
// 		break;
// 		default:
// 		printf("Executing with 'X'\n");
// 		break;
// 	}
// }

int main(int argc, char **argv) {
	struct stat fileInfo;
	int fileArgumentCount;
	FILE* filep;
	char c;
	if(argc < 2) {
		fprintf(stderr, "Usage: ./my_tar -c file1 [file2 ...] or ./my_tar -x\n");
		exit(1);
	}
	else if((strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-x") != 0)) {
		fprintf(stderr, "Usage: ./my_tar -c file1 [file2 ...] or ./my_tar -x\n");
		exit(1);
	}

	if(strcmp(argv[1], "-c") == 0) {
		if(argc < 3) {
			fprintf(stderr, "Usage: ./my_tar -c file1 [file2 ...] or ./my_tar -x\n");
			exit(1);
		}
		fileArgumentCount = argc - 2;
		while(fileArgumentCount > 0) {
			//get file stats
			stat(argv[fileArgumentCount + 1], &fileInfo);
			//print file name
			fwrite(argv[fileArgumentCount + 1], strlen(argv[fileArgumentCount + 1]), 1, stdout);
			fprintf(stdout, " ");
			//print file stats
			fwrite(&fileInfo, sizeof(struct stat), 1, stdout);
			fprintf(stdout, " ");
			//print file contents
			if(!(filep = fopen(argv[fileArgumentCount + 1], "r"))) {
				perror("ERROR: ");
				exit(1);
			}
			do {
				c = fgetc(filep);
				fprintf(stdout, "%c", c);
			} while(c != EOF);
			fprintf(stdout, " ");
			--fileArgumentCount;
		}
		// my_tar('c', argc);
	}
	else {
		// my_tar('x', 0);
	}
}