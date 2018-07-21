/******************************************************************************
 *  app.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/*
 *  �e�^�X�N�̗D��x�̒�`
 */
// #define TMIN_TPRI        1            /* �^�X�N�D��x�̍ŏ��l�i�ō��l�j */
// #define TMIN_APP_TPRI        (TMIN_TPRI + 8)
#define MAIN_PRIORITY       TMIN_APP_TPRI + 1      /* main�^�X�N�̗D��x       (10) */
#define TRACER_PRIORITY     TMIN_APP_TPRI + 2      /* Tracer�^�X�N�̗D��x     (12) */
#define ODMETRY_PRIORITY    TMIN_APP_TPRI + 3      /* Odmetry�^�X�N�̗D��x    (13) */
#define TAILMOTOR_PRIORITY  TMIN_APP_TPRI + 4      /* TailMotor�^�X�N�̗D��x  (14) */
#define LOGGER_PRIORITY     TMIN_APP_TPRI + 5      /* Logger�^�X�N�̗D��x     (15) */
#define REMORT_PRIORITY     TMIN_APP_TPRI + 6      /* Remort�^�X�N�̗D��x     (15) */
#define UI_PRIORITY         TMIN_APP_TPRI + 7      /* UI�^�X�N�̗D��x         (16) */

#define ABS(v)  ((v) < 0 ? -(v) : (v))
/*
 *  �^�[�Q�b�g�Ɉˑ�����\���̂���萔�̒�`
 */
#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* �^�X�N�̃X�^�b�N�T�C�Y */
#endif /* STACK_SIZE */

/*
 *  �֐��̃v���g�^�C�v�錾
 */
#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void tracer_task(intptr_t exinf);
// extern void ev3_cyc_tracer(intptr_t exinf);
// added part
extern void tailmotor_task(intptr_t exinf);
extern void ev3_cyc_tailmotor(intptr_t exinf);
extern void odmetry_task(intptr_t exinf);
extern void ev3_cyc_odmetry(intptr_t exinf);
extern void ui_task(intptr_t exinf);
extern void ev3_cyc_ui(intptr_t exinf);
extern void logger_task(intptr_t exinf);
extern void ev3_cyc_logger(intptr_t exinf);
extern void remort_task(intptr_t exinf);
extern void ev3_cyc_remort(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
