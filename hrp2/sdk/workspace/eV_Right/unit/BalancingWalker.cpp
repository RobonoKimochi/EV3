/******************************************************************************
 *  BalancingWalker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class BalancingWalker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "BalancingWalker.h"

// �萔�錾
const int BalancingWalker::LOW    = 30;    // �ᑬ
const int BalancingWalker::NORMAL = 50;    // �ʏ�
const int BalancingWalker::HIGH   = 70;    // ����

/**
 * �R���X�g���N�^
 * @param gyroSensor �W���C���Z���T
 * @param leftWheel  �����[�^
 * @param rightWheel �E���[�^
 * @param balancer   �o�����T
 */
BalancingWalker::BalancingWalker(const ev3api::GyroSensor& gyroSensor,
                                 ev3api::Motor& leftWheel,
                                 ev3api::Motor& rightWheel,
                                 Balancer* balancer)
    : mGyroSensor(gyroSensor),
      mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mBalancer(balancer),
      mForward(0),
      mTurn(0),
      mTimerFallDown(0),
      back_count(0){
}

/**
 * �f�X�g���N�^
 */
BalancingWalker::~BalancingWalker() {
}

/**
 * �o�����X���s����
 */
void BalancingWalker::run() {
    int16_t angle = mGyroSensor.getAnglerVelocity();  // �W���C���Z���T�l
    int rightWheelEnc = mRightWheel.getCount();       // �E���[�^��]�p�x
    int leftWheelEnc  = mLeftWheel.getCount();        // �����[�^��]�p�x

    mBalancer->setCommand(mForward, mTurn);

    int battery = ev3_battery_voltage_mV();
    mBalancer->update(angle, rightWheelEnc, leftWheelEnc, battery);

    // ���E���[�^�ɉ�]���w������
    if (detectFall() == false) {
        mLeftWheel.setPWM(mBalancer->getPwmLeft());
        mRightWheel.setPWM(mBalancer->getPwmRight());
    }
    else {
        mLeftWheel.setPWM(0);
        mRightWheel.setPWM(0);
    }
}

/**
 * ���s����
 * (�o�����X��������Ȃ��j
 */
void BalancingWalker::runWithoutBalancing() {
    int rightWheelEnc = mRightWheel.getCount();       // �E���[�^��]�p�x
    int leftWheelEnc  = mLeftWheel.getCount();        // �����[�^��]�p�x

    mBalancer->setCommand(mForward, mTurn);

    int battery = ev3_battery_voltage_mV();
    mBalancer->update(0, rightWheelEnc, leftWheelEnc, battery);

    // ���E���[�^�ɉ�]���w������
    if (detectFall() == false) {
        mLeftWheel.setPWM(mBalancer->getPwmLeft());
        mRightWheel.setPWM(mBalancer->getPwmRight());
    }
    else {
        mLeftWheel.setPWM(0);
        mRightWheel.setPWM(0);
    }
}

/**
 * �o�����X���s�ɕK�v�Ȃ��̂����Z�b�g����
 */
void BalancingWalker::init() {
    int offset = 0;

    // ���[�^�G���R�[�_�����Z�b�g����
    mLeftWheel.reset();
    mRightWheel.reset();

    // �|���U�q���䏉����
    mBalancer->init(offset);
}

/**
 * PWM�l��ݒ肷��
 * @param forward �O�i�l
 * @param turn    ����l
 */
void BalancingWalker::setCommand(int forward, int turn) {
    mForward = forward;
    mTurn    = turn;
}

/**
 * ���s�̂̃t�F�[����ԂƂ��āA�]�|�������ǂ����𔻒f����
 * ���t�F�[���Z�[�t���K�C���ӏ�
 */
bool BalancingWalker::detectFall()
{
#define KtimeFallDown   (250)

    if (   (mBalancer->getPwmLeft() ) * (mBalancer->getPwmRight() ) >= 10000)  {

        mTimerFallDown++;
    }
    else {
        // mTimerFallDown = 0;
        if( mTimerFallDown <= KtimeFallDown){
            mTimerFallDown = 0;
        }
    }

    if (mTimerFallDown > KtimeFallDown) {
        return true;
    }
    else {
        return false;
    }
}

int BalancingWalker::SettleMode(){
	
	int16_t angle = mGyroSensor.getAnglerVelocity();  // �W���C���Z���T�l
    int rightWheelEnc = mRightWheel.getCount();       // �E���[�^��]�p�x
    int leftWheelEnc  = mLeftWheel.getCount();        // �����[�^��]�p�x

    mBalancer->setCommand(mForward, mTurn);

    int battery = ev3_battery_voltage_mV();
    mBalancer->update(angle, rightWheelEnc, leftWheelEnc, battery);

    // ���E���[�^�ɉ�]���w������
    if (detectFall() == false) {
    	
    	if(mBalancer->getPwmLeft() < 0){
    		mLeftWheel.setPWM(0);
    		back_count++;
    	} else {
    		back_count = 0;
    		mLeftWheel.setPWM(mBalancer->getPwmLeft());
    	}
    	
    	if(mBalancer->getPwmRight() < 0){
    		 mRightWheel.setPWM(0);
    	} else {
    		mRightWheel.setPWM(mBalancer->getPwmRight());
    	}
    	
        
    } else {
        mLeftWheel.setPWM(0);
        mRightWheel.setPWM(0);
    }
	return (back_count);
}


/**
 * �W���C���l���w�肵�ăo�����X������s��
 */
void BalancingWalker::GyroChange( int16_t angle ) {
    int rightWheelEnc = mRightWheel.getCount();       // �E���[�^��]�p�x
    int leftWheelEnc  = mLeftWheel.getCount();        // �����[�^��]�p�x

    mBalancer->setCommand(mForward, mTurn);

    int battery = ev3_battery_voltage_mV();
    mBalancer->update(angle, rightWheelEnc, leftWheelEnc, battery);

    // ���E���[�^�ɉ�]���w������
    if (detectFall() == false) {
        mLeftWheel.setPWM(mBalancer->getPwmLeft());
        mRightWheel.setPWM(mBalancer->getPwmRight());
    }
    else {
        mLeftWheel.setPWM(0);
        mRightWheel.setPWM(0);
    }
}
 	
int BalancingWalker::getLeftPwm() {
	return (mBalancer->getPwmLeft());
}

int BalancingWalker::getRightPwm() {
	return (mBalancer->getPwmRight());
 		
}