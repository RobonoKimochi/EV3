#ifndef STARTER_H_
#define STARTER_H_

#include "TouchSensor.h"

using ev3api::TouchSensor;

class Starter {
    TouchSensor& mTouchSensor;
    bool mPushed;
public:
    Starter(TouchSensor& touchSensor);
    bool isPushed();
};

#endif
