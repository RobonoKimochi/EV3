/**
 * Bluetooth Send API Sample Program
 *
 * Send message via Bluetooth connection
 * On the client, terminal shows "btmsg: N" recieved
 * via COM port named "Serial Port Profile"
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
	int msgNum = 0;
	char msg[32] = "";
	while (true) {
		if ( bluetooth->isConnected()) {
			lcd->drawString("isConnected(): true", 0, fontHeight);
			snprintf(msg, 32, "btmsg: %d\n", msgNum++);
			bluetooth->send(msg);
			lcd->drawString(msg, 0, fontHeight*2);
		}
		clock->wait(1000);
	}
}
