/**
 * RTOS Task Sample Program
 *
 * This program shows "Hello Task1" and "Hello Task 2" in rotation.
 * 2 Tasks are implemented. These 2 tasks are rotated by cyclic tasks.
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

// MAIN_TASK1 defined in app.cfg
void main_task1(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	while (true) {
		lcd->drawString("Hello Task1!!", 0, 0, true);
		clock->wait(1000);
	}
}

// MAIN_TASK2 defined in app.cfg
void main_task2(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	wait(500);
	while (true) {
		lcd->drawString("Hello Task2!!", 0, 0, true);
		wait(1000);
	}
}
