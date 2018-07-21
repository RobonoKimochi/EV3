/*
 * Button.h
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "ev3api.h"

namespace ev3api {

class Button {

private:
	button_t button = LEFT_BUTTON;

public:
	explicit Button(button_t button);
	virtual ~Button();

public:
	/**
	 * ボタンの数を返す
	 */
	uint16_t getTNum();

public:

	/**
	 * ボタンの押下状態を取得する．
	 *
	 * @return
	 * 		true 押された状態
	 * 		false 押されていない状態
	 */
	virtual bool_t isPressed();

	/**
	 * 指定したボタンのクリックイベントハンドラを設定する．
	 * ボタンハンドラはタスクコンテストで実行する．デフォルトは，待ち禁止状態から呼び出される．
	 *
	 * @param handler　イベントハンドラ．NULLを指定した場合，元のハンドラがクリアされる
	 * @param exinf　イベントハンドラの拡張情報
	 * @return
	 * 			E_OK 正常終了
	 * 			E_ID 不正のボタン番号
	 */
	virtual ER setOnClicked(ISR handler, intptr_t exinf);


	/**
	 * ボタンの名前を返す
	 * @return LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, ENTER_BUTTON, BACK_BUTTON
	 */
	const char* toString();
};

}

#endif /* BUTTON_H_ */
