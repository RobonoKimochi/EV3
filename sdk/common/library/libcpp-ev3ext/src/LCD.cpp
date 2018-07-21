#include "LCD.h"

using namespace ev3api;

lcdfont_t LCD::currentFont = EV3_FONT_SMALL;

LCD::LCD() {
}

LCD::~LCD() {
}

ER LCD::getFontSize(lcdfont_t font, int32_t* width, int32_t* height) {
	return ev3_font_get_size(font, width, height);
}

ER LCD::setFont(lcdfont_t font) {
	ER result = ev3_lcd_set_font(font);
	if ( result == E_OK ) {
		currentFont = font;
	}
	return result;
}

int32_t LCD::getCurrentFontHeight() {
	int32_t width = 0;
	int32_t height = 0;
	ev3_font_get_size(currentFont, &width, &height);
	return height;
}

int32_t LCD::getCurrentFontWidth() {
	int32_t width = 0;
	int32_t height = 0;
	ev3_font_get_size(currentFont, &width, &height);
	return width;
}

ER LCD::drawImage(const image_t* image, int32_t x, int32_t y) {
	return ev3_lcd_draw_image(image, x, y);
}

ER LCD::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
	return ev3_lcd_draw_line(x0, y0, x1, y1);
}

ER LCD::drawString(const char* str, int32_t x, int32_t y, bool_t autoClear) {
	if ( autoClear == true ) {
		char blankStr[EV3_LCD_WIDTH+1] = "";
		for ( int i=0; i<EV3_LCD_WIDTH; i++ ) {
			blankStr[i] = ' ';
			blankStr[i+1] = '\0';
		}
		ev3_lcd_draw_string(blankStr, x, y);
	}
	return ev3_lcd_draw_string(str, x, y);
}

ER LCD::fillRect(int32_t x, int32_t y, int32_t w, int32_t h,
		lcdcolor_t color) {
	return ev3_lcd_fill_rect(x, y, w, h, color);
}

ER LCD::loadImage(const memfile_t* memfile, image_t* image) {
	return ev3_image_load(memfile, image);
}

ER LCD::freeImage(image_t* image) {
	return ev3_image_free(image);
}

uint16_t LCD::getHeight() {
	return EV3_LCD_HEIGHT;
}

uint16_t LCD::getWidth() {
	return EV3_LCD_WIDTH;
}

void LCD::clear() {
	fillRect(0, 0, getWidth(), getHeight(), EV3_LCD_WHITE);
}

