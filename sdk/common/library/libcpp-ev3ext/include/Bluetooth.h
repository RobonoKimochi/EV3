/*
 * Bluetooth.h
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "ev3api.h"

namespace ev3api {

class Bluetooth {

private:
	FILE* fpBT;

public:
	explicit Bluetooth();
	virtual ~Bluetooth();

public:
	bool_t isConnected();
	void send(const char* msg);
	int16_t recv(char* buf, int16_t size);
};

}

#endif /* BLUETOOTH_H_ */
