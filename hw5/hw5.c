#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	int numThreads;
	float contrastPercent;
	bool Invert = false;
	bool rotateRight = false;
	bool rotateLeft = false;
	bool keepRed = false;
	bool keepGreen = false;
	bool keepBlue = false;
	bool adjustContrast = false;

	int width, height, maxPixelValue, i, ***pixels;
	char *buffer[100];


	if(argc < 3 || argc > 4) {
		fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
		exit(1);
	}

	for(i = 1; i < argc; i++) {
		switch(i) {
			case 1:
			if(!(numThreads = atoi(argv[i]))) {
				fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
			}
			break;

			case 2:
			if(strcmp(argv[i], "-I") == 0) {
				Invert = true;
			}
			else if(strcmp(argv[i], "-R") == 0) {
				rotateRight = true;
			}
			else if(strcmp(argv[i], "-L") == 0) {
				rotateLeft = true;
			}
			else if(strcmp(argv[i], "-red") == 0) {
				keepRed = true;
			}
			else if(strcmp(argv[i], "-green") == 0) {
				keepGreen = true;
			}
			else if(strcmp(argv[i], "-blue") == 0) {
				keepBlue = true;
			}
			else if(strcmp(argv[i], "-C") == 0) {
				adjustContrast = true;
				if(argc != 4) {
					fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
					exit(1);
				}
			}
			else {
				fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
				exit(1);
			}
			break;

			case 3:
			if(!(contrastPercent = atof(argv[i]))) {
				fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
				exit(1);
			}
			if(contrastPercent < 0.0 || contrastPercent > 1.0) {
				fprintf(stderr, "usage: ./hw5 num_threads option [arg]\n");
				exit(1);
			}
			break;
		}
	}

	//int scanCounter = 0;
	//int r, c;
	// while(scanf("%s", buffer) != EOF) {
	// 	if(scanCounter == 1) {
	// 		width = atoi(buffer);
	// 		// fprintf(stderr, "WIDTH: %d\n", width);
	// 	}
	// 	else if(scanCounter == 2) {
	// 		height = atoi(buffer);
	// 		// fprintf(stderr, "HEIGHT: %d\n", height);
	// 		//allocate 3D pixel array
	// 		pixels = (int***) malloc(sizeof(int**) * height);
	// 		for(r = 0; r < height; r++) {
	// 			pixels[r] = (int**) malloc(sizeof(int*) * width);
	// 			for(c = 0; c < width; c++) {
	// 				pixels[r][c] = (int*) malloc(sizeof(int) * 3);
	// 			}
	// 		}
	// 	}
	// 	else if(scanCounter == 3) {
	// 		maxPixelValue = atoi(buffer);
	// 		r = 0;
	// 		c = 0;
	// 	}
	// 	else if(scanCounter > 3) {
	// 		if(scanCounter % 3 == 1) {
	// 			pixels[r][c][0] = atoi(buffer);
	// 		}
	// 		else if(scanCounter % 3 == 2) {
	// 			pixels[r][c][1] = atoi(buffer);
	// 		}
	// 		else if(scanCounter % 3 == 0) {
	// 			pixels[r][c][2] = atoi(buffer);
	// 			if(c < width) {
	// 				c += 1;
	// 			}
	// 			else {
	// 				if(r < height) {
	// 					r += 1;
	// 					c = 0;
	// 				}
	// 			}
	// 		}
	// 	}
	// 	++scanCounter;
	// }

	//get file width, height, max pixel values
	for(i = 0; i < 4; i++) {
		scanf("%s", buffer);
		if(i == 1) {
			width = atoi(buffer);
		}
		else if(i == 2) {
			height = atoi(buffer);
		}
		else if(i == 3){
			maxPixelValue = atoi(buffer);
		}
	}


//allocate 3D pixel array
	int r, c, p;
	pixels = (int***) malloc(sizeof(int**) * height);
	for(r = 0; r < height; r++) {
		pixels[r] = (int**) malloc(sizeof(int*) * width);
		for(c = 0; c < width; c++) {
			pixels[r][c] = (int*) malloc(sizeof(int) * 3);
		}
	}

//read in pixels to array
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			for(p = 0; p < 3; p++) {
				if(scanf("%s", buffer) != EOF) {
					pixels[r][c][p] = atoi(buffer);
				}
			}
		}
	}

	r = 0;
	c = 0;
	fprintf(stdout, "P3\n");
	fprintf(stdout, "%d %d\n", width, height);
	fprintf(stdout, "%d\n", maxPixelValue);
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			fprintf(stdout, "%d %d %d\n", pixels[r][c][0], pixels[r][c][1], pixels[r][c][2]);
		}
	}
}