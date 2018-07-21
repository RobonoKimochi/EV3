/**
 * RTOS Data Queue Sample Program
 *
 * A task generates messages,
 * and another task receives and show a message on LCD.
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

void wait(uint32_t duration) {
	uint32_t base = clock->now();
	while ( base + duration > clock->now() );
	return;
}

void show_dtq() {
	lcd->setFont(EV3_FONT_MEDIUM);
	int32_t height = lcd->getCurrentFontHeight();
	int32_t width = lcd->getCurrentFontWidth();
	char str[32] = "";

	T_RDTQ refDtq;
	if ( ref_dtq(MSQ, &refDtq) == E_OK ) {
		int32_t n = refDtq.sdtqcnt;
		lcd->fillRect(0, height, lcd->getWidth(), height, EV3_LCD_WHITE);
		for ( int i=0; i<n; i++ ) {
			lcd->fillRect(i*width, height, width-1, height, EV3_LCD_BLACK);
		}
		snprintf(str, 32, "MSQ size: %ld", n);
		lcd->drawString(str, 0, height*2);
	}
	else {
		lcd->drawString("REF_DTQ not E_OK", 0, height*2);
	}
}

// GEN_TASK defined in app.cfg
uint16_t msgCount = 0;
void gen_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	char str[32] = "";
	while (true) {
		snprintf(str, 32, "Send: %d", msgCount);
		lcd->drawString(str, 0, 0);
		show_dtq();

		snd_dtq(MSQ, msgCount);
		msgCount++;
		wait(1000);
	}
}

// SHOW_TASK defined in app.cfg
void show_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	char str[32] = "";
	intptr_t msgCount = 0;

	while (true) {
		if ( rcv_dtq(MSQ, &msgCount) == E_OK ) {
			snprintf(str, 32, "Received: %d", msgCount);
			lcd->drawString(str, 0, lcd->getCurrentFontHeight()*3);
			show_dtq();
			wait(1000);
		}
	}
}
