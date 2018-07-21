/*
 * Speaker.cpp
 *
 *  Created on: 2016/02/06
 *      Author: yasuh
 */
#include "Speaker.h"

using namespace ev3api;

Speaker::Speaker() {
}

Speaker::~Speaker() {
}

ER Speaker::setVolume(uint8_t volume) {
	return ev3_speaker_set_volume(volume);
}

ER Speaker::playTone(uint16_t frequency, int32_t duration) {
	return ev3_speaker_play_tone(frequency, duration);
}

ER Speaker::playFile(const memfile_t* memfile,
		int32_t duration) {
	return ev3_speaker_play_file(memfile, duration);
}

ER Speaker::stop() {
	return ev3_speaker_stop();
}

