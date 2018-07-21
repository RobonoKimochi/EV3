#ifndef EV3_UNIT_REMORT_H_
#define EV3_UNIT_REMORT_H_
#include "ev3api.h"
#include "BlueTooth.h"
#include "BalancingWalker.h"

class Remort : public BlueTooth
{
public:
	Remort(BalancingWalker* balancingwalker);
	void RemortControl();
	void OpenSerialPort();
	bool RemortState();
	bool bt_cmdflg;
	void setState();
	void getsr();

    FILE *mBT;
	uint8_t c_debug; // !! �f�o�b�N�p;
private:
    // FILE *mBT;
	void chgSpeed();
	void setSpeed();
	BalancingWalker* mbalancingwalker;
	uint8_t c;
	int32_t   bt_cmd;      /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
	int32_t   bt_cmdfor;      /* Bluetooth�R�}���h 2:�����[�g�X�^�[�g */
	int32_t   bt_cmdturn;      /* Bluetooth�R�}���h 3:�����[�g�X�^�[�g */
	int32_t   bt_cmdend;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
	int32_t   bt_cmdtail;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
	int32_t   bt_cmdspd;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
	int32_t   forward;
	int32_t   turn;
};
#endif  // EV3_UNIT_BLUETOOTH_H_
