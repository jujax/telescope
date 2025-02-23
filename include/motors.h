#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "A4988.h"
#include "variables.h"

class Motors {
public:
    Motors() : stepper_DA(MOTOR_STEPS, AD_DIR, AD_STEP), stepper_DEC(MOTOR_STEPS, DEC_DIR, DEC_STEP) {}

    void initialize() {
        stepper_DA.begin(RPM, MICROSTEPS);
        stepper_DEC.begin(RPM, MICROSTEPS);
        stepper_DEC.enable();
        stepper_DA.enable();
        stepper_DA.setSpeedProfile(stepper_DA.LINEAR_SPEED, 100, 100);
        stepper_DEC.setSpeedProfile(stepper_DEC.LINEAR_SPEED, 100, 100);
    }

    void moveDA(double steps) {
        stepper_DA.startRotate(steps * RAPPORT_POULIE);
    }

    void moveDEC(double steps) {
        stepper_DEC.startRotate(steps * RAPPORT_POULIE);
    }

    void stopDA() {
        stepper_DA.stop();
    }

    void stopDEC() {
        stepper_DEC.stop();
    }

private:
    A4988 stepper_DA;
    A4988 stepper_DEC;
};

#endif // MOTORS_H
