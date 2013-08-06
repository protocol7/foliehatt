#include <ncurses.h>
#include <string>
#include <stdlib.h>
#include <algorithm>


int histogramWidth = 10;
//float data[] = { 0.1f, 0.2f, 0.3f, 0.5f, 0.3f, 0.2f, 0.3f, 0.6f, 0.3f, 0.1f };

float* getData() {
	float *data = new float[histogramWidth];
	char c;
	std::string word;
	int i=0;
	while((c = getchar()) != '\n' && i < histogramWidth) {
		if (c == ' ') {
			data[i] = atof(word.c_str());
			i++;
			word = "";
		} else {
			word += c;
		}
	}
	return data;
}

int ll = 0;
float *prev = NULL;
void visualizeHistogram(float *data, int width) {
	//clear();
	init_pair(1, COLOR_RED, COLOR_MAGENTA);
	attron(COLOR_PAIR(1));
	mvprintw(0, 0, "Work %d", ll++);
	if (prev)
		for(int i=0; i < width; i++) {
			mvaddch(LINES - 1 - std::max(0.0f, prev[i] * 10), i, ' ');
		}
	init_pair(2, COLOR_RED, COLOR_BLUE);
	attron(COLOR_PAIR(2));
	for(int i=0; i < width; i++) {
		mvaddch(LINES - 1 - std::max(0.0f, data[i] * 10), i, ' ');
	}
	prev = data;
	refresh();
}

int main() {
	initscr();
	start_color();	
	while(true) {
		visualizeHistogram(getData(), histogramWidth);
	}
	endwin();
}
