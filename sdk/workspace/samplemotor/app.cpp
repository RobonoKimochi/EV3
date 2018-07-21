/**
 * Motor API Sample Program
 *
 * 1. pivot left turn in 10sec
 * 2. pivot right turn in 10 sec
 * 3.　stop
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "Motor.h"
#include "Port.h"
#include "Clock.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto mRight = new Motor(PORT_B);
auto mLeft = new Motor(PORT_C);
auto mFront = new Motor(PORT_A);
auto mRear = new Motor(PORT_D, true, MEDIUM_MOTOR);
auto clock = new Clock();

#include "LCD.h"
auto lcd = new LCD();

/**
 * 現在のカウント値をLCDに表示する
 * @param y LCD上の表示位置Y座標
 */
void showCurrentCount(uint16_t y) {
	int32_t countLeft = mLeft->getCount();
	int32_t countRight = mRight->getCount();
	int32_t countFront = mFront->getCount();
	int32_t countRear = mRear->getCount();
	int32_t fontHeight = lcd->getCurrentFontHeight();
	char msg[32] = "";
	snprintf(msg, 32, "LEFT:  %3ld", countLeft);
	lcd->drawString(msg, 0, y);
	snprintf(msg, 32, "RIGHT: %3ld", countRight);
	lcd->drawString(msg, 0, y+fontHeight);
	snprintf(msg, 32, "FRONT: %3ld", countFront);
	lcd->drawString(msg, 0, y+fontHeight*2);
	snprintf(msg, 32, "REAR:  %3ld", countRear);
	lcd->drawString(msg, 0, y+fontHeight*3);
}

/**
 * 超振地旋回（LCDにカウント値表示）
 * @param speed PWM値 (-100～100), 負数の場合左方向、正数の場合右方向
 * @param time 旋回時間[sec]
 */
void pivotTurn(int16_t speed, int16_t time) {

	// 現在のカウント値を表示
	showCurrentCount(0);

	// モード指定: ブレーキモード
	mRight->setBrake(true);
	mLeft->setBrake(true);
	mRear->setBrake(true);

	// 超振地旋回
	mRight->setPWM((-1)*speed);
	mLeft->setPWM(speed);
	mRear->setPWM((-1)*speed);

	// 旋回継続
	for (int i = 0; i < time*10; i++) {
		// 現在のカウント値を表示
		showCurrentCount(0);
		clock->wait(100);
	}

	// 停止
	mRight->setPWM(0);
	mLeft->setPWM(0);
	mRear->setPWM(0);

	// 現在のカウント値を表示
	showCurrentCount(0);
}

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {

	// モータ停止 && ロータリーエンコーダのオフセットを0リセット
	mRight->reset();
	mLeft->reset();
	mFront->reset();
	mRear->reset();

	// 超振地旋回（左方向）, 5sec
	pivotTurn(-50, 5);

	// 超振地旋回（右方向）, 5sec
	pivotTurn(50, 5);

	// 5sec現在位置維持
	clock->wait(5000);

	//////////////////////////////
	// 以下、角度指定の動作テスト
	// blockingをtrueにした場合、モータ設置時の物理的制約によりその角度に至らない場合
	// 関数が戻らないので注意すること

	// モーター位置初期化
	ev3_motor_rotate(EV3_PORT_A, 0, 50, false);
	ev3_motor_rotate(EV3_PORT_D, 0, 50, false);

	// モーター角度30度ずつ前進
	for ( int i=0; i<5; i++ ) {
		mLeft->rotate(30, 50, false);
		mRight->rotate(30, 50, false);
		mRear->rotate(30, 50, false);
		showCurrentCount(0);
		clock->wait(1000);
	}

	// モーター角度30度ずつ後進
	for ( int i=0; i<5; i++ ) {
		mLeft->rotate(-30, 50, false);
		mRight->rotate(-30, 50, false);
		mRear->rotate(-30, 50, false);
		showCurrentCount(0);
		clock->wait(1000);
	}

	// モーター角度3度ずつリフトを上げる
	for ( int i=0; i<5; i++ ) {
		mFront->rotate(3, 50, false);
		showCurrentCount(0);
		clock->wait(1000);
	}

	// モーター角度3度ずつリフトを下げる
	for ( int i=0; i<5; i++ ) {
		mFront->rotate(-3, 50, false);
		showCurrentCount(0);
		clock->wait(1000);
	}
}
