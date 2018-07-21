/**
 * Bluetooth Send/Recv API Sample Program
 *
 * Echo back via bluetooth connection
 * Please send message with CR
 *
 * In ev3rt-beta6-2-release, maybe a module which loads app via bluetooth zmodem has a bug.
 * When an app runs after being transfered via bluetooth zmodem,
 * the app receives ?00 code (3bytes) at first, before receiving user expected datum.
 * But the app runs from SD card, it means not used bluetooth zmodem before running the app,
 * the app can receive user expected code without unknown code.
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "Bluetooth.h"
#include "LCD.h"
#include "Clock.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto bluetooth = new Bluetooth();
auto clock = new Clock();
auto lcd = new LCD();

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {
	lcd->drawString("main_task started.", 0, 0);
	int32_t fontHeight = lcd->getCurrentFontHeight();
	char recvBuf[32] = "";
	char msg[32] = "";
	while (true) {
		if ( bluetooth->isConnected()) {
			lcd->drawString("isConnected(): true", 0, fontHeight);
			uint16_t size = bluetooth->recv(recvBuf, 32);
			lcd->drawString(recvBuf, 0, fontHeight*2);
			snprintf(msg, 32, "recv size: %d", size);
			lcd->drawString(msg, 0, fontHeight*3);
			snprintf(msg, 32, "%s\n", recvBuf);
			bluetooth->send(msg);
		}
	}
}
