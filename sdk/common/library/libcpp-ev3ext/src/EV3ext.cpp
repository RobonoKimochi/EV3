/*
 * EV3ext.cpp
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */
#include "EV3ext.h"

using namespace ev3api;

EV3ext::EV3ext() {
}

EV3ext::~EV3ext() {
}

int EV3ext::getCurrentmA() {
	return ev3_battery_current_mA();
}

int EV3ext::getCurrentmV() {
	return ev3_battery_voltage_mV();
}

ER EV3ext::setLEDColor(ledcolor_t color) {
	return ev3_led_set_color(color);
}

