/*
 * EV3ext.h
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */

#ifndef EV3EXT_H_
#define EV3EXT_H_

#include "ev3api.h"

namespace ev3api {

class EV3ext {

public:
	explicit EV3ext();
	~EV3ext();

public:
	int getCurrentmA();
	int getCurrentmV();
	ER setLEDColor(ledcolor_t color);
};

}

#endif /* EV3EXT_H_ */
