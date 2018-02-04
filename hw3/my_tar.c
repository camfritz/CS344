#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv) {
	struct stat fileInfo;
	int fileArgumentCount;
	FILE* filep;
	char *fileBuffer;
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
		struct stat temp;
		int scanLoop = 3, readInCounter = 0;
		char *readIn;

		//obtain size of standard input
		filep = freopen(NULL, "rb", stdin);
		fseek(filep, 0, SEEK_END);
		fileSize = ftell(filep);
		rewind(filep);

		//allocate memory for standard input
		fileBuffer = (char *) malloc(fileSize * sizeof(char));
		readIn = (char *) malloc(fileSize * sizeof(char));
		if(fileBuffer == NULL) {
			perror("ERROR: ");
			exit(1);
		}

		//copy standard input into buffer
		size_t copyCheck = fread(fileBuffer, 1, fileSize, filep);
		if(copyCheck != fileSize) {
			perror("ERROR: ");
			exit(1);
		}
		fclose(filep);

		//iterate through buffer

		//get name, create file
		int i;
		for(i = 0; i < fileSize; i++) {
			if(fileBuffer[i] != ' ') {
				readIn[readInCounter] = fileBuffer[i];
				++readInCounter;
			}
			else {
				switch(scanLoop) {
					case 3:
					if(!(filep = fopen(readIn, "w+b"))) {
						perror("ERROR: ");
						exit(1);
					}
					readInCounter = 0;
					readIn[readInCounter] = '\0';
					--scanLoop;
					break;

					case 2:
					for(int j = 0; j < sizeof(struct stat); j++) {
						++i;
						readIn[readInCounter] = fileBuffer[i];
					}
					printf("%lu\n", sizeof(struct stat));
					readInCounter = 0;
					readIn[readInCounter] = '\0';
					--scanLoop;
					break;

					case 1:
					fwrite(readIn, strlen(readIn), 1, filep);
					readInCounter = 0;
					readIn[readInCounter] = '\0';
					--scanLoop;
					break;
				}

			}
		}
		fwrite(fileBuffer, 1, fileSize, stdout);
		fclose(filep);
		free(fileBuffer);

	}
}

/*test code
fread(&temp, sizeof(struct stat), 1, stdin);
*/

/*
	else {
		struct stat temp;
		scanLoop = 3;
		while(scanf("%s", str) != EOF) {
			switch(scanLoop) {
				case 3:
				filep = fopen(str, "w+b");
				//while ((getchar()) != '\n');
				--scanLoop;
				break;

				case 2:
				fread(&temp, sizeof(struct stat), 1, stdin);
				// fread(&fileInfo, sizeof(struct stat), 1, stdin);
				printf("******FILE SIZE IS******: %lld\n", temp.st_size);
				fwrite(&temp, sizeof(struct stat), 1, stdout);
				--scanLoop;
				break;

				case 1:
				//fwrite(stdin, strlen(str), 1, stdout);
				break;
			}
		}
		fclose(filep);
	}
	*/