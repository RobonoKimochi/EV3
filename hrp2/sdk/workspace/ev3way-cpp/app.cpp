/**
 ******************************************************************************
 ** �t�@�C���� : app.cpp
 **
 ** �T�v : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/HRP2�pC++�T���v���v���O����
 **
 ** ���L : sample_cpp (���C���g���[�X/�K�����[�^/�����g�Z���T/�����[�g�X�^�[�g)
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
static int32_t   bt_cmd = 10;      /* Bluetooth�R�}���h 1:�����[�g�X�^�[�g */
static int32_t   bt_cmdfor = 0;      /* Bluetooth�R�}���h 2:�����[�g�X�^�[�g */
static int32_t   bt_cmdturn = 0;      /* Bluetooth�R�}���h 3:�����[�g�X�^�[�g */
static int32_t   bt_cmdend = 0;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
static int32_t   bt_cmdtail = 0;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
static int32_t   bt_cmdspd = 4;      /* Bluetooth�R�}���h 4:�����[�g�X�^�[�g */
static int32_t   bt_cmdflg = 0;      
static int32_t   lightcount = 0; 


static FILE     *bt = NULL;      /* Bluetooth�t�@�C���n���h�� */

/* ���L�̃}�N���͌�/���ɍ��킹�ĕύX����K�v������܂� */
#define GYRO_OFFSET           0  /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define LIGHT_WHITE          40  /* ���F�̌��Z���T�l */
#define LIGHT_BLACK           0  /* ���F�̌��Z���T�l */
#define SONAR_ALERT_DISTANCE 30  /* �����g�Z���T�ɂ���Q�����m����[cm] */
#define TAIL_ANGLE_STAND_UP  91  /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3  /* �o�����X���s���̊p�x[�x] */
#define P_GAIN             2.5F  /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60  /* ���S��~�p���[�^����PWM��΍ő�l */
//#define DEVICE_NAME     "ET0"  /* Bluetooth�� hrp2/target/ev3.h BLUETOOTH_LOCAL_NAME�Őݒ� */
//#define PASS_KEY        "1234" /* �p�X�L�[    hrp2/target/ev3.h BLUETOOTH_PIN_CODE�Őݒ� */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h */

/* LCD�t�H���g�T�C�Y */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* �֐��v���g�^�C�v�錾 */
static int32_t sonar_alert(void);
static void tail_control(int32_t angle);

/* �I�u�W�F�N�g�ւ̃|�C���^��` */
TouchSensor*    touchSensor;
SonarSensor*    sonarSensor;
ColorSensor*    colorSensor;
GyroSensor*     gyroSensor;
Motor*          leftMotor;
Motor*          rightMotor;
Motor*          tailMotor;
Clock*          clock;

