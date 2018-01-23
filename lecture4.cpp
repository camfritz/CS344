#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int main(int argc, char **argv) {
	char buff[100];

	if(argc != 3) {
		fprintf(stderr, "Command Line Arguments Error\n");

		int src_fileOpened = open(argv[1], O_RDONLY, S_IRUSR);
		int wr_fileOpened = open(argv[2], O_WRONLY, S_IWUSR | S_IRUSR);

		if(src_fileOpened == -1 || wr_fileOpened == -1) {
			perror("ERROR: ");
			exit(1);
		}

		if(read(src_fileOpened, (char*) buff, 100) == -1) {
			perror("ERROR: ");
			exit(1);
		}

		if(write(wr_fileOpened, (char*) buff, 100) == -1) {
			perror("ERROR: ");
			exit(1);
		}

		int clSrc = close(src_fileOpened);
		int clWr = close(wr_fileOpened);

		if(clSrc == -1 || clWr == -1) {
			perror("ERROR: ");
			exit(1);
		}
	}
}