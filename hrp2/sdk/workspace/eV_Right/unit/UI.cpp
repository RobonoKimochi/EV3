#include "UI.h"

/**
 * �R���X�g���N�^
 */
UI::UI():
    mState(NOP),
    mDotFlag(false)
{
    OpenSerialPort();
    if( isConnected() == true){
        putString("BlueTooth is connected!\n\r");
    } else{
        // mBlueTooth->putString("conect dekite naiyo\n\r");
    }
}
void UI::setState(uint8_t state){
    switch(state){
    case 1:
        mState = REMOTE_START;
        break;
    case 2:
        mState = LOGGER;
        break;
    case 3:
        mState = REMORT_MODE;
        break;
    case 9:
        mState = FINISH;
        break;
    default:
        mState = NOP;
        break;
    }
}
UI::COMMAND UI::getState(){
    return mState;
}
void UI::raiseDotFlag(){
    mDotFlag = true;
}
void UI::dropDotFlag(){
    mDotFlag = false;
}
bool UI::checkDotFlag(){
    return mDotFlag;
}
float UI::getFloatFromCUI(){
    uint8_t c = 0;
    float fnum = 0;
    float dp = 0.1;                 // dp = decimal point (�����_)
    while(1){
        c = getChar();
        if(c == '\r'){              // Enter�L�[����
            break;
        }
        if(c == '.' && checkDotFlag() == false){              // �h�b�g����
            raiseDotFlag();
            dp = 0.1;
            continue;
        }
        if(c < '0' || c > '9'){     // ����0-9,Enter�ȊO
            putString("\n\r");      // ���s
            fnum = 0;                // num ���N���A
            dropDotFlag();
            dp = 0.1;
            continue;
        }
        if( checkDotFlag() == false ){
            fnum = fnum * 10 + c - '0';
        } else if(dp > DP_LIMIT){
            fnum = fnum + (c - '0') * dp;
            dp /= 10;
        } else{
            putString("\b \b");      // BackSpace
        }
    }
    dropDotFlag();
    return (fnum);
}
int16_t UI::getIntegerFromCUI(){
    uint16_t c = 0;
    uint16_t num = 0;
    bool mflag = false;             // �}�C�i�X�t���O
    while(1){
        c = getChar();
        if(c == '\r'){              // Enter�L�[����
            break;
        }
        if(c== '-' && num == 0 && mflag == false){  // ���̏����ł́A0���͌�̃}�C�i�X(ex. 0-25etc...)�����O�ł��Ȃ�
            mflag = true;
            continue;
        }
        if(c < '0' || c > '9'){     // ����0-9�ȊO
            putString("\n\r");      // ���s
            num = 0;                // num ���N���A
            mflag = false;          // �}�C�i�X�t���O���N���A
            continue;
        }
        num = num * 10 + ( c - '0');
        if( num > 255 || num < -255){
            putString("\n\rBeyond the limit\n\r");  // �͈͊O
        }
    }
    if(mflag == false){
        return (num);
    } else{
        return ( -1 * num);
    }
}
