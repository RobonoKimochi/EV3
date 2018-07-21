#include "Remort.h"
/**
 * コンストラクタ
 */
 Remort::Remort(BalancingWalker* balancingwalker)
    //  :  mBT(fp)
:bt_cmdflg(false),
mbalancingwalker(balancingwalker),
bt_cmd(10),
bt_cmdfor(0),
bt_cmdturn(0),
bt_cmdend(0),
bt_cmdtail(0),
bt_cmdspd(4)

        {
            // FILE *mBT;
            OpenSerialPort();
 }

void Remort::OpenSerialPort(){
   mBT = ev3_serial_open_file(EV3_SERIAL_BT);
}

void Remort::getsr(){
		c = getChar();
        c_debug = c; // デバック用
	chgSpeed();
}


void Remort::RemortControl(){

	mbalancingwalker->run();
	setSpeed();
	mbalancingwalker->setCommand(forward,turn);
}

void Remort::chgSpeed(){

		switch(c)
        {
        case '0':
            bt_cmd = 1;
            break;
        default:
            break;
        }

        switch(c)
        {
        case '0':
            bt_cmdend++;
        	c = 10;
            break;
        default:
            break;
        }


        switch(c)
        {
        case '5':
            bt_cmdfor = 5;
            break;

        case '8':
            bt_cmdfor = 8;
            break;

        case '2':
            bt_cmdfor = 2;
            break;

        case '4':
            bt_cmdfor = 4;
            break;

        case '6':
            bt_cmdfor = 6;
            break;

        case 'a':
            bt_cmdfor = 20;
            break;

        case 's':
            bt_cmdfor = 21;
            break;
        default:
            break;
        }

    	switch(c)
        {
        case '3':
            bt_cmdtail = 3;
            break;

        case '9':
            bt_cmdtail = 9;
            break;
        default:
            break;
        }

    	switch(c)
        {
        case '1':
        	if(bt_cmdspd >1){
            bt_cmdspd--;
        	}
        	c = 10;
            break;

        case '7':
        	if(bt_cmdspd<10){
            bt_cmdspd++;
        	}
        	c = 10;
            break;
        default:
            break;
        }

    	switch(c)
    	{
        case 'b':
    		if(bt_cmdflg == false){
    			bt_cmdflg = true;
    			c = 10;
    			ev3_led_set_color(LED_ORANGE); /*デバッグ用にLEDを光らせる*/
    		}else{
    			bt_cmdflg = false;       // if bt_cmdflg == true
    			c = 10;
    			ev3_led_set_color(LED_GREEN); /*デバッグ用にLEDを光らせる*/
    		}
    		break;

    	default:
    		break;
    	}

}

void Remort::setSpeed(){


       		if (bt_cmdfor == 5){
   				forward = 0;
   				turn = 0;
   			}else if(bt_cmdfor == 8){
   				forward = bt_cmdspd*10;
   				turn = 0;
   			}else if(bt_cmdfor == 2){
   				forward = -bt_cmdspd*10;
   				turn = 0;
   			}else if(bt_cmdfor == 4){
   				turn = -20;
   				forward = 0;
   			}else if(bt_cmdfor == 6){
   				turn = 20;
   				forward = 0;
   			}else if(bt_cmdfor == 20){
   				turn = -20;
   			}else if(bt_cmdfor == 21){
   				turn = 20;
   			}else{
   				forward = 0;
   				turn = 0;
   			}

}


bool Remort::RemortState(){

		if (bt_cmdflg == true)
		{
			return (true);
		} else {
			return (false);
		}

}

void Remort::setState(){

	bt_cmdflg = true;
	return;
}
