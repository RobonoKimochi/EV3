#include "TailStand.h"

TailStand::TailStand(TailMotor* tailMotor, Starter* starter)
    : mTailMotor(tailMotor),
      mStarter(starter),
      mFlagInit(false),
      mFlagTailDown(false)
{
    mTailMotor->init(0);
}

void TailStand::run() {
    if(mStarter->isPushed()) {
        if(!mFlagInit) {
            mFlagInit = true;
            mTailMotor->setAngle(80);
        }   
        mFlagTailDown = mTailMotor->doMoveTail();
    }
}

