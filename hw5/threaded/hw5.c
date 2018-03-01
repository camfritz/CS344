#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

int numThreads;
int r, c, p;
float contrastPercent;
bool Invert = false;
bool rotateRight = false;
bool rotateLeft = false;
bool keepRed = false;
bool keepGreen = false;
bool keepBlue = false;
bool adjustContrast = false;

int width, height, maxPixelValue, i, ***pixels, ***newImage, newHeight, newWidth;
char *buffer[100];

void InvertImage() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				pixels[r][c][i] = maxPixelValue - pixels[r][c][i];
			}
		}
	}
}

void transformRed() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = maxPixelValue - pixels[r][c][0];
			pixels[r][c][1] = 0;
			pixels[r][c][2] = 0;
		}
	}
}

void transformGreen() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = 0;
			pixels[r][c][1] = maxPixelValue - pixels[r][c][1];
			pixels[r][c][2] = 0;
		}
	}
}

void transformBlue() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = 0;
			pixels[r][c][1] = 0;
			pixels[r][c][2] = maxPixelValue - pixels[r][c][2];
		}
	}
}

void transformContrast() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				if(pixels[r][c][i] <= (maxPixelValue / 2)) {
					pixels[r][c][i] -= (maxPixelValue * contrastPercent);
				}
				else {
					pixels[r][c][i] += (maxPixelValue * contrastPercent);
				}
			}
		}
	}
}

void transformLeft() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				newImage[width - c - 1][r][i] = pixels[r][c][i];
			}
		}
	}
}

void transformRight() {
	for(r = 0; r < height; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				newImage[c][height - r - 1][i] = pixels[r][c][i];
			}
		}
	}
}

void outputImage() {
	if(rotateLeft == true || rotateRight == true) {
		for(r = 0; r < newHeight; r++) {
			for(c = 0; c < newWidth; c++) {
				fprintf(stdout, "%d %d %d\n", newImage[r][c][0], newImage[r][c][1], newImage[r][c][2]);
			}
		}
	}
	else {
		for(r = 0; r < height; r++) {
			for(c = 0; c < width; c++) {
				fprintf(stdout, "%d %d %d\n", pixels[r][c][0], pixels[r][c][1], pixels[r][c][2]);
			}
		}
	}
}

int main(int argc, char **argv) {

	if(argc < 3) {
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
				if(argc < 4) {
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

	//get file width, height, max pixel values
	for(i = 0; i < 4; i++) {
		scanf("%s", buffer);
		if(i == 1) {
			width = atoi(buffer);
		}
		else if(i == 2) {
			height = atoi(buffer);
		}
		else if(i == 3) {
			maxPixelValue = atoi(buffer);
		}
	}

//allocate 3D pixel array
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


//allocate new array for rotated image
	if(rotateLeft == true || rotateRight == true) {
		newHeight = width;
		newWidth = height;
		newImage = (int***) malloc(sizeof(int**) * newHeight);
		for(r = 0; r < newHeight; r++) {
			newImage[r] = (int**) malloc(sizeof(int*) * newWidth);
			for(c = 0; c < newWidth; c++) {
				newImage[r][c] = (int*) malloc(sizeof(int) * 3);
			}
		}
	}

	//output image header
	fprintf(stdout, "P3\n");
	if(rotateLeft == true || rotateRight == true) {
		fprintf(stdout, "%d %d\n", newWidth, newHeight);
	}
	else {
		fprintf(stdout, "%d %d\n", width, height);
	}
	fprintf(stdout, "%d\n", maxPixelValue);


	if(Invert == true) {
		InvertImage();
	}
	else if(keepRed == true) {
		transformRed();
	}
	else if(keepGreen == true) {
		transformGreen();
	}
	else if(keepBlue == true) {
		transformBlue();
	}
	else if(adjustContrast == true) {
		transformContrast();
	}
	else if(rotateLeft == true) {
		transformLeft();
	}
	else if(rotateRight == true) {
		transformRight();
	}

	outputImage();
}