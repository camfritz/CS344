#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <utime.h>

int main(int argc, char **argv) {
	struct stat fileInfo;
	int fileArgumentCount;
	FILE* filep;
	char *fileBuffer;
	long fileSize;
	//error check cmd line arguments
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
			if(access(argv[fileArgumentCount + 1], R_OK) != 0) {
				fprintf(stderr, "File does not have read permissions..skipping\n");
			}
			else {
			}
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

	else {
		char fileName[1000];
		char *fileContents;
		struct stat temp;
		struct utimbuf changeTimes;
		FILE* filep;

		//while file names still to be read, create file with file name, readin stats, write contents, change modes and access/mod times
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
			if(chmod(fileName, temp.st_mode) != 0) {
				perror("ERROR: ");
				exit(1);
			}

			changeTimes.actime = temp.st_atime;
			changeTimes.modtime = temp.st_mtime;
			if(utime(fileName, &changeTimes) != 0) {
				perror("ERROR: ");
				exit(1);
			}
			free(fileContents);
		}
		//free(fileContents);
	}

}