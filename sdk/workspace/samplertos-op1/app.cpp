/**
 * RTOS Task Operation Sample Program
 * EV3 Buttons (Left, Right, Up, Down, Enter, Back) assigned Task and Operation.
 *
 * USER_TASK1の操作
 * Right Button: タスクの起動(act_tsk)/タスクの終了(ter_tsk): USER_TASK1
 * Up Button: タスクのサスペンド(sus_tsk)/タスクのレジューム(rsm_tsk): USER_TASK1
 *
 * USER_TASK2の操作
 * Left Button: タスクの起動(act_tsk)/タスクの終了(ter_tsk): USER_TASK2
 * Down Button: タスクのサスペンド(sus_tsk)/タスクのレジューム(rsm_tsk): USER_TASK2
 *
 * USER_TASK1,UESR_TASK2の優先度の入れ替え
 * Back Button: 優先度の変更(chg_pri): user_task1 <-> user_task2
 *
 * CYC_HANDLER(周期タスク)の操作
 * Enter Button: 周期ハンドラ起動(sta_cyc)/周期ハンドラ停止(stp_cyc): cyc_handler
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
auto leftButton = new Button(LEFT_BUTTON);
auto rightButton = new Button(RIGHT_BUTTON);
auto upButton = new Button(UP_BUTTON);
auto downButton = new Button(DOWN_BUTTON);
auto enterButton = new Button(ENTER_BUTTON);
auto backButton = new Button(BACK_BUTTON);

void wait(uint32_t duration) {
	uint32_t base = clock->now();
	while (base + duration > clock->now())
		;
	return;
}

void act_ter_tsk(intptr_t tskid) {
	char str[32] = "";

	T_RTSK refTsk;
	if (ref_tsk(tskid, &refTsk) == E_OK) {
		if (refTsk.tskstat != TTS_RUN && refTsk.tskstat != TTS_RDY) {
			snprintf(str, 32, "act_tsk: %d", tskid);
			lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
			act_tsk(tskid);
		} else {
			snprintf(str, 32, "ter_tsk: %d", tskid);
			lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
			ter_tsk(tskid);
		}
	}
}

void sus_rsm_tsk(intptr_t tskid) {
	char str[32] = "";

	T_RTSK refTsk;
	if (ref_tsk(tskid, &refTsk) == E_OK) {
		if (refTsk.tskstat == TTS_SUS) {
			snprintf(str, 32, "rsm_tsk: %d", tskid);
			lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
			rsm_tsk(tskid);
		} else {
			snprintf(str, 32, "sus_tsk: %d", tskid);
			lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
			sus_tsk(tskid);
		}
	}
}

// 周期ハンドラの起動・停止処理
// ダイナミックローディング環境において cyc_sta, cyc_stp, ref_cyc は使えないことに注意。
// EV3環境で提供された ev3_cyc_sta, ev3_cyc_stp を利用すること。
// また ev3_ref_cycは提供されていない
bool_t isCycHandlerRunning = false;
void sta_stp_cyc(intptr_t cycid) {
	char str[32] = "";

	if (isCycHandlerRunning == false) {
		snprintf(str, 32, "sta_cyc: %d", cycid);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
		ev3_sta_cyc(cycid);
		isCycHandlerRunning = true;
	} else {
		snprintf(str, 32, "stp_cyc: %d", cycid);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 4);
		ev3_stp_cyc(cycid);
		isCycHandlerRunning = false;
	}
}

// USER_TASK1とUSER_TASK2の優先度を逆転させる
void rotate_priority(intptr_t unused) {
	lcd->drawString("rotate_priority", 0, lcd->getCurrentFontHeight() * 4);

	PRI t1pri;
	PRI t2pri;
	if (get_pri(USER_TASK1, &t1pri) == E_OK
			&& get_pri(USER_TASK2, &t2pri) == E_OK) {
		chg_pri(USER_TASK1, t2pri);
		chg_pri(USER_TASK2, t1pri);
	}
}

const char* getStateSName(intptr_t tskid) {
	T_RTSK refTsk;
	if (ref_tsk(tskid, &refTsk) == E_OK) {
		switch (refTsk.tskstat) {
		case TTS_RUN:
			return "RUN";
		case TTS_RDY:
			return "RDY";
		case TTS_SUS:
			return "SUS";
		case TTS_WAI:
			return "WAI";
		case TTS_WAS:
			return "WAS";
		case TTS_DMT:
			return "DMT";
		default:
			return "-";
		}
	}
	return "-";
}

const PRI getPriority(intptr_t tskid) {
	PRI pri;
	if (get_pri(tskid, &pri) == E_OK) {
		return pri;
	}
	return (-1);
}

// 各タスクの状態を表示する周期ハンドラ
void show_handler(intptr_t unused) {
	char str[32] = "";

	snprintf(str, 32, "UT1: %d/%s/%d", USER_TASK1, getStateSName(USER_TASK1),
			getPriority(USER_TASK1));
	lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 5);
	snprintf(str, 32, "UT2: %d/%s/%d", USER_TASK2, getStateSName(USER_TASK2),
			getPriority(USER_TASK2));
	lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 6);
	snprintf(str, 32, "CYC: %d/%s", SHOW_HANDLER, getStateSName(SHOW_HANDLER));
	lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 7);
}

// CYC_HANDLER defined in app.cfg
uint16_t cyc_handler_times = 0;
void cyc_handler(intptr_t unused) {
	char str[32] = "";
	snprintf(str, 32, "CYC_HANDLER: %d", cyc_handler_times++);
	lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 3);
}

// TASK defined in app.cfg
void main_task(intptr_t unused) {
	lcd->setFont(EV3_FONT_MEDIUM);
	lcd->drawString("Hello MAIN_TASK!!", 0, 0);

	rightButton->setOnClicked(act_ter_tsk, USER_TASK1);
	leftButton->setOnClicked(act_ter_tsk, USER_TASK2);
	upButton->setOnClicked(sus_rsm_tsk, USER_TASK1);
	downButton->setOnClicked(sus_rsm_tsk, USER_TASK2);
	enterButton->setOnClicked(sta_stp_cyc, CYC_HANDLER);
	backButton->setOnClicked(rotate_priority, 0);
}

// TASK defined in app.cfg
void user_task1(intptr_t unused) {
	char str[32] = "";
	int16_t n = 0;
	while (true) {
		snprintf(str, 32, "USER_TASK1: %d", n++);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight());
		wait(1000);
	}
}

// TASK defined in app.cfg
void user_task2(intptr_t unused) {
	char str[32] = "";
	int16_t n = 0;
	while (true) {
		snprintf(str, 32, "USER_TASK2: %d", n++);
		lcd->drawString(str, 0, lcd->getCurrentFontHeight() * 2);
		wait(1000);
	}
}
