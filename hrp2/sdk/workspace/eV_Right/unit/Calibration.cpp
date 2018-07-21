/******************************************************************************
 *  Calibration.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Calibration.h"


// �萔�錾
const int Calibration::NUM_OF_GYRO_CALIBRATION =  255; // ���L�����u���[�V�������K�C���ӏ�
const int Calibration::NUM_OF_BLACK_CALIBRATION = 255; // ���L�����u���[�V�������K�C���ӏ�
const int Calibration::NUM_OF_WHITE_CALIBRATION = 255; // ���L�����u���[�V�������K�C���ӏ�

/**
 * �R���X�g���N�^
 * @param colorSensor �J���[�Z���T
 * @param gyroSensor  �W���C���Z���T
 * @param lineMonitor ���C�����j�^
 */
Calibration::Calibration(const ev3api::ColorSensor& colorSensor,
                  ev3api::GyroSensor& gyroSensor,
                  LineMonitor* lineMonitor)
    : mColorSensor(colorSensor),
      mGyroSensor(gyroSensor),
      mLineMonitor(lineMonitor),
      mIsStartedGyro(false),
      mIsStartedBlack(false),
      mIsStartedWhite(false),
      mCalCount(0),
      mSum(0) {
}

/**
 * �f�X�g���N�^
 */
Calibration::~Calibration() {
}


/**
 * �L�����u���[�V�����ɕK�v�Ȃ��̂����Z�b�g����
 */
void Calibration::init() {

    mGyroSensor.setOffset(0);                           // �W���C���Z���T�I�t�Z�b�g������
}

/**
 * �W���C���Z���T�̃I�t�Z�b�g�l���L�����u���[�V��������
 * ���߂�l��
 *    false: �L�����u���[�V����������
 *    true : �L�����u���[�V��������
 */
bool Calibration::calibrateGyro(bool startTrigger) {

    int16_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mGyroSensor.getAnglerVelocity();

    if (mIsStartedGyro == false) {
        sprintf( buf, "gyro = %03d", sensor);           // �W���C���Z���T�l��\��
        ev3_lcd_draw_string( buf, 0, 50);

        if (startTrigger == true) {

            mIsStartedGyro = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                               // �W���C���Z���T�l��ώZ
        mCalCount++;

        if (mCalCount == NUM_OF_GYRO_CALIBRATION) {     // �K��񐔈ȏ�ώZ

            cal = mSum / NUM_OF_GYRO_CALIBRATION;       // ���ϒl ���L�����u���[�V�������K�C���ӏ�
            mGyroSensor.setOffset(cal);

            sprintf( buf, "gyroOffset = %03d", cal);    // �W���C���I�t�Z�b�g�l��\��
            ev3_lcd_draw_string( buf, 0, 50);

            finish = true;                              // ����
        }
    }
    return finish;
}

/**
 * ���F��臒l���L�����u���[�V��������
 * ���߂�l��
 *    false: �L�����u���[�V����������
 *    true : �L�����u���[�V��������
 */
bool Calibration::calibrateBlack(bool startTrigger) {

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedBlack == false) {

        sprintf( buf, "black = %03d", sensor);          // ���Z���T�l��\��
        ev3_lcd_draw_string( buf, 0, 70);

        if (startTrigger == true) {

            mIsStartedBlack = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                                 // ���Z���T�l��ώZ
        mCalCount++;

        if (mCalCount == NUM_OF_BLACK_CALIBRATION) {    // �K��񐔈ȏ�ώZ

            cal = mSum / NUM_OF_BLACK_CALIBRATION;                                  // ���ϒl ���L�����u���[�V�������K�C���ӏ�
            mLineMonitor->setBlackThreshold(cal);

            sprintf( buf, "blackTh = %03d", cal);       // ���������l��\��
            ev3_lcd_draw_string( buf, 0, 70);

            finish = true;                              // ����
        }
    }
    return finish;
}

/**
 * ���F��臒l���L�����u���[�V��������
 * ���߂�l��
 *    false: �L�����u���[�V����������
 *    true : �L�����u���[�V��������
 */
bool Calibration::calibrateWhite(bool startTrigger) {

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedWhite == false) {

        sprintf( buf, "white = %03d", sensor);          // ���Z���T�l��\��
        ev3_lcd_draw_string( buf, 0, 90);

        if (startTrigger == true) {

            mIsStartedWhite = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                                 // ���Z���T�l��ώZ
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) {    // �K��񐔈ȏ�ώZ

            cal = mSum / NUM_OF_WHITE_CALIBRATION;                                  // ���ϒl ���L�����u���[�V�������K�C���ӏ�
            mLineMonitor->setWhiteThreshold(cal);

            sprintf( buf, "whiteTh = %03d", cal);       // ���������l��\��
            ev3_lcd_draw_string( buf, 0, 90);

            finish = true;                              // ����
        }
    }
    return finish;
}

/**
 * ���C���g���[�X�������l��ݒ�
 */
void Calibration::calibrateLineThreshold() {
    mLineMonitor->calLineThreshold();
}
