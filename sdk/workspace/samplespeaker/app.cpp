/**
 * Speaker API Sample Program
 *
 * 1. show message on LCD
 * 2. play sound
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "Speaker.h"
#include "LCD.h"
#include "Clock.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto clock = new Clock();
auto lcd = new LCD();
auto speaker = new Speaker();

void play() {
	unsigned char vol = 25;
	int note = 150;
	int pow = 2;

	// ボリューム設定
	speaker->setVolume(vol);

	// 第1引数：周波数(Hz)，第2引数:長さ (ms)
	speaker->playTone(1,1);
	speaker->playTone(783, note * pow);
	clock->wait(note);
	speaker->playTone(739, note * pow);
	clock->wait(note);
	speaker->playTone(622, note * pow);
	clock->wait(note);
	speaker->playTone(440, note * pow);
	clock->wait(note);
	speaker->playTone(415, note * pow);
	clock->wait(note);
	speaker->playTone(659, note * pow);
	clock->wait(note);
	speaker->playTone(830, note * pow);
	clock->wait(note);
	speaker->playTone(1046, note * pow);
	clock->wait(note * 2);
}

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {
	lcd->drawString("Play Sound!!", 0, 0, true);
	play();
}
