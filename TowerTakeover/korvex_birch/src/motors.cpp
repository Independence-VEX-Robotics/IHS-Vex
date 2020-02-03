#include "main.h"
#include "korvexlib.h"
#include "okapi/api.hpp"

using namespace okapi;

pros::Motor liftMotor(ARM_MTR1, pros::E_MOTOR_GEARSET_36, false);
pros::Motor clawMotor(CLAW_MTR1, pros::E_MOTOR_GEARSET_36, false);


// controller
Controller masterController;
ControllerButton armUp(ControllerDigital::R1);
ControllerButton armDown(ControllerDigital::R2);
ControllerButton clawOpen(ControllerDigital::L1);
ControllerButton clawClose(ControllerDigital::L2);
