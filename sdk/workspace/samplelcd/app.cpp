/**
 * LCD API Sample Program
 *
 * 1. show LCD width/height
 * 2. show font size (EV3_FONT_SMALL)
 * 3. show font size (EV3_FONT_MEDIUM)
 * 4. show string by using EV3_FONT_MEDIUM
 * 5. show line
 * 6. show rectangle
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "LCD.h"
#include "Clock.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto clock = new Clock();
auto lcd = new LCD();

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {

	uint32_t positionY=0;

	// LCDにLCDのサイズを表示
	char msg[32] = "";
	snprintf(msg, 32, "W=%d:H=%d", lcd->getWidth(), lcd->getHeight());
	lcd->drawString(msg, 0, positionY, true);

	// LCDにフォント(FONT_SMALL)のサイズを表示
	int32_t fontSmallWidth = 0;
	int32_t fontSmallHeight = 0;
	lcd->getFontSize(EV3_FONT_SMALL, &fontSmallWidth, &fontSmallHeight);
	snprintf(msg, 32, "FONT_SMALL: W=%ld:H=%ld", fontSmallWidth, fontSmallHeight);
	positionY += fontSmallHeight;
	lcd->drawString(msg, 0, positionY, true);
	positionY += fontSmallHeight;

	// LCDにフォント(FONT_MEDIUM)のサイズを表示
	int32_t fontMediumWidth = 0;
	int32_t fontMediumHeight = 0;
	lcd->getFontSize(EV3_FONT_MEDIUM, &fontMediumWidth, &fontMediumHeight);
	snprintf(msg, 32, "FONT_MEDIUM: W=%ld:H=%ld", fontMediumWidth, fontMediumHeight);
	lcd->drawString(msg, 0, positionY, true);
	positionY += fontSmallHeight;

	// LCDに大き目のフォント(FONT_MEDIUM)で文字列を表示
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("FONT_MEDIUM", 0, positionY, true);
	positionY += fontMediumHeight;

	// 線を描画する
	const short lineWidth = 5;
	const short lineY0 = positionY;
	const short lineY1 = positionY + 10;
	positionY = lineY1;
	for (int x = 0; x < (lcd->getWidth()) - lineWidth; x = x + lineWidth) {
		if (x % 10 == 0) {
			lcd->drawLine(x, lineY0, x + lineWidth, lineY1);
		} else {
			lcd->drawLine(x, lineY1, x + lineWidth, lineY0);
		}
	}

	// 四角を描画する
	const short rectWidth = 5;
	const short rectY0 = positionY;
	const short rectHeight = 5;
	for (int x = 0; x < (lcd->getWidth()) - rectWidth; x = x + rectWidth) {
		if (x % 10 == 0) {
			lcd->fillRect(x, rectY0, rectWidth, rectHeight, EV3_LCD_BLACK);
		} else {
			lcd->fillRect(x, rectY0, rectWidth, rectHeight, EV3_LCD_WHITE);
		}
	}
}
