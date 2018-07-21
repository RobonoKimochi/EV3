/******************************************************************************
 *  app.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "LineTracerWithStarter.h"
// added part
#include "Odmetry.h"
#include "UI.h"
// #include "BlueTooth.h"
#include "Logger.h"

#include "MotorDriver.h"
#include "SensorDriver.h"

#include "Remort.h"
#include "AttitudeControl.h"
#include "MeasureDistance.h"
#include "RunManager.h"
#include "TailMotor.h"
#include "BalancingWalker.h"
#include "Sound.h"
// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

// Motor関連
ev3api::Motor gLeftWheel = MotorDriver::getInstance().getLeftWheel();
ev3api::Motor gRightWheel = MotorDriver::getInstance().getRightWheel();
// Sensor関連
ev3api::TouchSensor gTouchSensor = SensorDriver::getInstance().getTouchSensor();
ev3api::ColorSensor gColorSensor = SensorDriver::getInstance().getColorSensor();
ev3api::SonarSensor gSonarSensor = SensorDriver::getInstance().getSonarSensor();
ev3api::GyroSensor gGyroSensor = SensorDriver::getInstance().getGyroSensor();

// オブジェクトの定義
static LineMonitor		*gLineMonitor;
static Balancer			*gBalancer;
static BalancingWalker	*gBalancingWalker;
static LineTracer		*gLineTracer;
static Starter			*gStarter;
static Calibration		*gCalibration;
static PidController	*gPidController;
static LineTracerWithStarter *gLineTracerWithStarter;
static MeasureDistance	*gMeasureDistance;
static UI				*gUI;
static Logger			*gLogger;
static TailMotor		*gTailMotor;// = TailMotor::getInstance();
static RunManager		*gRunManager;
static Garage			*gGarage;
static Remort			*gRemort;
static AttitudeControl	*gAttitudeControl;
static LookUpGate		*gLookUpGate;
static Odmetry			*gOdmetry;
static Sound 			*gSound;

/**
 * EV3システム生成
 */
static void user_system_create() {
	tslp_tsk(2);
    // オブジェクトの作成
    gBalancer        = new Balancer();
    gBalancingWalker = new BalancingWalker(gGyroSensor,
                                           gLeftWheel,
                                           gRightWheel,
                                           gBalancer);
    gLineMonitor     = new LineMonitor(gColorSensor);
    gStarter         = new Starter(gTouchSensor);
    gPidController   = new PidController();
	gAttitudeControl = new AttitudeControl(gBalancingWalker, gGyroSensor);
	gMeasureDistance = new MeasureDistance(gSonarSensor);
	gLookUpGate      = new LookUpGate(gAttitudeControl,gBalancingWalker,gMeasureDistance, gPidController, gLineMonitor);
    gLineTracer      = new LineTracer(gLineMonitor, gBalancingWalker, gPidController);
    gCalibration     = new Calibration(gColorSensor, gGyroSensor, gLineMonitor);
	gRemort          = new Remort(gBalancingWalker);
	gMeasureDistance = new MeasureDistance(gSonarSensor);
	gGarage          = new Garage(gBalancingWalker, gLineTracer, gPidController, gLineMonitor);

    gLineTracerWithStarter = new LineTracerWithStarter(gLineTracer, gStarter, gCalibration, gRemort, gLookUpGate , gMeasureDistance, gGarage, gBalancingWalker);
    gUI             = new UI();
    gLogger         = new Logger();
	gOdmetry		= Odmetry::getInstance();
	gRunManager		= RunManager::getInstance();
	gTailMotor		= TailMotor::getInstance();
	gSound 			= Sound::getInstance();

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
	gSound->ready();
}

/**
 * EV3システム破棄
 */
static void user_system_destroy() {
    gLeftWheel.reset();
    gRightWheel.reset();

    delete gLineTracerWithStarter;
    delete gLineTracer;
    delete gStarter;
    delete gLineMonitor;
    delete gBalancingWalker;
    delete gBalancer;
    delete gCalibration;
    delete gPidController;
    delete gUI;
    delete gLogger;
}
/**
 * メインタスク
 */
void main_task(intptr_t unused) {
    user_system_create();  // センサやモータの初期化処理

	// トレーサタスク開始
	act_tsk(TRACER_TASK);

    // 周期ハンドラ開始
	ev3_sta_cyc(EV3_CYC_ODMETRY);
	ev3_sta_cyc(EV3_CYC_TAILMOTOR);

	// UIタスク開始
	act_tsk(UI_TASK);


    slp_tsk();  // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    // ev3_stp_cyc(EV3_CYC_TRACER);
	ter_tsk(TRACER_TASK);

    user_system_destroy();  // 終了処理

    ext_tsk();
}
/**
 * ライントレース周期タスク
 */
// void ev3_cyc_tracer(intptr_t exinf) {
//     act_tsk(TRACER_TASK);
// }
/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf) {
	while(1){
	    if (ev3_button_is_pressed(BACK_BUTTON)) {
	        wup_tsk(MAIN_TASK);  // バックボタン押下
	    } else {
	        gLineTracerWithStarter->run();  // (倒立)走行
	    }
		tslp_tsk(4);
	}
    // ext_tsk();
}
/**
 * テールモータ周期タスク
 */
