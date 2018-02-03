#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv) {
	struct stat fileInfo;
	int fileArgumentCount, scanLoop;
	FILE* filep;
	char *fileBuffer, str[10000];
	long fileSize;
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
			if(!(S_ISREG(fileInfo.st_mode))) {
				fprintf(stderr, "Not a regular file, skipping...\n");
				--fileArgumentCount;
			}
			else {
			//print file name
				fwrite(argv[fileArgumentCount + 1], strlen(argv[fileArgumentCount + 1]), 1, stdout);
				fprintf(stdout, "%c", ' ');
			//print file stats
				fwrite(&fileInfo, sizeof(struct stat), 1, stdout);
				fprintf(stdout, "%c", ' ');
			//print file contents
				if(!(filep = fopen(argv[fileArgumentCount + 1], "rb"))) {
					perror("ERROR: ");
					exit(1);
				}
				//fprintf(stdout, "*****BOF*****\n");
				// do {
				// 	fread(&c, sizeof(char), 1, filep);
				// 	//c = fgetc(filep);
				// 	fwrite(&c, sizeof(char), 1, stdout);
				// 	if(filep != NULL) {
				// 		++filep;
				// 	}
				// } while(c != EOF);

				//obtain size of file
				fseek(filep, 0, SEEK_END);
				fileSize = ftell(filep);
				rewind(filep);

				//allocate memory for file
				fileBuffer = (char *) malloc(fileSize * sizeof(char));
				if(fileBuffer == NULL) {
					perror("ERROR: ");
					exit(1);
				}

				//copy file into buffer
				size_t copyCheck = fread(fileBuffer, 1, fileSize, filep);
				if(copyCheck != fileSize) {
					perror("ERROR: ");
					exit(1);
				}

				fwrite(fileBuffer, 1, fileSize, stdout);

				fprintf(stdout, "%c", ' ');
				--fileArgumentCount;
				fclose(filep);
				free(fileBuffer);
			}
		}
	}


	else {
		scanLoop = 3;
		while(fscanf(stdin, "%s", str) != EOF) {
			switch(scanLoop) {
				case 3:
				filep = fopen(str, "w+b");
				--scanLoop;
				break;

				case 2:
				fread(&fileInfo, sizeof(struct stat), 1, stdin);
				printf("%lld\n", fileInfo.st_size);
				--scanLoop;
				break;

				case 1:
				fwrite(stdin, strlen(str), 1, filep);
				break;
			}
		}
		fclose(filep);
	}
}