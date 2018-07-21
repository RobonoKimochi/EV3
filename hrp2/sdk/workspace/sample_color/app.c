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

const int touch_sensor = EV3_PORT_2,
          color_sensor = EV3_PORT_3,
          tail_motor   = EV3_PORT_A,
          left_motor   = EV3_PORT_B,
          right_motor  = EV3_PORT_C;

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH  (6)
#define CALIB_FONT_HEIGHT (8)

void main_task(intptr_t unused) {

    char    buf[256];
//  uint8_t ambient;
    uint8_t light;

    /* LCD画面表示 */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);

    // Configure sensors
    ev3_sensor_config(color_sensor, COLOR_SENSOR);

    while (1) {

//      ambient = ev3_color_sensor_get_ambient( color_sensor );
        light   = ev3_color_sensor_get_reflect( color_sensor );

        sprintf( buf, "sample_color : light   = %03d", light );
        ev3_lcd_draw_string( buf, 0, CALIB_FONT_HEIGHT*1);

//        sprintf( buf, "             : ambient = %03d", ambient );
//        ev3_lcd_draw_string( buf, 0, CALIB_FONT_HEIGHT*2);

        tslp_tsk(4); /* 4msec周期起動 */
    }

    ext_tsk();
}

