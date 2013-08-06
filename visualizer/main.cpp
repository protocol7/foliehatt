#include <ncurses.h>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <list>

const int histogramWidth = 40;
struct Histogram {
	float data[histogramWidth];
};

//float data[] = { 0.1f, 0.2f, 0.3f, 0.5f, 0.3f, 0.2f, 0.3f, 0.6f, 0.3f, 0.1f };

Histogram getData() {
	Histogram hist;
	char c;
	std::string word;
	int i=0;
	while((c = getchar()) != '\n' && i < histogramWidth) {
		if (c == ' ') {
			hist.data[i] = atof(word.c_str());
			i++;
			word = "";
		} else {
			word += c;
		}
	}
	return hist;
}

int screenWidth, screenHeight;
float *screen;
void initScreen() {
	screen = new float[screenWidth * screenHeight];
	for(int i=0; i< screenWidth * screenHeight; i++) screen[i] = 1;
}
void draw(int x, int y, float color) {
	if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight) return;
	attron(COLOR_PAIR((int)color));
	screen[x + y * screenWidth] = color;
	mvaddch(y, x, ' ');
}
float getColor(int x, int y) {
	return screen[x + y * screenWidth];
}

void fadeAll() {
	for(int x=0; x < screenWidth; x++)
		for(int y=0; y < screenHeight; y++)
			draw(x, y, std::max(1.0f, getColor(x, y) - 0.02f));
}

void drawHistogram(Histogram hist, int width, int color) {
	for(int i=0; i < width; i++) {
		draw(i, screenHeight - 1 - std::max(0.0f, hist.data[i] * 10), color);
		//mvaddch(screenHeight - 1 - std::max(0.0f, hist.data[i] * 10), i, ' ');
	}
}

void visualizeHistogram(Histogram hist, int width) {
	//clear();
	fadeAll();
	drawHistogram(hist, width, 5);
	refresh();
}

int main() {
	//getmaxyx(&screenHeight, &screenWidth);
	initscr();
	start_color();
	screenHeight = LINES;
	screenWidth = COLS;
	initScreen();
	curs_set(0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_RED, COLOR_MAGENTA);
	while(true) {
		visualizeHistogram(getData(), histogramWidth);
	}
	endwin();
}
