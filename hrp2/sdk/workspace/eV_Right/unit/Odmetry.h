#ifndef EV3_UNIT_ODMETRY_H_
#define EV3_UNIT_ODMETRY_H_

// #include "RotaryEnc.h"
#include "Mat.h"
#include "MotorDriver.h"
#include "Filter.h"

#define GEAR 360.
#define LEFT_WHEEL  4.05F//8.1F // ���ԗ֒��a [cm]
#define RIGHT_WHEEL 4.05F//8.1F // �E�ԗ֒��a [cm]
#define TREAD       16.00F////13.26F // �g���b�h�i�ԗ֊ԋ����j [cm]
#define SAMP_RATE   0.004F  // �T���v�����O���[�g [s]

class Odmetry {
public:
    static Odmetry *getInstance(){
        static Odmetry odmetry;
        return &odmetry;
    }
    float getX();                                   // X���W���擾����
    float getY();                                   // Y���W���擾����
    float getTheta();                               // �p�x�����擾����
    void updateLocation();                          // �ʒu�����X�V����
    void clearLocation();                           // �ʒu�����N���A����
    float getDeltaTheta();                          // �p�x�̎��ԕω����iLPF�Ńt�B���^�ς݁j
private:
    Odmetry();
    // ~Odmetry(){};
    float calcLeftAngVel();                         // ���ԗւ̊p���x���v�Z����
    float calcRightAngVel();                        // �E�ԗւ̊p���x���v�Z����
private:
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    Mat mMat;
    float mLeftAngVel;
    float mRightAngVel;
    int32_t mLeftEnc;
    int32_t mRightEnc;
    float mVelocity;        // ���{�b�g�̑��x
    float mAngleVelocity;   // ���{�b�g�̊p���x
    float mX;
    float mY;
    float mTheta;
    float mDeltaTheta;
    Filter *mFirstFilter;
    Filter *mSecondFilter;
};

#endif  // EV3_UNIT_ODMETRY_H_
