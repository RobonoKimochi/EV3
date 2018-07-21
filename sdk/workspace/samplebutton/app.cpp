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
#include "Button.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto clock = new Clock();
auto lcd = new LCD();

// EV3本体のボタンに対応するオブジェクト
Button* buttonSet[TNUM_BUTTON];
auto leftButton = new Button(LEFT_BUTTON);
auto rightButton = new Button(RIGHT_BUTTON);
auto upButton = new Button(UP_BUTTON);
auto downButton = new Button(DOWN_BUTTON);
auto enterButton = new Button(ENTER_BUTTON);
auto backButton = new Button(BACK_BUTTON);

/**
 * ボタンが押された時に呼び出されるイベントハンドラ
 * @param button 押されたボタンが登録された配列のインデックス
 */
void buttonOnClick(intptr_t button) {
	char str[64] = "";
	snprintf(str, 64, "%s on Click", buttonSet[button]->toString());
	lcd->drawString(str, 0, lcd->getCurrentFontHeight()*8);
}

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {

	// ボタンを配列に登録
	buttonSet[0] = leftButton;
	buttonSet[1] = rightButton;
	buttonSet[2] = upButton;
	buttonSet[3] = downButton;
	buttonSet[4] = enterButton;
	buttonSet[5] = backButton;

	// ボタンの情報をLCDに出力
	char str[64] = "";
	uint16_t fontHeight = lcd->getCurrentFontHeight();
	snprintf(str, 64, "Buttons on EV3: Num: %d", leftButton->getTNum());
	lcd->drawString(str, 0, 0);
	for ( int i=0; i<leftButton->getTNum(); i++) {
		snprintf(str, 64, "  %d: %s", i, buttonSet[i]->toString());
		lcd->drawString(str, 0, fontHeight*(i+1));
	}

	// 以下のボタンクリック検出表示用に線を引く
	lcd->drawLine(0, fontHeight*7, lcd->getWidth(), fontHeight*7);

	// ボタン押下に対するイベントハンドラを登録
	for ( int i=0; i<leftButton->getTNum(); i++) {
		buttonSet[i]->setOnClicked(buttonOnClick, i);
	}

	// ボタンを押下を検出してLCDに表示
	// ev3rt-beta6.2-release では button->isPressed() は常にfalse!?
	while ( true ) {
		for ( int i=0; i<leftButton->getTNum(); i++) {
			if ( buttonSet[i]->isPressed() == true ) {
				snprintf(str, 64, "%s is Pressed", buttonSet[i]->toString());
				lcd->drawString(str, 0, fontHeight*9);
			}
		}
		clock->wait(200);
	}
}
