#include "RotaryEnc.h"

  /**
   * ���ԗւ̃G���R�[�_�l���擾����
   */
  int32_t RotaryEnc::getLeftEnc(){
      mLeftEnc = mLeftWheel.getCount();// - mLeftEncOffset;
      return mLeftEnc;
  }
  /**
   * �E�ԗւ̃G���R�[�_�l���擾����
   */
  int32_t RotaryEnc::getRightEnc(){
      mRightEnc = mRightWheel.getCount();// - mRightEncOffset;
      return mRightEnc;
  }
