#include "Starter.h"

Starter::Starter(ev3api::TouchSensor& touchSensor)
    : mTouchSensor(touchSensor),
      mPushed(false)
{
}

bool Starter::isPushed() {
    if(!mPushed) {
        if(mTouchSensor.isPressed()) {
            mPushed = true;
        }
    }
    return mPushed;
}
