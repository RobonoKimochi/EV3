#include "Garage.h"


/**
 * �R���X�g���N�^
 */
Garage::Garage(	BalancingWalker* balancingWalker,
				LineTracer* lineTracer,
				PidController* pidController,
				LineMonitor* lineMonitor)
    :   mBalancingWalker( balancingWalker ),
    	mLineTracer( lineTracer ),
    	mPidController( pidController ),
    	mLineMonitor( lineMonitor ),
    	mTailAngle( 80 ),			// �K���ڕW�p�x80��
    	mForward( 5 ),				// PID����l
    	mGaragePoint( 40 ),			// ��~������n�_�܂ł̋���
    	mAdjustSpeed( -1 ),			// �������̈ړ����x
    	mState( INIT )				// �������
    	{
}

/**
* @brief 			��ԂɑΉ��������������s�B
*
* @param			����
*
* @return 			����
*
* @detail 			��Ԃɉ����������֐����Ăяo���B
*/
void Garage::StateGarage()
{
	// ��Ԃɉ��������������s
	switch( mState ){
	case INIT:
		this->Initialize();			// ����������
		break;
	case WALKING:
		this->GoToGarage();			// �K���[�W���̒�~�ʒu�܂ő��s�����֐�
		break;
	case ADJUST:
		this->Adjust();				// �����~����
		break;
	case STOP:
		this->Stop();				// ��~����
	default:
		break;
	}
}

/**
* @brief 			����������
*
* @param			����
*
* @return 			����
*
* @detail 			�K���[�W�����J�n����RunManager�̌��_�Ƃ��A
*					�K���[�W���̒�~�ʒu�܂ő��s��ԑJ��
*/
void Garage::Initialize()
{
	mRunManager->setOrigin();					// ���_��ݒ�i��~�n�_�܂ł̋������f�Ɏg�p�j

	mLineMonitor->setLineThreshold( 10 );

	mPidController->setPID( 2, 0, 5 );

	mState = WALKING;							// ��ԑJ�ځF�K���[�W�܂Ői�s
}

/**
* @brief 			�K���[�W���̒�~�ʒu�܂ő��s����
*
* @param			����
*
* @return 			����
*
* @detail 			��~�n�_�imGaragePoint�j�܂Ń��C���g���[�X����Ői�s�B
*					��~�n�_���B��A��~�O���������ɑJ�ځB
*/
void Garage::GoToGarage()
{
	if( mRunManager->getDistanceFromOrigin() <= mGaragePoint )	// ��~�n�_�܂Ői��łȂ��H
	{
		// ���ߑł��̒l�i���F0�A���F10�A�g���[�X�l�F5�j��PID�����������
		// �E�ԗւɂ�mForward(���ߑł��̑O�i�l�j���猻�݂̃J���[�Z���T�l�ƃ��C��臒l�̍������������l�B
		// ���ԗւɂ�mForward�i���ߑł��̑O�i�l�j���猻�݂̃J���[�Z���T�l�ƃ��C��臒l�̍������������l�B
		mRightWheel.setPWM( mForward - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
		mLeftWheel.setPWM( mForward + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
	}
	else	// ��~�����܂Œʉ�
	{
		mState = ADJUST;			// ��ԑJ�ځF��~��������
	}
}

/**
* @brief 			��~�O��������
*
* @param			����
*
* @return 			����
*
* @detail 			�}��~���̓]�|��h�~���邽�߂̒����������s���B
*/
void Garage::Adjust()
{
	// �}��~���̑O���]�|��h�~�̂��߁A����Ɏԗւ��񂷁B
	mRightWheel.setPWM( mAdjustSpeed );		// �E�ԗ֌�ޕ����ɉ�
	mLeftWheel.setPWM( mAdjustSpeed );		// ���ԗ֌�ޕ����ɉ�

	// �ԗւ��}��~�����Ȃ����߃u���[�L����
	mRightWheel.setBrake( false );	// �E�ԗփu���[�L���
	mLeftWheel.setBrake( false );	// ���ԗփu���[�L���

//	mTailMotor->setAngle( mTailMotor->getAngle() + 0 );	// ���s�̂̌X�����ɂ₩�ɂ���

	mState = STOP;					// ��ԑJ�ځF��~
}

/**
* @brief 			��~����
*
* @param			����
*
* @return 			����
*
* @detail 			�K���[�W��~��̏����B
*/
void Garage::Stop()
{
	mRightWheel.setPWM( 0 );		// �E�ԗ֒�~
	mLeftWheel.setPWM( 0 );			// ���ԗ֒�~

	mRightWheel.setBrake( true );	// �E�ԗփu���[�L���
	mLeftWheel.setBrake( true );	// ���ԗփu���[�L���
}
