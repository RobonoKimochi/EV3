#ifndef	EV3_UNIT_TAILMOTOR_H_
#define EV3_UNIT_TAILMOTOR_H_

// #include "Motor.h"
#include "MotorDriver.h"
#include "app.h"

class TailMotor
{
	public:
		/**
		 * �C���X�^���X�̎擾
		 */
		 static TailMotor *getInstance(){
			 static TailMotor tailMotor;
			 return &tailMotor;
		 }
		// ev3api::Motor& mTailMotor;		// ���[�^�[�N���X

		/**
		* @brief 			�e�C�����[�^�̏���������
		*
		* @param [IN]		offset�F�e�C�����[�^�I�t�Z�b�g�l(�����l)
		*
		* @return 			����
		*
		* @detail 			�e�C�����[�^�̃I�t�Z�b�g�l�̐ݒ�A����̏������������s���B
		*/
		void init( int32_t offset );

		/**
 		* @brief 			�e�C�����[�^��ᐧ��Q�C���̐ݒ�
		*
 		* @param [IN]		p_gain�F�e�C�����[�^��ᐧ��Q�C���ݒ�l
 		*
 		* @return 			����
 		*
 		* @detail 			�e�C�����[�^����̔��l�̐ݒ���s���B
 		*/
		void setPgain( float p_gain );

		/**
		* @brief			�e�C�����[�^�̐���
		*
		* @param angle		�e�C���̖ڕW�p�x
		*
		* @return			����
		*
		* @detail			�����uangle�v����e�C�����[�^�̖ڕW�p�x��ݒ�
		*/
		void setAngle( int32_t angle );
		
		/**
		* @brief			�e�C�����[�^�̐���
		*
		* @param 			����
		*
		* @return			����
		*
		* @detail			�e�C�����[�^�ɐݒ肷��PWM��������݂�mAngle����Z�o��
		* 					����PWM�ő�l�ȏ�Ȃ�ő�l�A�ŏ��l�ȉ��Ȃ�ŏ��l�ɏC����
		*					�e�C�����[�^��PWM����l��^����B
		*/
		void moveTail();
		void setTargetAngle(int target);

		void setAngleSlow();	// �ԑ̌X�Ηp
	
		void setPWM(int ang);

	    float getDiff();
	    bool  Complete();
		int32_t  mAngle;				// ���[�^�[�̊p�x(��)
	bool CompleteDull();
	void  setTargetAngle();
	
/**
	* @brief			�e�C�����[�^�̊p�x�擾
	*
	* @param 			����
	*
	* @return			�e�C�����[�^�̊p�x
	*
	* @detail			���݂̃e�C�����[�^�̊p�x��߂�l�Ƃ��ĕԂ��B
	*/
	int32_t getAngle();
	private:
		TailMotor();					// �R���X�g���N�^

		int32_t  mOffset;				// �I�t�Z�b�g�l
	    int      count;
		bool     flag;
		float    mPWM;					// PWM����l
		float    mP_Gain;				// ��ᐧ��Q�C��
		float    PWM_MAX;				// PWM����O�a臒l
	    float    SetAngle;
		float    preTargetAngle;
	    int TimeCount;
		int    TargetAngle;

	    ev3api::Motor mTailMotor = MotorDriver::getInstance().getTailMotor();
};
#endif	// EV3_UNIT_TAILMOTOR_H_
