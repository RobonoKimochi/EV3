/*
 * LCD.h
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */

#ifndef LCD_H_
#define LCD_H_

#include "ev3api.h"

namespace ev3api {

class LCD {

private:
	static lcdfont_t currentFont;

public:
	// コンストラクタ
	explicit LCD();

	// デストラクタ
	~LCD();

public:
	// wrapped LCD function in EV3RT C API
	ER getFontSize(lcdfont_t font, int32_t * width, int32_t * height);
	ER drawImage(const image_t * image, int32_t x, int32_t y);
	ER drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
	ER setFont(lcdfont_t font);
	int32_t getCurrentFontHeight();
	int32_t getCurrentFontWidth();

	/**
	 * 指定位置に文字列を出力するメソッド
	 * 指定位置以降を出力前に空白で初期化する（autoClear: trueの場合）
	 */
	ER drawString(const char * str, int32_t x, int32_t y, bool_t autoClear = true);

	ER fillRect(int32_t x, int32_t y, int32_t w, int32_t h, lcdcolor_t color);
	ER loadImage(const memfile_t * memfile, image_t * image);
	ER freeImage(image_t * image);

	void clear();

	uint16_t getHeight();
	uint16_t getWidth();
};

}

#endif /* LCD_H_ */
