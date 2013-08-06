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
	for(int i=0; i< screenWidth * screenHeight; i++) screen[i] = 0;
}
void draw(int x, int y, float color) {
	if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight) return;
	attron(COLOR_PAIR(1 + (int)(color*8.0f)));
	screen[x + y * screenWidth] = color;
	mvaddch(y, x, ' ');
}
float getColor(int x, int y) {
	if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight) return 0.0f;
	return screen[x + y * screenWidth];
}

void fadeAll() {
	for(int x=0; x < screenWidth; x++)
		for(int y=0; y < screenHeight; y++)
			draw(x, y, std::max(0.0f, getColor(x, y) - 0.002f));
}
void offsetAll() {
	float speed = 0.05f;
	for(int x=screenWidth; x >= 0; x--)
		for(int y=0; y < screenHeight -1; y++)
			draw(x, y, std::max(0.0f, 
				getColor(x, y) * (1 - speed) +
				getColor(x - 1, y + 1) * speed));
}

void blurAll() {
	for(int x=0; x < screenWidth; x++)
		for(int y=screenHeight; y >= 0; y--)
			draw(x, y, 
				(getColor(x, y) + 
				getColor(x + 1, y) +
				getColor(x - 1, y) +
				getColor(x, y + 1) +
				getColor(x, y - 1)) / 5.0f);
}

void drawHistogram(Histogram hist, int width, float color, 
	int xOffset = 0, int yOffset = 0) {
	for(int i=0; i < width; i++) {
		float v = std::max(0.0f, hist.data[i]);
		draw(i + xOffset, screenHeight - 1 - v*10 + yOffset, v);
	}
}

void fillHistogram(Histogram hist, int width, float color,
	int xOffset = 0, int yOffset = 0) {
	for(int i=0; i < width; i++) {
		for(int y=std::max(0.0f, hist.data[i] * 10); y > 0; y--)
			draw(i + xOffset, screenHeight - 1 - y + yOffset, color);
	}
}

typedef std::list<Histogram> PrevType;
PrevType prev;
int histI = 0;
void historyDraw(Histogram hist, int width) {
	int prevMax = 30;
	if(histI++ % 10 != 0) return;
	prev.push_front(hist);
	if (prev.size() > prevMax) prev.pop_back();
	int i=0;
	for(PrevType::iterator it = prev.begin(); it != prev.end(); it++) {
		drawHistogram(*it, width, ((float)i)/(float)prevMax, i, -i);
		i++;
	}
	fillHistogram(hist, width, 1);
}

int main() {
	//getmaxyx(&screenHeight, &screenWidth);
	initscr();
	start_color();
	noecho();
	//mousemask(ALL_MOUSE_EVENTS, NULL);
	screenHeight = LINES;
	screenWidth = COLS;
	initScreen();
	curs_set(0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_RED, COLOR_MAGENTA);
	init_pair(6, COLOR_RED, COLOR_YELLOW);
	init_pair(7, COLOR_RED, COLOR_WHITE);
	init_pair(8, COLOR_RED, COLOR_RED);
	init_pair(9, COLOR_RED, COLOR_BLACK);
	while(true) {
		blurAll();
		fadeAll();
		fillHistogram(getData(), histogramWidth, 1.0f, 10, -10);
		//historyDraw(getData(), histogramWidth);
		refresh();
	}
	endwin();
}
