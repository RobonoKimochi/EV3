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
	uint8_t c_debug; // !! デバック用;
private:
    // FILE *mBT;
	void chgSpeed();
	void setSpeed();
	BalancingWalker* mbalancingwalker;
	uint8_t c;
	int32_t   bt_cmd;      /* Bluetoothコマンド 1:リモートスタート */
	int32_t   bt_cmdfor;      /* Bluetoothコマンド 2:リモートスタート */
	int32_t   bt_cmdturn;      /* Bluetoothコマンド 3:リモートスタート */
	int32_t   bt_cmdend;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   bt_cmdtail;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   bt_cmdspd;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   forward;
	int32_t   turn;
};
#endif  // EV3_UNIT_BLUETOOTH_H_
