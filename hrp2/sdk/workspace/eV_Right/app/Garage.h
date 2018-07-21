/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_GARAGE_H_
#define EV3_APP_GARAGE_H_

// unit���̃w�b�_���C���N���[�h
#include "LineTracer.h"
#include "BalancingWalker.h"

#include "MotorDriver.h"
#include "RunManager.h"
#include "TailMotor.h"
#include "Odmetry.h"

//#define DOLEAN
class Garage
{
	public:
		Garage(	BalancingWalker* balancingWalker, // �R���X�g���N�^
				LineTracer* lineTracer,
				PidController* pidController,
				LineMonitor* lineMonitor);

		/**
		* @brief 			��ԂɑΉ��������������s�B
		*
		* @param			����
		*
		* @return 			����
		*
		* @detail 			��Ԃɉ����������֐����Ăяo���B
		*/
		void StateGarage();

		/**
		* @brief 			����������
		*
		* @param			����
		*
		* @return 			����
		*
		* @detail 			�K���[�W�����J�n���̌��_�ݒ肵�A
		*					�K���[�W�܂Ői�s�ɏ�ԑJ��
		*/
		void Initialize();

		/**
		* @brief 			�K���[�W���̒�~�ʒu�܂ő��s����
		*
		* @param			����
		*
		* @return 			����
		*
		* @detail 			�K���[�W�̐K���|���n�_�܂Ń��C���g���[�X����Ői�s�B
		*					�n�_���B��A�X�Β��ɏ�ԑJ�ځB
		*/
		void GoToGarage();

		/**
		* @brief 			��������
		*
		* @param			����
		*
		* @return 			����
		*
		* @detail 			�K���|����Ԃ���A�K���[�W���̒�~�n�_���
		*					��O�Œ�~�����ꍇ�A�������s���B
		*					������A��~�����ɏ�ԑJ�ځB
		*/
		void Adjust();

		/**
		* @brief 			��~����
		*
		* @param			����
		*
		* @return 			����
		*
		* @detail 			���ԗւ��~������B
		*/
		void Stop();			// ��~�����֐�
	private:
		enum State {
        UNDEFINED,
		INIT,		// �������
		WALKING,	// �K���[�W�܂Ői�s���
		ADJUST,		// ����������
		STOP		// ��~���
    	};
    	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
		ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    	TailMotor *mTailMotor = TailMotor::getInstance();
    	RunManager *mRunManager = RunManager::getInstance();
    	Odmetry *mOdmetry = Odmetry::getInstance();
    	BalancingWalker* mBalancingWalker;
		LineTracer*      mLineTracer;
		PidController*   mPidController;
		LineMonitor*     mLineMonitor;
    	int32_t          mTailAngle;		// �e�C���̖ڕW�p�x
    	int32_t          mForward;			// PID����l
    	float            mLeanPoint;		// �K���|���n�_�܂ł̋���
    	float            mGaragePoint;		// ��~����
    	int32_t          mAdjustSpeed;		// �������̈ړ����x
    	State            mState;			// ��ԕϐ�
};
#endif  // EV3_APP_LINETRACER_H_
