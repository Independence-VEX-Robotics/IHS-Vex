#include "main.h"
#include "okapi/api.hpp"

#pragma
// ports for motors

// chassis motors
const int LEFT_MTR1 = 1;
const int RIGHT_MTR1 = 2;
const int ARM_MTR1 = 8;
const int CLAW_MTR1 = 3;

// globals
extern int autonSelection;

// motor declerations
extern pros::Motor liftMotor;
extern pros::Motor clawMotor;

// controller declerations
extern okapi::Controller masterController;

extern okapi::ControllerButton intakeIn;
extern okapi::ControllerButton armUp;
extern okapi::ControllerButton armDown;
extern okapi::ControllerButton clawOpen;
extern okapi::ControllerButton clawClose;