void ev3_cyc_tailmotor(intptr_t exinf) {
	act_tsk(TAILMOTOR_TASK);
}
/**
 * テールモータタスク
 */
void tailmotor_task(intptr_t exinf) {
	if(gLineTracerWithStarter->TailInit == true) {
	gTailMotor->moveTail();	// 尻尾制御
	}
	ext_tsk();
}
/**
 * オドメトリ周期タスク
 */
void ev3_cyc_odmetry(intptr_t exinf) {
	act_tsk(ODMETRY_TASK);
}
/**
 * オドメトリタスク
 */
void odmetry_task(intptr_t exinf) {
    gOdmetry->updateLocation();  // 倒立走行
    ext_tsk();
}
/**
 * ロガー周期タスク
 */
void ev3_cyc_logger(intptr_t exinf) {
    act_tsk(LOGGER_TASK);
}
/**
 * ロガータスク
 */
#define LOG_NUM 11
void logger_task(intptr_t exinf){
	float gLog[LOG_NUM];
	// gLog[0]  = gOdmetry->getX();					// X座標
	// gLog[1]  = gOdmetry->getY();					// Y座標
	// gLog[2]  = gOdmetry->getTheta();				// 角度
	// gLog[3]  = gColorSensor.getBrightness();		// カラーセンサ値
	// gLog[4]  = gGyroSensor.getAnglerVelocity();	// ジャイロ値
	// gLog[5]  = gBalancer->getForward();			// Forward値
	// gLog[6]  = gBalancer->getTurn();				// Turn値
	// gLog[7]  = gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
	// gLog[8]  = ev3_battery_voltage_mV();			// バッテリー
	// gLog[9]  = gLineTracerWithStarter->debug;	// mState
	// gLog[10] = gTailMotor->mAngle;				// テールモータのアングル
	// gLog[11] = gSonarSensor.getDistance();		// 距離センサ
	// gLog[12] = gRunManager->mZone;				// 走行区間
	// gLog[13] = gRunManager->dDist;				// 走行区間
	// gLog[14] = gRunManager->dLine;				// 走行区間
	// gLog[15] = gRunManager->dCount;				// 走行区間(kaunto)

	gLog[0]  = gOdmetry->getX();					// X座標
	gLog[1]  = gOdmetry->getY();					// Y座標
	gLog[2]  = gOdmetry->getTheta();				// 角度
	gLog[3]  = gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
	gLog[4]  = ev3_battery_voltage_mV();			// バッテリー
	gLog[5]  = gLineTracerWithStarter->debug;	// mState
	gLog[6]  = gSonarSensor.getDistance();		// 距離センサ
	gLog[7]  = gRunManager->mZone;				// 走行区間
	gLog[8]  = gRunManager->dDist;				// 走行区間
	gLog[9]  = gRunManager->dLine;				// 走行区間
	gLog[10] = gGyroSensor.getAnglerVelocity();	// ジャイロ値

    gLogger->sendLog(gLog,LOG_NUM);				// BlueToothを使ってログをteratermに送る
}
/**
 * リモート周期タスク
 */
void ev3_cyc_remort(intptr_t exinf){
	act_tsk(REMORT_TASK);
}
/**
 * リモートタスク
 */
void remort_task(intptr_t exinf) {
	char debug[50] = {0};
	gRemort->getsr();										// コマンドを入力
	sprintf(debug,"\n\ryou put: %c \n\r",gRemort->c_debug); //
	gUI->putString(debug);									// コマンドを表示
}
/**
 * UIタスク
 */
void ui_task(intptr_t exinf){
	gUI->putString("\n\rMENU\n\r 1.REMOTE START\n\r 2.Start Logger\n\r 3.Remort\n\r 9.Finish\n\r");
    while(1){
		if( gUI->getState() == UI::LOGGER){
			// Logger起動時はMenu画面を飛ばす
		}else{
			//
		}
        gUI->setState( gUI->getNumber() );
        switch( gUI->getState() ){
        case UI::REMOTE_START:
			gLineTracerWithStarter->mStartSignal = true;
            continue;
        case UI::LOGGER:
            gLogger->init();
            gOdmetry->clearLocation();
            ev3_sta_cyc(EV3_CYC_LOGGER);
			// gUI->putString("\n\rtime,x,y,theta,brightness,gyro,forward,turn,deltaTheta,battery,state,TailAngle,Sonar,Zone,dist,Line\n\r");
			gUI->putString("\n\rtime,x,y,theta,deltaTheta,battery,state,sonar,Zone,dist,Line,count,gyro\n\r");
            continue;
        case UI::REMORT_MODE:
        	 ev3_sta_cyc(EV3_CYC_REMORT);
			 gUI->putString("\n\rREMORT MODE\n\r");
			 slp_tsk();
        	continue;
		case UI::FINISH:
			ev3_stp_cyc(EV3_CYC_LOGGER);
            wup_tsk(MAIN_TASK);
            continue;
        default:
            gUI->putString("\n\rImput 1 to 4");
            continue;
        }
    }
}
