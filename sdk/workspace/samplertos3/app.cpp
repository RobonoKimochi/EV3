/**
 * RTOS Task Sample Program
 *
 * This program shows "Hello Task1" and "Hello Task 2" in rotation.
 * 2 Tasks are implemented. These 2 tasks are rotated by task priority.
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

// CYC_HANDLER1 defined in app.cfg
void cyc_handler1(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("Hello Task1!!", 0, 0, true);
}

// CYC_HANDLER2 defined in app.cfg
void cyc_handler2(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("Hello Task2!!", 0, 0, true);
}

// TASK defined in app.cfg
void user_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	char str[32] = "";
	int16_t n = 0;
	while (true) {
		snprintf(str, 32, "USER_TASK: %d", n++);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight());
		wait(1000);
	}
}
