/**
 * EV3ext API Sample Program
 *
 * 1. show current battery status (mA, mV)
 * 2. change color of led by 3 sec.
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "EV3ext.h"
#include "Clock.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto ev3 = new EV3ext();
auto clock = new Clock();

#include "LCD.h"
auto lcd = new LCD();

/**
 * 現在のバッテリ状態（mA, mV）をLCDに表示する
 * @param x LCD上の表示位置X座標
 * @param y LCD上の表示位置Y座標
 */
void showCurrentBatteryStatus(uint16_t x, uint16_t y) {
	int32_t currentmA = ev3->getCurrentmA();
	int32_t currentmV = ev3->getCurrentmV();
	char msg[32] = "";
	snprintf(msg, 32, "mA=%ld:mV=%ld", currentmA, currentmV);
	lcd->drawString(msg, x, y);
}

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {

	// 現在のカウント値を表示
	showCurrentBatteryStatus(0, 0);

	// 3secごとにLEDの色を変更
	ev3->setLEDColor(LED_OFF);
	clock->wait(3000);
	ev3->setLEDColor(LED_RED);
	clock->wait(3000);
	ev3->setLEDColor(LED_GREEN);
	clock->wait(3000);
	ev3->setLEDColor(LED_ORANGE);
	clock->wait(3000);
	ev3->setLEDColor(LED_OFF);
}
