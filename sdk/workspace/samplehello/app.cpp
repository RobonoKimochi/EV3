/**
 * Hello EV3 Sample Program
 *
 * Very Simple Sample Program for 1st Build.
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "LCD.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto lcd = new LCD();

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("Hello EV3!!", 0, 0);
}
