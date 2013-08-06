#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

const int histogramWidth = 30;
float rawHistogram[histogramWidth];
float outHistogram[histogramWidth];
float sinI = 0;

int main() {
	for(int i=0; i < histogramWidth; i++) {
		rawHistogram[i] = outHistogram[i] = 0;
	}
	while(true) {
		for(int x=0; x < histogramWidth; x++) {
			float f = (float) rand() / (float) RAND_MAX;
			rawHistogram[x] = rawHistogram[x] + (1 - f * 2) * 0.01;
			printf("%f ", rawHistogram[x] * sin(sinI));
		}
		printf("\n");
		sinI += 0.01;
		usleep(100);
	}
	return 0;
}
