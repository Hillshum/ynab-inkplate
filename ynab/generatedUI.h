#include "Inkplate.h"
#include "Fonts/FreeSansBold24pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"

#define PROGRESS_WIDTH 670
#define PROGRESS_HEIGHT 20
#define PROGRESS_COLOR 6

extern Inkplate display;

char text0_content[32] = "";
int text0_cursor_x = 60;
int text0_cursor_y = 93;
const GFXfont *text0_font = &FreeSansBold24pt7b;

char text1_content[32] = "";
int text1_cursor_x = 410;
int text1_cursor_y = 193;
const GFXfont *text1_font = &FreeSansBold24pt7b;

int category_bar_x = 60;
int category_bar_y = 500;

char text2_content[32] = "";
int text2_cursor_x = 60;
int text2_cursor_y = 301;
const GFXfont *text2_font = &FreeSansBold24pt7b;

char text3_content[32] = "";
int text3_cursor_x = 410;
int text3_cursor_y = 401;
const GFXfont *text3_font = &FreeSansBold24pt7b;


char time_content[64] = "";
int time_cursor_x = 60;
int time_cursor_y = 570;
const GFXfont *time_font = &FreeSansBold12pt7b;


float month_progress;


void drawBar(int16_t x, int16_t y, float progress)
{
    int16_t solid_width = PROGRESS_WIDTH * progress;
    display.fillRect(x, y, solid_width, PROGRESS_HEIGHT, PROGRESS_COLOR);

    int16_t hollow_start = x + solid_width;
    int16_t hollow_width = PROGRESS_WIDTH - solid_width;
    display.drawRect(hollow_start, y, hollow_width, PROGRESS_HEIGHT, PROGRESS_COLOR);

}

void mainDraw() {
    display.setFont(text0_font);
    display.setTextSize(1);    display.setCursor(text0_cursor_x, text0_cursor_y);
    display.print(text0_content);

    display.setFont(text1_font);
    display.setTextSize(2);    display.setCursor(text1_cursor_x, text1_cursor_y);
    display.print(text1_content);

    display.setFont(text2_font);
    display.setTextSize(1);    display.setCursor(text2_cursor_x, text2_cursor_y);
    display.print(text2_content);

    display.setFont(text3_font);
    display.setTextSize(2);    display.setCursor(text3_cursor_x, text3_cursor_y);
    display.print(text3_content);

    display.setFont(time_font);
    display.setTextSize(1);    display.setCursor(time_cursor_x, time_cursor_y);
    display.print(time_content);

    drawBar(category_bar_x, category_bar_y, month_progress);


}
