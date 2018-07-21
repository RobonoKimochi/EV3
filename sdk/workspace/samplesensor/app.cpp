/**
 * Sensor API Sample Program
 *
 * Keep to show sensor values in LCD
 * - TouchSensor
 * - ColorSensor
 * - GyroSensor
 * - SonarSensor
 *
 * Written by Yasuhiro Noguchi
 * Email: yasuhiro.noguchi@gmail.com
 */

#include "ev3api.h"
#include "app.h"

// 利用するAPIのヘッダファイル
#include "LCD.h"
#include "Clock.h"
#include "TouchSensor.h"
#include "GyroSensor.h"
#include "ColorSensor.h"
#include "SonarSensor.h"

// EV3APIの名前空間
using namespace ev3api;

// EV3APIのオブジェクト
auto lcd = new LCD();
auto clock = new Clock();
auto touch = new TouchSensor(PORT_1);
auto color = new ColorSensor(PORT_2);
auto sonar = new SonarSensor(PORT_3);
auto gyro = new GyroSensor(PORT_4);

/**
 * 横線を引く
 * @param y 位置
 * @param weight 太さ
 * @return 描画に使ったLCDの高さ方向の幅
 */
int32_t drawSeparator(int32_t y, int32_t weight) {
	lcd->drawLine(0, y, lcd->getWidth(), y + weight - 1);
	return weight;
}

/**
 * LCD上の(0, Y)からタッチセンサー値を表示する
 * @param y LCDのY座標
 * @return 描画に使ったLCDの高さ方向の幅
 */
int32_t drawTouchSensorValue(int32_t y) {

	char str[32] = "";
	int32_t fontHeight = lcd->getCurrentFontHeight();

	// タッチセンサー値の表示
	lcd->drawString("TouchSensor", 0, y);
	snprintf(str, 32, "  isPressed(): %d", touch->isPressed());
	lcd->drawString(str, 0, y + fontHeight);

	return fontHeight * 2;
}

/**
 * LCD上の(0, Y)からカラーセンサー値を表示する
 * @param y LCDのY座標
 * @return 描画に使ったLCDの高さ方向の幅
 */
int32_t drawColorSensorValue(int32_t y) {

	char str[32] = "";
	int32_t fontHeight = lcd->getCurrentFontHeight();

	lcd->drawString("ColorSensor", 0, y);
	snprintf(str, 32, "  getAmbient(): %d", color->getAmbient());
	lcd->drawString(str, 0, y + fontHeight);
	snprintf(str, 32, "  getBrightness(): %d", color->getBrightness());
	lcd->drawString(str, 0, y + fontHeight * 2);
	snprintf(str, 32, "  getColorNumber(): %d", color->getColorNumber());
	lcd->drawString(str, 0, y + fontHeight * 3);
	rgb_raw_t rgb;
	color->getRawColor(rgb);
	lcd->drawString("  getColorRawColor(rgb):", 0, y + fontHeight * 3);
	snprintf(str, 32, "    r=%d, g=%d, b=%d", rgb.r, rgb.g, rgb.b);
	lcd->drawString(str, 0, y + fontHeight * 4);

	return fontHeight * 5;
}

/**
 * LCD上の(0, Y)からソナーセンサー値を表示する
 * @param y LCDのY座標
 * @return 描画に使ったLCDの高さ方向の幅
 */
int32_t drawSonarSensorValue(int32_t y) {

	char str[64] = "";
	int32_t fontHeight = lcd->getCurrentFontHeight();

	lcd->drawString("SonarSensor", 0, y);
	snprintf(str, 64, "  getDistance(): %d [cm]", sonar->getDistance());
	lcd->drawString(str, 0, y + fontHeight);
	snprintf(str, 64, "  listen(): %d", sonar->listen());
	lcd->drawString(str, 0, y + fontHeight * 2);

	return fontHeight * 3;
}

/**
 * LCD上の(0, Y)からジャイロセンサー値を表示する
 * @param y LCDのY座標
 * @return 描画に使ったLCDの高さ方向の幅
 */
int32_t drawGyroSensorValue(int32_t y) {

	char str[64] = "";
	int32_t fontHeight = lcd->getCurrentFontHeight();

	lcd->drawString("GyroSensor", 0, y);
	snprintf(str, 64, "  getAnglerVelocity(): %d [deg/sec]",
			gyro->getAnglerVelocity());
	lcd->drawString(str, 0, y + fontHeight);
	snprintf(str, 64, "  getAngle(): %d [deg]", gyro->getAngle());
	lcd->drawString(str, 0, y + fontHeight * 2);

	return fontHeight * 3;
}

void drawSensorValues(int32_t y) {

	// タッチセンサー値の表示
	y += drawTouchSensorValue(y);

	// セパレータを引く
	y += drawSeparator(y, 1);

	// カラーセンサー値の表示
	y += drawColorSensorValue(y);

	// セパレータを引く
	y += drawSeparator(y, 1);

	// ソナーセンサー値の表示
	y += drawSonarSensorValue(y);

	// セパレータを引く
	y += drawSeparator(y, 1);

	// ジャイロセンサー値の表示
	y += drawGyroSensorValue(y);
}

// MAIN_TASK defined in app.cfg
void main_task(intptr_t unused) {

	lcd->drawString("main_task started.", 0, 0);
	int32_t fontHeight = lcd->getCurrentFontHeight();
	gyro->reset();
	gyro->setOffset(0);

	// 周期的にセンサー値表示を更新する
	while (true) {
		drawSensorValues(fontHeight + 1);
		clock->wait(100);
	}
}
