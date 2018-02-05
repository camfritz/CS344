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
				fprintf(stdout, "%c", '\n');
			//print file stats
				fprintf(stdout, "%lld", fileInfo.st_size);
				fprintf(stdout, "%c", '\n');
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

				fprintf(stdout, "%c", '\n');
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
		int i, byteSz;
		for(i = 0; i < fileSize; i++) {
			if(fileBuffer[i] != '\n' && scanLoop != 1) {
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
					byteSz = atoi(readIn);
					readInCounter = 0;
					readIn[readInCounter] = '\0';
					--scanLoop;
					break;

					case 1:
					for(int j = 0; j < byteSz; j++) {
						readIn[j] = fileBuffer[i];
						//fprintf(stderr, "%c", fileBuffer[i]);
						++i;
					}
					fwrite(readIn, byteSz, 1, filep);
					readInCounter = 0;
					readIn[readInCounter] = '\0';
					fprintf(stderr, "THE FILE NAME IS: %s", readIn);
					scanLoop = 3;
					break;
				}

			}
		}
		//fwrite(fileBuffer, 1, fileSize, stdout);
		fclose(filep);
		free(fileBuffer);

	}
}