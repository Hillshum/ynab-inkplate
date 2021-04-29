#include "Inkplate.h"
#include "Fonts/FreeSansBold24pt7b.h"

extern Inkplate display;

char text0_content[32] = "Fun Money";
int text0_cursor_x = 101;
int text0_cursor_y = 93;
const GFXfont *text0_font = &FreeSansBold24pt7b;

char text1_content[32] = "$13895499.23";
int text1_cursor_x = 480;
int text1_cursor_y = 168;
const GFXfont *text1_font = &FreeSansBold24pt7b;

char text2_content[32] = "Planned Quick Meals";
int text2_cursor_x = 29;
int text2_cursor_y = 371;
const GFXfont *text2_font = &FreeSansBold24pt7b;

char text3_content[32] = "$1242.2424";
int text3_cursor_x = 480;
int text3_cursor_y = 492;
const GFXfont *text3_font = &FreeSansBold24pt7b;

void mainDraw() {
    display.setFont(text0_font);
    display.setTextColor(0, 7);    display.setTextSize(1);    display.setCursor(text0_cursor_x, text0_cursor_y);
    display.print(text0_content);

    display.setFont(text1_font);
    display.setTextColor(0, 7);    display.setTextSize(2);    display.setCursor(text1_cursor_x, text1_cursor_y);
    display.print(text1_content);

    display.setFont(text2_font);
    display.setTextColor(0, 7);    display.setTextSize(1);    display.setCursor(text2_cursor_x, text2_cursor_y);
    display.print(text2_content);

    display.setFont(text3_font);
    display.setTextColor(0, 7);    display.setTextSize(2);    display.setCursor(text3_cursor_x, text3_cursor_y);
    display.print(text3_content);


}
