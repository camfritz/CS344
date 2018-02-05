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
			lstat(argv[fileArgumentCount + 1], &fileInfo);
			if(!(S_ISREG(fileInfo.st_mode))) {
				fprintf(stderr, "Not a regular file, skipping...\n");
				--fileArgumentCount;
			}
			else {
			//print file name
				fprintf(stdout, "%s\n", argv[fileArgumentCount + 1]);
				fprintf(stdout, "%c", ' ');
			//print file stats
				fwrite(&fileInfo, sizeof(struct stat), 1, stdout);
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
		char fileName[1000];
		char *fileContents;
		struct stat temp;
		FILE* filep;

		while(scanf("%s ", fileName) > 0) {
			if(!(filep = fopen(fileName, "w+b"))) {
				perror("ERROR: ");
				exit(1);
			}
			printf("%s\n", fileName);
			fread(&temp, sizeof(struct stat), 1, stdin);
			printf("THE SIZE OF THE FILE IS: %lld\n", temp.st_size);

			fileContents = (char *) malloc(temp.st_size * sizeof(char));
			fread(fileContents, temp.st_size, 1, stdin);
			fwrite(fileContents, temp.st_size, 1, filep);
			fclose(filep);
			free(fileContents);
		}
		// struct stat temp;
		// int scanLoop = 3, readInCounter = 0;
		// char *readIn;

		// //obtain size of standard input
		// filep = freopen(NULL, "rb", stdin);
		// fseek(filep, 0, SEEK_END);
		// fileSize = ftell(filep);
		// rewind(filep);

		// //allocate memory for standard input
		// fileBuffer = (char *) malloc(fileSize * sizeof(char));
		// readIn = (char *) malloc(fileSize * sizeof(char));
		// if(fileBuffer == NULL) {
		// 	perror("ERROR: ");
		// 	exit(1);
		// }

		// //copy standard input into buffer
		// size_t copyCheck = fread(fileBuffer, 1, fileSize, filep);
		// if(copyCheck != fileSize) {
		// 	perror("ERROR: ");
		// 	exit(1);
		// }
		// fclose(filep);

		// //iterate through buffer

		// //get name, create file
		// int i;
		// for(i = 0; i < fileSize; i++) {
		// 	if(fileBuffer[i] != ' ') {
		// 		readIn[readInCounter] = fileBuffer[i];
		// 		++readInCounter;
		// 	}
		// 	else {
		// 		if(readIn[0] == '\0') {
		// 			break;
		// 		}

		// 		if(!(filep = fopen(readIn, "w+b"))) {
		// 			perror("ERROR: ");
		// 			exit(1);
		// 		}
		// 		readInCounter = 0;
		// 		readIn[readInCounter] = '\0';

		// 		for(int j = 0; j < sizeof(struct stat); j++) {
		// 			++i;
		// 			readIn[j] = fileBuffer[i];
		// 		}
		// 		memcpy(&temp, readIn, sizeof(struct stat));
		// 		readInCounter = 0;
		// 		readIn[readInCounter] = '\0';

		// 		++i;
		// 		for(int j = 0; j < temp.st_size; j++) {
		// 			++i;
		// 			readIn[j] = fileBuffer[i];
		// 			// fprintf(stderr, "%c\n", fileBuffer[j]);
		// 		}
		// 		fwrite(readIn, temp.st_size - 1, 1, filep);
		// 		readInCounter = 0;
		// 		readIn[readInCounter] = '\0';

		// 		fclose(filep);
		// 		//++i;
		// 		// if((fileSize - i) < sizeof(struct stat)) {
		// 		// 	break;
		// 		// }

		// 		//fprintf(stderr, "%lld\n", temp.st_size);
		// 		//fprintf(stderr, "%s\n", readIn);

		// 			// case 2:
		// 			// fread(&temp, sizeof(struct stat), 1, readIn);
		// 			// readInCounter = 0;
		// 			// readIn[readInCounter] = '\0';
		// 			// --scanLoop;
		// 			// break;

		// 			// case 1:
		// 			// fwrite(readIn, temp.st_size, 1, filep);
		// 			// readInCounter = 0;
		// 			// readIn[readInCounter] = '\0';
		// 			// --scanLoop;
		// 			// break;
		// 	}

		// }
		// //fwrite(fileBuffer, 1, fileSize, stdout);
		// free(fileBuffer);
	}


}