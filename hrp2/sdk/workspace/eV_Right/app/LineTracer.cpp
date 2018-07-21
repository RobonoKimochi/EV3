/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"

/**
 * �R���X�g���N�^
 * @param lineMonitor     ���C������
 * @param balancingWalker �|�����s
 */
LineTracer::LineTracer(LineMonitor* lineMonitor,
                       BalancingWalker* balancingWalker,
                       PidController* pidController)
    : debug(0),
      mLineMonitor(lineMonitor),
      mBalancingWalker(balancingWalker),
      mPidController(pidController),
      mIsInitialized(false),
      mIsFinished(false),
      percent(1.0) {
}

/**
 * �f�X�g���N�^
 */
LineTracer::~LineTracer() {
}

/**
 * ���C���g���[�X����
 */
void LineTracer::run() {
    if (mIsInitialized == false) {
        mBalancingWalker->init();
        mIsInitialized = true;
    }
    // �R�[�X�󋵂𔻒f����
    mSection = mRunManager->determineCourse();

        int direction = calcDirection();
    	//���ւ�Pwm�l���}�C�i�X�̎��ɕ������t�ɂ���B��ɃX�^�[�g�p
    	if((mBalancingWalker->getLeftPwm() < 0) && (mBalancingWalker->getRightPwm() < 0)) {
    		direction = -direction / 2;
    		direction = direction * (1.0f - percent);
    		percent = percent * 0.9f;
     	}
    	//direction = 0;
    if( mSection == RunManager::STRAIGHT_ZONE){
        mPidController->setPID(1.0, 0, 10.0);
        mBalancingWalker->setCommand(100, direction);	//�������x�͎b��
        mBalancingWalker->run();
    }else if(mSection == RunManager::CURB_ZONE){
        mPidController->setPID(1.5, 0 ,5);
        mBalancingWalker->setCommand(70, direction);	//�������x�͎b��
        mBalancingWalker->run();
    }else if( mSection == RunManager::FINISHED){
        mIsFinished = true;
    	mLineMonitor->LineThresholdGray();
        mBalancingWalker->setCommand(20, direction);	//�������x�͎b��
        mBalancingWalker->run();
    }else if( mSection == RunManager::GRAY){
    //    while(1);
        mBalancingWalker->setCommand(20, direction);	//�������x�͎b��
        mBalancingWalker->run();
    }
    // �A�C�f�A�A��LineTracer���ŃX�^�[�g����Ԍɂ܂Łi�\�[�X�Ǘ������?�j
    // }else if(mSection == RunManager::LOOKUPGATE){
    //     // ���b�N�A�b�v�Q�[�g
    // }else if(mSection == RunManager::STAIRS){
    //     // �K�i
    // }else if(mSection == RunManager::GARAGE){
    //     // �Ԍɓ���
    // }
}

/**
 * ���C���g���[�X����
 * (�o�����X��������Ȃ��j
 */
void LineTracer::runWithoutBalancing() {
    if (mIsInitialized == false) {
        mBalancingWalker->init();
        mIsInitialized = true;
    }
    // �R�[�X�󋵂𔻒f����
//    mSection = mRunManager->determineCourse();
//    if( mSection == RunManager::LINETRACE){
        int direction = calcDirection();
        mBalancingWalker->setCommand(50, direction);

        mBalancingWalker->runWithoutBalancing();
//    }else if( mSection == RunManager::FINISHED){
        mIsFinished = true;
//    }
    // �A�C�f�A�A��LineTracer���ŃX�^�[�g����Ԍɂ܂Łi�\�[�X�Ǘ������?�j
    // }else if(mSection == RunManager::LOOKUPGATE){
    //     // ���b�N�A�b�v�Q�[�g
    // }else if(mSection == RunManager::STAIRS){
    //     // �K�i
    // }else if(mSection == RunManager::GARAGE){
    //     // �Ԍɓ���
    // }
}

bool LineTracer::detectGray() {
	mLineMonitor->LineThresholdGray();
	mRightWheel.setPWM(10 - mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
	mLeftWheel.setPWM(10 + mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
    if( mRunManager->detectGray() == true){
        return true;
    }else{
        return false;
    }
}
/**
 * ���s�̂̌������v�Z����
 * @param isOnLine true:���C����/false:���C���O
 * @retval 30  ���C����ɂ���ꍇ(�E����w��)
 * @retval -30 ���C���O�ɂ���ꍇ(������w��)
 */
int LineTracer::calcDirection() {
    // ��PID���s
   return mPidController->calControlledVariable(mLineMonitor->getDeviation());
}
/**
 * ���s��Ԃ��I���������߂�l�ŕԂ�
 */
bool LineTracer::isFinished() {
    if( mIsFinished == true){
        return true;
    }else{
        return false;
    }
}
/**
 * PID�̃p�����[�^��ݒ肷��
 * @param kp P����l
 * @param ki I����l
 * @param kd D����l
 */
// void LineTracer::setPID(float kp, float ki, float kd) {
//     mPidController->setPID(kp,ki,kd);
//     // return;
// }
