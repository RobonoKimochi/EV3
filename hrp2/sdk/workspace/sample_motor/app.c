#include "ev3api.h"
#include "app.h"

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

#define DRIVING_POWER  40

const int touch_sensor = EV3_PORT_2,
          color_sensor = EV3_PORT_3,
          tail_motor   = EV3_PORT_A,
          left_motor   = EV3_PORT_B,
          right_motor  = EV3_PORT_C;

void main_task(intptr_t unused) {

    // Configure motors
    ev3_motor_config( tail_motor,  LARGE_MOTOR );
    ev3_motor_config( left_motor,  LARGE_MOTOR );
    ev3_motor_config( right_motor, LARGE_MOTOR );
    ev3_motor_config( EV3_PORT_D,  NONE_MOTOR  );

    while (1) {
        ev3_motor_set_power( left_motor, DRIVING_POWER );

        tslp_tsk(4); /* 4msecŽüŠú‹N“® */
    }

    ext_tsk();
}

