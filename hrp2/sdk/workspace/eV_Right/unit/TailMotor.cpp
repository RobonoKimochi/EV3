#include "TailMotor.h"

/**
 * �R���X�g���N�^
 */

TailMotor::TailMotor()
	: //mTailMotor(tailMotor),
	  mAngle(0),
	  mOffset(0),
	  count(0),
	  mPWM(0),
	  mP_Gain(0.50F),
	  PWM_MAX(60),
	  preTargetAngle(0),
	  TimeCount(0),
      TargetAngle(0)
	  {
}

/**
 * @brief 			�e�[�����[�^�̏���������
 *
 * @param [IN]		offset�F�e�[�����[�^�I�t�Z�b�g�l(�����l)
 *
 * @return 			����
 *
 * @detail 			�e�[�����[�^�̃I�t�Z�b�g�l�̐ݒ�A����̏������������s���B
 */
void TailMotor::init( int32_t offset )
{
	mTailMotor.reset();	// ���[�^���Z�b�g
	mOffset = offset;	// �e�[�����[�^�̃I�t�Z�b�g�l�ݒ�

	return;				// �y�߂�l�z����
}

/**
 * @brief 			�e�[�����[�^��ᐧ��Q�C���̐ݒ�
 *
 * @param [IN]		p_gain�F�e�[�����[�^��ᐧ��Q�C���ݒ�l
 *
 * @return 			����
 *
 * @detail 			�e�[�����[�^����̔��l�̐ݒ���s���B
 */
void TailMotor::setPgain( float p_gain )
{
	mP_Gain = p_gain;	// �e�[�����[�^�̃I�t�Z�b�g�l�ݒ�

	return;				// �y�߂�l�z����
}

/**
 * @brief			�e�[�����[�^�̐���
 *
 * @param angle		�e�[���̖ڕW�p�x
 *
 * @return			����
 *
 * @detail			�����uangle�v����e�C�����[�^�̖ڕW�p�x��ݒ�
 */

void TailMotor::setAngle( int32_t angle )
{
	mAngle = angle;
	return;	// �y�߂�l�z����
}

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

void TailMotor::moveTail()
{
	mPWM = (float)(( mAngle + mOffset - mTailMotor.getCount() ) * mP_Gain );	// ��ᐧ��
	
	// PWM�o�͖O�a����
	if( mPWM > PWM_MAX )		// ����PWM��΍ő�l���傫���H
	{
		mPWM = PWM_MAX;			// �ő�l�ݒ�(�ő�l�𒴂������Ȃ�)
	}
	else if( mPWM < -PWM_MAX )	// ����PWM��΍ő�l��菬�����H
	{
		mPWM = -PWM_MAX;		// �ŏ��l�ݒ�(�ŏ��l�����ɂ����Ȃ�)
	}

	mTailMotor.setPWM( mPWM );	// �e�C�����[�^��PWM����l��ݒ�

	return;	// �y�߂�l�z����
}

void TailMotor::setTargetAngle(int target)
{
	
	TargetAngle = target;
	
	return;
}

void TailMotor::setAngleSlow()
{
	if(preTargetAngle != TargetAngle) {
		SetAngle = mTailMotor.getCount();
	}
	if (SetAngle > TargetAngle){
		
    	if (TimeCount > 29){
    		SetAngle--;
    		TimeCount = 0;
    	}
    	TimeCount++;
	} else if( SetAngle < TargetAngle) {
		
		if (TimeCount > 29){
    		SetAngle++;
    		TimeCount = 0;
    	}
    	TimeCount++;
	}
	mAngle = SetAngle;
	preTargetAngle = TargetAngle;
#if(0)
	mPWM = (float)(( SetAngle + mOffset - mTailMotor.getCount() ) * mP_Gain );	// ��ᐧ��
	
	// PWM�o�͖O�a����
	if( mPWM > PWM_MAX )		// ����PWM��΍ő�l���傫���H
	{
		mPWM = PWM_MAX;			// �ő�l�ݒ�(�ő�l�𒴂������Ȃ�)
	}
	else if( mPWM < -PWM_MAX )	// ����PWM��΍ő�l��菬�����H
	{
		mPWM = -PWM_MAX;		// �ŏ��l�ݒ�(�ŏ��l�����ɂ����Ȃ�)
	}

	
	mTailMotor.setPWM( mPWM );	// �e�C�����[�^��PWM����l��ݒ�
#endif
	return;	// �y�߂�l�z����
}

float TailMotor::getDiff(){
	
	return(mP_Gain - mTailMotor.getCount()) ;
}


bool TailMotor::Complete(){
	
	if (mPWM < 1.0f) {
		count++;
	} else {
		count = 0;
	}
	
	if (count >20) {
		flag = true;
	} else {
		flag = false;
	}
	return(flag) ;
}

bool TailMotor::CompleteDull(){
	
	if (ABS(TargetAngle - mTailMotor.getCount()) <=  2.0f) {
		count++;
	} else {
		count = 0;
	}
	
	if (count >80) {
		flag = true;
	} else {
		flag = false;
	}
	return(flag) ;
}

/**
 * @brief			�e�[�����[�^�̊p�x�擾
 *
 * @param angle		����
 *
 * @return			�e�|�����[�^�̊p�x
 *
 * @detail			���݂̃e�[�����[�^�̊p�x��߂�l�Ƃ��ĕԂ��B
 */

int32_t TailMotor::getAngle()
{
	return	mTailMotor.getCount();	// �y�߂�l�z�e�[�����[�^�̊p�x
}

void TailMotor::setPWM(int ang)
{
	mTailMotor.setPWM(ang);	// �y�߂�l�z�e�[�����[�^�̊p�x
}

//�ȉ�������
//bool TailMotor::KeepTorque(){
//
//	
//	mTailMotor.setPWM( mPWM );
//	return() ;
//}