/* ���C���^�X�N */
void main_task(intptr_t unused)
{
    int8_t forward;      /* �O��i���� */
    int8_t turn;         /* ���񖽗� */
    int8_t pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int8_t color_now;
	int8_t diff_tail;
	int8_t diff_tail_sum;
	int8_t tail_count;
    /* �e�I�u�W�F�N�g�𐶐��E���������� */
    touchSensor = new TouchSensor(PORT_1);
    colorSensor = new ColorSensor(PORT_2);
    sonarSensor = new SonarSensor(PORT_3);
    gyroSensor  = new GyroSensor(PORT_4);
    leftMotor   = new Motor(PORT_C);
    rightMotor  = new Motor(PORT_B);
    tailMotor   = new Motor(PORT_A);
    clock       = new Clock();

    /* LCD��ʕ\�� */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("EV3way-ET sample_cpp", 0, CALIB_FONT_HEIGHT*1);

    /* �K�����[�^�[�̃��Z�b�g */
    tailMotor->reset();
    
    /* Open Bluetooth file */
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    /* Bluetooth�ʐM�^�X�N�̋N�� */
    act_tsk(BT_TASK);

    ev3_led_set_color(LED_ORANGE); /* �����������ʒm */

    /* �X�^�[�g�ҋ@ */
    while(1)
    {
        tail_control(TAIL_ANGLE_STAND_UP +( 0.1 * diff_tail_sum)); /* ���S��~�p�p�x�ɐ��� */

        if (bt_cmd == 1)
        {
            break; /* �����[�g�X�^�[�g */
        }

        if (touchSensor->isPressed())
        {
            break; /* �^�b�`�Z���T�������ꂽ */
        }
    	fprintf(bt,"tailangle%d\n",tailMotor->getCount());   //*******************�f�o�b�O�p*******************/
    	fprintf(bt,"tailanglediff%d\n",diff_tail);   //*******************�f�o�b�O�p*******************/
    	fprintf(bt,"diff_sum%d\n",diff_tail_sum);   //*******************�f�o�b�O�p*******************/
    	fprintf(bt,"tailcount%d\n",tail_count);   //*******************�f�o�b�O�p*******************/
    	
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

    /* ���s���[�^�[�G���R�[�_�[���Z�b�g */
    leftMotor->reset();
    rightMotor->reset();
    
    /* �W���C���Z���T�[���Z�b�g */
    gyroSensor->reset();
    balance_init(); /* �|���U�qAPI������ */

    ev3_led_set_color(LED_GREEN); /* �X�^�[�g�ʒm */
tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */
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

    	if((color_now <= 2) && (lightcount <= 10))  //�J���[�Z���T�̒l�𗘗p���Ē��n�𔻒肷��
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
//			tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */
    	}
    	

				
		if(bt_cmdflg == 0){
			ev3_led_set_color(LED_GREEN); /*�f�o�b�O�p��LED�����点��*/
				
        	if (color_now <= 25)
           	{
               	turn =  -10; /* �����񖽗� */
           		forward = 10;
           	}
           	else if(color_now >= 60)
           	{
               	turn = 10; /* �E���񖽗� */
           		forward = 10;
       		}
			else
			{
			forward = 20;
			}
		
		}else{
				
			ev3_led_set_color(LED_ORANGE); /*�f�o�b�O�p��LED�����点��*/
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
    	
    	if (sonar_alert() == 1) /* ��Q�����m */
        {
        	if (forward >= 0)
        	{
        		forward  = 0; /* ��Q�������m�������~ */
        	}
        }
    	
  //  	fprintf(bt,"motorangle%d\n",tailMotor->getCount());   //*******************�f�o�b�O�p*******************/
	
        /* �|���U�q����API �ɓn���p�����[�^���擾���� */
        motor_ang_l = leftMotor->getCount();
        motor_ang_r = rightMotor->getCount();
        gyro = gyroSensor->getAnglerVelocity();
        volt = ev3_battery_voltage_mV();
		
    	
    	if(lightcount <= 4)
    	{
        /* �|���U�q����API���Ăяo���A�|�����s���邽�߂� */
        /* ���E���[�^�o�͒l�𓾂� */
    		
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
    	else							//���n�Ă��Ȃ��ꍇ�̓��Z�b�g����
    	{
//    		tail_control(TAIL_ANGLE_STAND_UP);
    		leftMotor->setPWM(0);
        	rightMotor->setPWM(0);
    		balance_init();
    		leftMotor->reset();
    		rightMotor->reset();
    		gyroSensor->reset();
    	}
        clock->sleep(4); /* 4msec�����N�� */
    }
    leftMotor->reset();
    rightMotor->reset();

    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// �֐��� : sonar_alert
// ���� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int32_t sonar_alert(void)
{
    static uint32_t counter = 0;
    static int32_t alert = 0;

    int32_t distance;

    if (++counter == 40/4) /* ��40msec�������ɏ�Q�����m  */
    {
        /*
         * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
         * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
         * EV3�̏ꍇ�́A�v�m�F
         */
        distance = sonarSensor->getDistance();
        if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
        {
            alert = 1; /* ��Q�������m */
        }
        else
        {
            alert = 0; /* ��Q������ */
        }
        counter = 0;
    }

    return alert;
}

//*****************************************************************************
// �֐��� : tail_control
// ���� : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
static void tail_control(int32_t angle)
{
    float pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; /* ��ᐧ�� */
    /* PWM�o�͖O�a���� */
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
// �֐��� : bt_task
// ���� : unused
// �Ԃ�l : �Ȃ�
// �T�v : Bluetooth�ʐM�ɂ�郊���[�g�X�^�[�g�B Tera Term�Ȃǂ̃^�[�~�i���\�t�g����A
//       ASCII�R�[�h��1�𑗐M����ƁA�����[�g�X�^�[�g����B
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while(1)
    {
        uint8_t c = fgetc(bt); /* ��M */
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
