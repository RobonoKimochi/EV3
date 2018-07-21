/*
 * Speaker.h
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_

#include "ev3api.h"

namespace ev3api {

class Speaker {

public:
	explicit Speaker();
	~Speaker();

public:
	ER setVolume(uint8_t volume);
	ER playTone(uint16_t frequency, int32_t duration);
	ER playFile(const memfile_t *memfile, int32_t duration);
	ER stop();
};

}

#endif /* SPEAKER_H_ */
