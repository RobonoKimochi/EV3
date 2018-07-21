#ifndef TAIL_STAND_H
#define TAIL_STAND_H

#include "TailMotor.h"
#include "Starter.h"

class TailStand {
public:
    TailStand(TailMotor* tailMotor, Starter* starter);
    void run();
private:
    TailMotor* mTailMotor;
    Starter* mStarter;
    bool mFlagInit;
    bool mFlagTailDown;

};

#endif