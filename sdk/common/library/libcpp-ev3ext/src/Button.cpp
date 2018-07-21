/*
 * Button.cpp
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */
#include "Button.h"

using namespace ev3api;

Button::Button(button_t button) {
	this->button = button;
}

Button::~Button() {
}

bool_t Button::isPressed() {
	return ev3_button_is_pressed(button);
}

ER Button::setOnClicked(ISR handler, intptr_t exinf) {
	return ev3_button_set_on_clicked(button, handler, exinf);
}

uint16_t Button::getTNum() {
	return TNUM_BUTTON;
}

const char* Button::toString() {
	switch(button) {
		case 0:
			return "LEFT_BUTTON";
			break;
		case 1: 
			return "RIGHT_BUTTON";
			break;
		case 2:
			return "UP_BUTTON";
			break;
		case 3:
			return "DOWN_BUTTON";
			break;
		case 4:
			return "ENTER_BUTTON";
			break;
		case 5:
			return "BACK_BUTTON";
			break;
		default:
			return "UNKNOWN_BUTTON";
			break;
	}
	return "UNKNOWN_BUTTON";
}

