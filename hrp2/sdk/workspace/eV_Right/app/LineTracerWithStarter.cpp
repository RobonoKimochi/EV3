/******************************************************************************
 *  LineTracerWithStarter.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracerWithStarter
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracerWithStarter.h"
#include "ev3api.h"
#include "MotorDriver.h"

#include "TailMotor.h"
/**
 * コンストラクタ
 * @param lineTracer ライントレーサ
 * @param starter    スタータ
 */
 LineTracerWithStarter::LineTracerWithStarter(LineTracer* lineTracer,
                                              const Starter* starter,
                                              Calibration* calibration,
 											 Remort*     Remort,
 											 LookUpGate*     LookUpGate,
 											 MeasureDistance *measureDistance,
 											 Garage*         Garage,
                                              BalancingWalker* balancingWalker
 											 )
    : TailInit(false),
      LookUpCompFlag(false),
      mLineTracer(lineTracer),
      mStarter(starter),
      mCalibration(calibration),
      mRemort(Remort),
      mLookUpGate(LookUpGate),
      mMeasureDistance(measureDistance),
      mGarage(Garage),
      mBalancingWalker(balancingWalker),
      mState(UNDEFINED),
      TimeCount(0),
      mStartSignal(false){
}

/**
 * デストラクタ
 */
LineTracerWithStarter::~LineTracerWithStarter() {
}

/**
 * ライントレースする
 */
void LineTracerWithStarter::run() {
    debug = mState; // !!
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case CALIBRATION_TAIL:
        execCalibrationTail();
        break;
    case CALIBRATION_GYRO:
    	mTailMotor->setAngle(94);	// 開始待ち時尻尾91°
        execCalibrationGyro();
        break;
    case CALIBRATION_BLACK:
        execCalibrationBlack();
        break;
    case CALIBRATION_WHITE:
        execCalibrationWhite();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case WALKING:
        execWalking();
        break;
    case REMORT:
    	remort();
    	break;
    case LOOKUPGATE:
    	execLookUpGate();
    	break;
    case GRAY_DETECT:
        execGrayDetect();
        break;
    case GARAGE:
    	execGarage();
    	break;
    default:
        break;
    }
}

/**
 * 未定義状態の処理
 */
void LineTracerWithStarter::execUndefined() {

	ev3_lcd_set_font(EV3_FONT_MEDIUM);
	ev3_lcd_draw_string("RightCourse", 10, 10);
	ev3_lcd_draw_string("RightEdge", 10, 30);
	mTailMotor->init(0);
    mCalibration->init();
	mState = CALIBRATION_TAIL;
}

/**
 * テールモータのキャリブレーション
 */
void LineTracerWithStarter::execCalibrationTail() {

	ev3_lcd_draw_string("TAILCALIBRATION", 10, 50);

	if(TailInit == false) {
		mTailMotor->setPWM(-2);
	}

	if(mStarter->isPushed() == true) {
		mTailMotor->setPWM(0);
		mTailMotor->init(0);
		TailInit = true;
	}

	if(TailInit == true) {

		TimeCount++;
	}

	if(TimeCount > 100) {
		mState = CALIBRATION_GYRO;
        mSound->ok();
	}


}

/**
 * ジャイロセンサのキャリブレーション
 */
void LineTracerWithStarter::execCalibrationGyro() {
    if (mCalibration->calibrateGyro(mStarter->isPushed()) == true) {

        mState = CALIBRATION_BLACK;
        mSound->ok();
        // mState = WAITING_FOR_START; // きゃりぶれ飛ばし
    }
}

/**
 * 黒キャリブレーション
 */
void LineTracerWithStarter::execCalibrationBlack() {
    if (mCalibration->calibrateBlack(mStarter->isPushed()) == true) {

        mState = CALIBRATION_WHITE;
        mSound->ok();
    }

}

/**
 * 白キャリブレーション
 */
void LineTracerWithStarter::execCalibrationWhite() {
    if (mCalibration->calibrateWhite(mStarter->isPushed()) == true) {

        mState = WAITING_FOR_START;
    	ev3_lcd_fill_rect(0,0,EV3_LCD_WIDTH,EV3_LCD_HEIGHT,EV3_LCD_WHITE);
    	ev3_lcd_draw_string("RightCourse", 10, 30);
		ev3_lcd_draw_string("RightEdge", 10, 50);
		ev3_lcd_draw_string("READY", 60, 70);
        mSound->trumpet();
    }

}

/**
 * 開始待ち状態の処理
 */
void LineTracerWithStarter::execWaitingForStart() {


    if (mStarter->isPushed() || mStartSignal == true) {

        mCalibration->calibrateLineThreshold();

        mTailMotor->setAngle(0);	// 開始時尻尾0°

        mState = WALKING;

    }
}

/**
 * 走行中状態の処理
 */
void LineTracerWithStarter::execWalking() {
	if(mRemort->RemortState() == true) {
		mState = REMORT;
	} else {
        mLineTracer->run();
        if( mLineTracer->isFinished() == true && mMeasureDistance->DetectGate() == true){
            mState = LOOKUPGATE;
		}
	}
}

/**
 * ルックアップゲート状態の処理
 */
void LineTracerWithStarter::execLookUpGate() {
	if(mRemort->RemortState() == true) {
		mState = REMORT;
	} else {
		LookUpCompFlag =mLookUpGate->RunLookUpGate();
		if(LookUpCompFlag == true) {
			mState = GARAGE;
            // mState = GRAY_DETECT;
		}
	}
}

/**
 * 車庫入れ状態の処理
 */
void LineTracerWithStarter::execGarage() {
	ev3_led_set_color(LED_ORANGE);
	if(mRemort->RemortState() == true) {
		mState = REMORT;
	} else {
		mGarage->StateGarage();
	}
}
/**
 * 灰色検知してガレージに渡す処理
 */
void LineTracerWithStarter::execGrayDetect() {
	ev3_led_set_color(LED_OFF);
	if(mRemort->RemortState() == true) {
		mState = REMORT;
	} else {
		if( mLineTracer->detectGray() == true){
            mState = GARAGE;
        }
	}
}/**
 * リモート中の処理
 */
void LineTracerWithStarter::remort() {
	if(mRemort->RemortState() == false) {
		mState = WALKING;
	} else {
		mRemort->RemortControl();
	}
}
