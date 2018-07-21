/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : 2輪倒立振子ライントレースロボットのTOPPERS/HRP2用C++サンプルプログラム
 **
 ** 注記 : sample_cpp (ライントレース/尻尾モータ/超音波センサ/リモートスタート)
 ******************************************************************************
 **/

#include "ev3api.h"
#include "app.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* Bluetooth */
static int32_t   bt_cmd = 10;      /* Bluetoothコマンド 1:リモートスタート */
static int32_t   bt_cmdfor = 0;      /* Bluetoothコマンド 2:リモートスタート */
static int32_t   bt_cmdturn = 0;      /* Bluetoothコマンド 3:リモートスタート */
static int32_t   bt_cmdend = 0;      /* Bluetoothコマンド 4:リモートスタート */
static int32_t   bt_cmdtail = 0;      /* Bluetoothコマンド 4:リモートスタート */
static int32_t   bt_cmdspd = 4;      /* Bluetoothコマンド 4:リモートスタート */
static int32_t   bt_cmdflg = 0;      
static int32_t   lightcount = 0; 


static FILE     *bt = NULL;      /* Bluetoothファイルハンドル */

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define GYRO_OFFSET           0  /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE          40  /* 白色の光センサ値 */
#define LIGHT_BLACK           0  /* 黒色の光センサ値 */
#define SONAR_ALERT_DISTANCE 30  /* 超音波センサによる障害物検知距離[cm] */
#define TAIL_ANGLE_STAND_UP  91  /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
#define P_GAIN             2.5F  /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */
//#define DEVICE_NAME     "ET0"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
//#define PASS_KEY        "1234" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* 関数プロトタイプ宣言 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);

/* オブジェクトへのポインタ定義 */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/* メインタスク */
void main_task(intptr_t unused)
{
    int8_t forward;      /* 前後進命令 */
    int8_t turn;         /* 旋回命令 */
    int8_t pwm_L, pwm_R; /* 左右モータPWM出力 */
	int8_t color_now;
	int8_t diff_tail;
	int8_t diff_tail_sum;
	int8_t tail_count;
    /* 各オブジェクトを生成・初期化する */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_2);
    sonarSensor = new SonarSensor(PORT_3);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();

    /* LCD画面表示 */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("EV3way-ET sample_cpp", 0, CALIB_FONT_HEIGHT*1);

    /* 尻尾モーターのリセット */
    tailMotor->reset();
    
    /* Open Bluetooth file */
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    /* Bluetooth通信タスクの起動 */
    act_tsk(BT_TASK);

    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

    /* スタート待機 */
    while(1)
    {
        tail_control(TAIL_ANGLE_STAND_UP +( 0.1 * diff_tail_sum)); /* 完全停止用角度に制御 */

        if (bt_cmd == 1)
        {
            break; /* リモートスタート */
        }

        if (touchSensor->isPressed())
        {
            break; /* タッチセンサが押された */
        }
    	fprintf(bt,"tailangle%d\n",tailMotor->getCount());   //*******************デバッグ用*******************/
    	fprintf(bt,"tailanglediff%d\n",diff_tail);   //*******************デバッグ用*******************/
    	fprintf(bt,"diff_sum%d\n",diff_tail_sum);   //*******************デバッグ用*******************/
    	fprintf(bt,"tailcount%d\n",tail_count);   //*******************デバッグ用*******************/
    	
    	if ( tail_count > 100 ){
    	diff_tail = (TAIL_ANGLE_STAND_UP - tailMotor->getCount());
    	}
    	
    	if(diff_tail > 0)
    	{
    		diff_tail_sum++;
    	}
    	else if(diff_tail < 0)
    	{
    		diff_tail_sum--;
    	}
    	else if(diff_tail_sum > 0)
    	{
    		diff_tail_sum--;
    	}
    	else if(diff_tail_sum < 0)
    	{
    		diff_tail_sum++;
    	}
    	
    	
    	if (tail_count < 120){
    		tail_count++;
    	}
    	
        clock->sleep(10);
    }

    /* 走行モーターエンコーダーリセット */
    leftMotor->reset();
    rightMotor->reset();
    
    /* ジャイロセンサーリセット */
    gyroSensor->reset();
    balance_init(); /* 倒立振子API初期化 */

    ev3_led_set_color(LED_GREEN); /* スタート通知 */
tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    /**
    * Main loop for the self-balance control algorithm
    */
    while(1)
    {
        int32_t motor_ang_l, motor_ang_r;
        int32_t gyro, volt;

    	
    	color_now = colorSensor->getBrightness();
    	
        if (ev3_button_is_pressed(BACK_BUTTON)) break;
    	if (bt_cmdend >= 2) break;

    	if((color_now <= 2) && (lightcount <= 10))  //カラーセンサの値を利用して着地を判定する
    	{
    		lightcount++;
    	}
    	else if ((color_now >= 5) && (lightcount >= 1))
    	{
    		lightcount--;
    	}
    	
    	if (bt_cmdtail == 3){
    		tail_control(TAIL_ANGLE_DRIVE);
    		ev3_led_set_color(LED_GREEN);
    	}else if(bt_cmdtail == 9){
    		tail_control(50);
    		ev3_led_set_color(LED_RED);
    	}else{
//			tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    	}
    	

				
		if(bt_cmdflg == 0){
			ev3_led_set_color(LED_GREEN); /*デバッグ用にLEDを光らせる*/
				
        	if (color_now <= 25)
           	{
               	turn =  -10; /* 左旋回命令 */
           		forward = 10;
           	}
           	else if(color_now >= 60)
           	{
               	turn = 10; /* 右旋回命令 */
           		forward = 10;
       		}
			else
			{
			forward = 20;
			}
		
		}else{
				
			ev3_led_set_color(LED_ORANGE); /*デバッグ用にLEDを光らせる*/
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
    	
    	if (sonar_alert() == 1) /* 障害物検知 */
        {
        	if (forward >= 0)
        	{
        		forward  = 0; /* 障害物を検知したら停止 */
        	}
        }
    	
  //  	fprintf(bt,"motorangle%d\n",tailMotor->getCount());   //*******************デバッグ用*******************/
	
        /* 倒立振子制御API に渡すパラメータを取得する */
        motor_ang_l = leftMotor->getCount();
        motor_ang_r = rightMotor->getCount();
        gyro = gyroSensor->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();
		
    	
    	if(lightcount <= 4)
    	{
        /* 倒立振子制御APIを呼び出し、倒立走行するための */
        /* 左右モータ出力値を得る */
    		
        balance_control(
            (float)forward,
            (float)turn,
            (float)gyro,
            (float)GYRO_OFFSET,
            (float)motor_ang_l,
            (float)motor_ang_r,
            (float)volt,
            (int8_t *)&pwm_L,
            (int8_t *)&pwm_R);
    	
        leftMotor->setPWM(pwm_L);
        rightMotor->setPWM(pwm_R);
    	}
    	else							//着地ていない場合はリセットする
    	{
//    		tail_control(TAIL_ANGLE_STAND_UP);
    		leftMotor->setPWM(0);
        	rightMotor->setPWM(0);
    		balance_init();
    		leftMotor->reset();
    		rightMotor->reset();
    		gyroSensor->reset();
    	}
        clock->sleep(4); /* 4msec周期起動 */
    }
    leftMotor->reset();
    rightMotor->reset();

    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// 関数名 : sonar_alert
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
static int32_t sonar_alert(void)
{
    static uint32_t counter = 0;
    static int32_t alert = 0;

    int32_t distance;

    if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
    {
        /*
         * 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
         * NXTの場合は、40msec周期程度が経験上の最短測定周期です。
         * EV3の場合は、要確認
         */
        distance = sonarSensor->getDistance();
        if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
        {
            alert = 1; /* 障害物を検知 */
        }
        else
        {
            alert = 0; /* 障害物無し */
        }
        counter = 0;
    }

    return alert;
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
static void tail_control(int32_t angle)
{
    float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; /* 比例制御 */
    /* PWM出力飽和処理 */
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }

    tailMotor->setPWM(pwm);
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while(1)
    {
        uint8_t c = fgetc(bt); /* 受信 */
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
    		if(bt_cmdflg == 0){
    			bt_cmdflg = 1;
    			c = 10;
    		}else{
    			bt_cmdflg = 0;
    			c = 10;
    		}
    		break;
    		
    	default:
    		break;
    	}
    	
    }
}
