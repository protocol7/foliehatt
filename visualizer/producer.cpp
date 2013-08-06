#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include <algorithm>
#include <stdlib.h>

const int histogramWidth = 40;
float rawHistogram[histogramWidth];
float outHistogram[histogramWidth];
float sinI = 0;

int main() {
	for(int i=0; i < histogramWidth; i++) {
		rawHistogram[i] = outHistogram[i] = 0.5;
	}
	while(true) {
		for(int x=0; x < histogramWidth; x++) {
			float f = (float) rand() / (float) RAND_MAX;
			rawHistogram[x] = std::max(0.0d, std::min(1.0d, rawHistogram[x] + (1 - f * 2) * 0.01));
			printf("%f ", rawHistogram[x] * fabs((float)sin(sinI)));
		}
		printf("\n");
		sinI += 0.01;
		//sinI = ((int)(1000*sinI) % 3140)/1000.0f;
		usleep(10000);
	}
	return 0;
}
