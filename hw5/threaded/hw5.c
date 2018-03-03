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

int width, height, maxPixelValue, i, j, ***pixels, ***newImage, newHeight, newWidth;
char *buffer[100];

//struct for storing start and end rows for thread processing
typedef struct segment {
	int start;
	int end;
} segment;

segment *threadSegments;

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

void *InvertImage(void *rows) {
	struct segment *args = rows;
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				pixels[r][c][i] = maxPixelValue - pixels[r][c][i];
			}
		}
	}
	return NULL;
}

void *transformRed(void *rows) {
	struct segment *args = rows;
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = maxPixelValue - pixels[r][c][0];
			pixels[r][c][1] = 0;
			pixels[r][c][2] = 0;
		}
	}
}

void *transformGreen(void *rows) {
	struct segment *args = rows;
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = 0;
			pixels[r][c][1] = maxPixelValue - pixels[r][c][1];
			pixels[r][c][2] = 0;
		}
	}
}

void *transformBlue(void *rows) {
	struct segment *args = rows;
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			pixels[r][c][0] = 0;
			pixels[r][c][1] = 0;
			pixels[r][c][2] = maxPixelValue - pixels[r][c][2];
		}
	}
}

void *transformContrast(void *rows) {
	struct segment *args = rows;
	for(r = args->start; r < args->end; r++) {
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

void *transformLeft(void *rows) {
	struct segment *args = rows;
	fprintf(stderr, "START: %d, END: %d\n", args->start, args->end);
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				newImage[width - c - 1][r][i] = pixels[r][c][i];
			}
		}
	}
}

void *transformRight(void *rows) {
	struct segment *args = rows;
	fprintf(stderr, "START: %d, END: %d\n", args->start, args->end);
	for(r = args->start; r < args->end; r++) {
		for(c = 0; c < width; c++) {
			for(i = 0; i < 3; i++) {
				newImage[c][newWidth - r - 1][i] = pixels[r][c][i];
			}
		}
	}
}

void freeMem() {
	for(r = 0; r < height; r++) {
		free(pixels[r]);
		for(c = 0; c < width; c++) {
			free(pixels[r][c]);
		}
	}
	free(pixels);

	if(rotateRight == true || rotateLeft == true) {
		for(r = 0; r < newHeight; r++) {
			free(newImage[r]);
			for(c = 0; c < newWidth; c++) {
				free(newImage[r][c]);
			}
		}
		free(newImage);
	}
}

int main(int argc, char **argv) {
//error check command line arguments
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
//set flags for processing
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
//more cmd line error checking
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

	//allocate threads, determine processing segments
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);
	threadSegments = (segment *) malloc(sizeof(segment) * numThreads);
	int step, remaining;
	if(rotateLeft == true || rotateRight == true) {
		if(height % numThreads != 0) {
			step = floor(height / numThreads);
		}
		else {
			step = height / numThreads;
		}
		remaining = height;
	}
	else {
		if(height % numThreads != 0) {
			step = floor(height / numThreads);
		}
		else {
			step = height / numThreads;
		}
		remaining = height;
	}

	for(j = 0; j < numThreads; j++) {
		if(j == 0) {
			threadSegments[j].start = 0;
			if(remaining >= step) {
				remaining -= step;
				threadSegments[j].end += step;
			}
			else {
				threadSegments[j].end = remaining;
			}
		}
		else {
			threadSegments[j].start = threadSegments[j - 1].end;
			if(remaining >= step) {
				remaining -= step;
				threadSegments[j].end = threadSegments[j].start + step;
			}
			else {
				threadSegments[j].end = threadSegments[j].start + remaining;
			}
		}
	}


//perform threaded processing on image
	if(Invert == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, InvertImage, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(keepRed == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformRed, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(keepGreen == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformGreen, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(keepBlue == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformBlue, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(adjustContrast == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformContrast, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(rotateLeft == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformLeft, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	else if(rotateRight == true) {
		for(j = 0; j < numThreads; j++) {
			pthread_create(&threads[j], NULL, transformRight, (void *) &threadSegments[j]);
			pthread_join(threads[j], NULL);
		}
		outputImage();
	}
	freeMem();
	free(threadSegments);
	free(threads);
}