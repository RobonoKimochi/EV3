/******************************************************************************
 *  LineTracerWithStarter.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracerWithStarter
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACERWITHSTARTER_H_
#define EV3_APP_LINETRACERWITHSTARTER_H_

#include "Starter.h"
#include "LineTracer.h"
#include "Calibration.h"
#include "Remort.h"
#include "MeasureDistance.h"
#include "LookUpGate.h"
#include "Garage.h"
#include "BalancingWalker.h"
#include "Sound.h"

class LineTracerWithStarter {
public:
    LineTracerWithStarter(LineTracer* lineTracer,
               const Starter* starter,
               Calibration* calibration,
               Remort*		   Remort,
               LookUpGate*     LookUpGate,
               MeasureDistance *measureDistance,
               Garage*         garage,
               BalancingWalker* balancingWalker
               );
    virtual ~LineTracerWithStarter();

    void run();

    int debug;  // !!
	bool TailInit;
	bool LookUpCompFlag;

private:
    enum State {
        UNDEFINED,
    	CALIBRATION_TAIL,
        CALIBRATION_GYRO,
        CALIBRATION_BLACK,
        CALIBRATION_WHITE,
        WAITING_FOR_START,
        WALKING,
    	REMORT,
    	LOOKUPGATE,
    	GARAGE,
        GRAY_DETECT
    };

    LineTracer* mLineTracer;
    const Starter* mStarter;
    Calibration* mCalibration;
	Remort* mRemort;
	LookUpGate* mLookUpGate;// = new LookUpGate();
	MeasureDistance* mMeasureDistance;// = new MeasureDistance();
	Garage* mGarage;
    BalancingWalker* mBalancingWalker;
    Sound *mSound = Sound::getInstance();
public:
    State mState;
private:
	int TimeCount;

    void execUndefined();
	void execCalibrationTail();
    void execCalibrationGyro();
    void execCalibrationBlack();
    void execCalibrationWhite();
    void execWaitingForStart();
    void execWalking();
	void execLookUpGate();
	void execGarage();
	void remort();
    void execGrayDetect();

    TailMotor *mTailMotor = TailMotor::getInstance();
public:
    bool mStartSignal;
};

#endif  // EV3_APP_LINETRACERWITHSTARTER_H_
