/******************************************************************************
 *  PidController.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "PidController.h"

/**
 * RXgN^
 */
PidController::PidController()
    : mDeviation(0),
      mIntegral(0) {
}

/**
 * fXgN^
 */
PidController::~PidController() {
}

/**
 * PID§äÅÌTurnlðZo·é
 * @param deviation  CÌèlÆÌ·ªiÎ·j
 */
int8_t PidController::calControlledVariable(int8_t deviation) {
#define KPID_P_FACTOR       (float)( 0.00)
#define KPID_I_FACTOR       (float)( 0.00)
#define KPID_D_FACTOR       (float)( 0.00)
#define KPID_EDGE_FACTOR    (int32_t)(1)        // Cg[Xûü 1 or -1

#define KPID_TURN_LIMIT     (int32_t)(100)      // ùñw¦l ÀEl

    int32_t brightness_P;                       // P¬ª
    int32_t brightness_I;                       // I¬ª
    int32_t brightness_D;                       // D¬ª
    int32_t turn_P;                             // PZl
    int32_t turn_I;                             // IZl
    int32_t turn_D;                             // DZl
    int32_t turn;                               // ùñ½ß: -100 (¶ùñ) ` 100 (Eùñ)

    brightness_P = (int32_t)deviation;
    brightness_I = mIntegral + brightness_P;
    brightness_D = brightness_P - mDeviation;

    mDeviation = brightness_P;                              // ñZpÉL¯
    mIntegral = brightness_I;

    /* PZ */
    turn_P = (int32_t)(KPID_P_FACTOR * (float)brightness_P);
    turn_P *= KPID_EDGE_FACTOR;

    /* IZ */
    turn_I = (int32_t)(KPID_I_FACTOR * (float)brightness_I);
    turn_I *= KPID_EDGE_FACTOR;

    /* DZ */
    turn_D = (int32_t)(KPID_D_FACTOR * (float)brightness_D);
    turn_D *= KPID_EDGE_FACTOR;

    /* ùñw¦lÝè   */
    turn = turn_P + turn_I + turn_D;

    if (turn >= KPID_TURN_LIMIT) {
        turn = KPID_TURN_LIMIT;
    } else if (turn <= (-1 * KPID_TURN_LIMIT)) {
        turn = -1 * KPID_TURN_LIMIT;
    }

    return turn;
}

