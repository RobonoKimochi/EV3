/*
 * Bluetooth.cpp
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */
#include "Bluetooth.h"

using namespace ev3api;

Bluetooth::Bluetooth() {
	fpBT = ev3_serial_open_file(EV3_SERIAL_BT);
}

Bluetooth::~Bluetooth() {
}

bool_t Bluetooth::isConnected() {
	if ( fpBT == NULL ) {
		return false;
	}
	return ev3_bluetooth_is_connected();
}

void Bluetooth::send(const char* msg) {
	if ( isConnected() ) {
		fprintf(fpBT, msg);
	}
}

int16_t Bluetooth::recv(char* buf, int16_t size) {
	if ( size <= 1 ) {
		return 0;
	}

	if ( !isConnected() ) {
		return 0;
	}
	buf[0] = '\0';

	int pos = 0;
	char c = fgetc(fpBT);
	while ( pos < size - 1 && c != '\r' ) {
		buf[pos] = c;
		pos++;
		c = fgetc(fpBT);
	}
	buf[pos] = '\0';
	return pos;
}

