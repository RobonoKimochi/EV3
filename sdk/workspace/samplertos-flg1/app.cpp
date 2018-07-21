/**
 * RTOS Flag Sample Program
 * Play beep sound when the position of front lift is over or under, if touch sensor was not pressed.
 * The threshold of the over/under position is defined in app.h. And the counter of the lift is reset at the app starting.
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "LCD.h"
#include "Clock.h"
#include "Motor.h"
#include "TouchSensor.h"
#include "Speaker.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto clock = new Clock();
auto lcd = new LCD();
auto mFront = new Motor(PORT_A);
auto touch = new TouchSensor(PORT_1);
auto speaker = new Speaker();

// CYC_HANDLER defined in app.cfg
uint16_t cyc_handler_times = 0;
void cyc_handler(intptr_t unused) {
	char str[32] = "";
	snprintf(str, 32, "CYC_HANDLER: %d", cyc_handler_times++);
	lcd->drawString(str, 0, lcd->getCurrentFontHeight());

	// EV3のデバイスの状態をチェックしてフラグに反映
	FLGPTN flgptn = 0;

	// タッチセンサーの押下状態
	if (touch->isPressed()) {
		snprintf(str, 32, "TS: true");
		flgptn |= TS_ON;
	} else {
		snprintf(str, 32, "TS: false");
		flgptn |= TS_OFF;
	}
	lcd->drawString(str, 0, lcd->getCurrentFontHeight()*2);

	// リフトが上に行き過ぎたり・下に行き過ぎたりした場合
	int32_t count = mFront->getCount();
	if (count >= LIFT_MAX) {
		flgptn |= LIFT_OVER;
	} else if (count <= LIFT_MIN) {
		flgptn |= LIFT_UNDER;
	} else {
		flgptn |= LIFT_NORMAL;
	}
	snprintf(str, 32, "LIFT: %ld", count);
	lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 3);

	// フラグをセット
	if ( flgptn != 0 ) {
		snprintf(str, 32, "SET FLG: 0x%x", flgptn);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);

		//　本来Toppers/HRPの仕様では非タスクコンテキストではiset_flgを使うべきだが
		// ダイナミックローディング環境では loader のタスク上でユーザアプリの周期タスクが
		// 実現されている模様。
		set_flg(SW_FLG, flgptn);
	}
}

// TASK defined in app.cfg
void main_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("Hello MAIN_TASK!!", 0, 0);

	char str[32] = "";
	FLGPTN flgptn = 0;
	while (true) {

		// 指定したビットの範囲(ALLMASK)において
		// いずれかのビットがセットされたら起床
		wai_flg(SW_FLG, ALLMASK, TWF_ORW, &flgptn);

		snprintf(str, 32, "WAI FLG: 0x%x", flgptn);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight()*5);

		if ( flgptn & TS_ON ) {
			// タッチセンサーが押されている間は警告音を抑制する
			lcd->drawString("TS_ON", 0, lcd->getCurrentFontHeight()*6);
			continue;
		}

		if ( flgptn & LIFT_OVER ) {
			// リフトが上限を超えたら警告音（高い）を出す
			speaker->playTone(1000, 10);
			lcd->drawString("LIFT OVER", 0, lcd->getCurrentFontHeight()*6);
		}
		else if ( flgptn & LIFT_UNDER ) {
			// リフトが上限を超えたら警告音（低い）を出す
			speaker->playTone(500, 10);
			lcd->drawString("LIFT_UNDER", 0, lcd->getCurrentFontHeight()*6);
		}
		else if ( flgptn & LIFT_NORMAL ) {
			lcd->drawString("LIFT_NORMAL", 0, lcd->getCurrentFontHeight()*6);
		}
	}
}
