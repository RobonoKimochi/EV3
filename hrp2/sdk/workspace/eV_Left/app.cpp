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
// �f�X�g���N�^�����̉���
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

// Motor�֘A
ev3api::Motor gLeftWheel = MotorDriver::getInstance().getLeftWheel();
ev3api::Motor gRightWheel = MotorDriver::getInstance().getRightWheel();
// Sensor�֘A
ev3api::TouchSensor gTouchSensor = SensorDriver::getInstance().getTouchSensor();
ev3api::ColorSensor gColorSensor = SensorDriver::getInstance().getColorSensor();
ev3api::SonarSensor gSonarSensor = SensorDriver::getInstance().getSonarSensor();
ev3api::GyroSensor gGyroSensor = SensorDriver::getInstance().getGyroSensor();

// �I�u�W�F�N�g�̒��`
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
static Run_Stairs		*gRun_Stairs ;

/**
 * EV3�V�X�e������
 */
static void user_system_create() {
	tslp_tsk(2);
    // �I�u�W�F�N�g�̍쐬
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
	gRun_Stairs		 = new Run_Stairs( gBalancingWalker, gPidController, gLineMonitor);
    gLineTracerWithStarter = new LineTracerWithStarter(gLineTracer, gStarter, gCalibration, gRemort, gLookUpGate , gMeasureDistance, gGarage, gBalancingWalker, gRun_Stairs);
    gUI             = new UI();
    gLogger         = new Logger();
	gOdmetry		= Odmetry::getInstance();
	gRunManager		= RunManager::getInstance();
	gTailMotor		= TailMotor::getInstance();
	gSound 			= Sound::getInstance();

    // �����������ʒm
    ev3_led_set_color(LED_ORANGE);
	gSound->ready();
}

/**
 * EV3�V�X�e���j��
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
 * ���C���^�X�N
 */
void main_task(intptr_t unused) {
    user_system_create();  // �Z���T�⃂�[�^�̏���������

	// �g���[�T�^�X�N�J�n
	act_tsk(TRACER_TASK);

    // �����n���h���J�n
    // ev3_sta_cyc(EV3_CYC_TRACER);
	ev3_sta_cyc(EV3_CYC_ODMETRY);
	ev3_sta_cyc(EV3_CYC_TAILMOTOR);

	// UI�^�X�N�J�n
	act_tsk(UI_TASK);


    slp_tsk();  // �o�b�N�{�^�������������܂ő҂�

    // �����n���h�����~
    // ev3_stp_cyc(EV3_CYC_TRACER);
	ter_tsk(TRACER_TASK);

    user_system_destroy();  // �I������

    ext_tsk();
}
/**
 * ���C���g���[�X�����^�X�N
 */
// void ev3_cyc_tracer(intptr_t exinf) {
//     act_tsk(TRACER_TASK);
// }
/**
 * ���C���g���[�X�^�X�N
 */
void tracer_task(intptr_t exinf) {
	while(1){
	    if (ev3_button_is_pressed(BACK_BUTTON)) {
	        wup_tsk(MAIN_TASK);  // �o�b�N�{�^������
	    } else {
	        gLineTracerWithStarter->run();  // (�|��)���s
	    }
		tslp_tsk(4);
	}
    // ext_tsk();
}
/**
 * �e�[�����[�^�����^�X�N
 */
void ev3_cyc_tailmotor(intptr_t exinf) {
	act_tsk(TAILMOTOR_TASK);
}
/**
 * �e�[�����[�^�^�X�N
 */
void tailmotor_task(intptr_t exinf) {
	if(gLineTracerWithStarter->TailInit == true) {
	gTailMotor->moveTail();	// �K������
	}
	ext_tsk();
}
/**
 * �I�h���g�������^�X�N
 */
void ev3_cyc_odmetry(intptr_t exinf) {
	act_tsk(ODMETRY_TASK);
}
/**
 * �I�h���g���^�X�N
 */
void odmetry_task(intptr_t exinf) {
    gOdmetry->updateLocation();  // �|�����s
    ext_tsk();
}
/**
 * ���K�[�����^�X�N
 */
void ev3_cyc_logger(intptr_t exinf) {
    act_tsk(LOGGER_TASK);
}
/**
 * ���K�[�^�X�N
 */
#define LOG_NUM 12
void logger_task(intptr_t exinf){
	float gLog[LOG_NUM];
	// gLog[0]  = gOdmetry->getX();					// X���W
	// gLog[1]  = gOdmetry->getY();					// Y���W
	// gLog[2]  = gOdmetry->getTheta();				// �p�x
	// gLog[3]  = gColorSensor.getBrightness();		// �J���[�Z���T�l
	// gLog[4]  = gGyroSensor.getAnglerVelocity();	// �W���C���l
	// gLog[5]  = gBalancer->getForward();			// Forward�l
	// gLog[6]  = gBalancer->getTurn();				// Turn�l
	// gLog[7]  = gOdmetry->getDeltaTheta();		// �p�x�̎��ԕω��i�t�B���^�ς݁j
	// gLog[8]  = ev3_battery_voltage_mV();			// �o�b�e���[
	// gLog[9]  = gLineTracerWithStarter->debug;	// mState
	// gLog[10] = gTailMotor->mAngle;				// �e�[�����[�^�̃A���O��
	// gLog[11] = gSonarSensor.getDistance();		// �����Z���T
	// gLog[12] = gRunManager->mZone;				// ���s����
	// gLog[13] = gRunManager->dDist;				// ���s����
	// gLog[14] = gRunManager->dLine;				// ���s����
	// gLog[15] = gRunManager->dCount;				// ���s����(kaunto)

	gLog[0]  = gOdmetry->getX();					// X���W
	gLog[1]  = gOdmetry->getY();					// Y���W
	gLog[2]  = gOdmetry->getTheta();				// �p�x
	gLog[3]  = gOdmetry->getDeltaTheta();		// �p�x�̎��ԕω��i�t�B���^�ς݁j
	gLog[4]  = ev3_battery_voltage_mV();			// �o�b�e���[
	gLog[5]  = gLineTracerWithStarter->debug;	// mState
	gLog[6]  = gSonarSensor.getDistance();		// �����Z���T
	gLog[7]  = gRunManager->mZone;				// ���s����
	gLog[8]  = gRunManager->dDist;				// ���s����
	gLog[9]  = gRunManager->dLine;				// ���s����
	// gLog[10] = gRunManager->dCount;				// ���s����(kaunto)
	gLog[10] = gGyroSensor.getAnglerVelocity();	// �W���C���l
	gLog[11] = gColorSensor.getBrightness();

    gLogger->sendLog(gLog,LOG_NUM);				// BlueTooth���g���ă��O��teraterm�ɑ���
}
/**
 * �����[�g�����^�X�N
 */
void ev3_cyc_remort(intptr_t exinf){
	act_tsk(REMORT_TASK);
}
/**
 * �����[�g�^�X�N
 */
void remort_task(intptr_t exinf) {
	char debug[50] = {0};
	gRemort->getsr();										// �R�}���h������
	sprintf(debug,"\n\ryou put: %c \n\r",gRemort->c_debug); //
	gUI->putString(debug);									// �R�}���h���\��
}
/**
 * UI�^�X�N
 */
void ui_task(intptr_t exinf){
	gUI->putString("\n\rMENU\n\r 1.REMOTE START\n\r 2.Start Logger\n\r 3.Remort\n\r 9.Finish\n\r");
    while(1){
		if( gUI->getState() == UI::LOGGER){
			// Logger�N������Menu���ʂ����΂�
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
