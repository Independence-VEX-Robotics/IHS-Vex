#include "main.h"
#include "okapi/api.hpp"
#include "korvexlib.h"
using namespace okapi;

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// factory constructs
// auto chassis = okapi::ChassisControllerFactory::create(
//     {LEFT_MTR1, LEFT_MTR2},               // Left motors
//     {-RIGHT_MTR1, -RIGHT_MTR2},           // Right motors
//     okapi::AbstractMotor::gearset::green, // Normal gearset
//     {4_in, 12.5_in}                       // 4 inch wheels, 12.5 inch wheelbase width
// );

auto chassis = ChassisControllerFactory::create(
    {LEFT_MTR1, LEFT_MTR2},     // Left motors
    {-RIGHT_MTR1, -RIGHT_MTR2}, // Right motors
    IterativePosPIDController::Gains{0.002, 0.0005, 0.002}, // distance args
    IterativePosPIDController::Gains{0.001, 0, 0.001}, // angle args (keeps robot straight)
    IterativePosPIDController::Gains{0.001, 0, 0.001}, // turn args
    AbstractMotor::gearset::green, // normal gearset
    {4_in, 12.5_in}                       // 4 inch wheels, 12.5 inch wheelbase width
);

auto chassisController = okapi::AsyncControllerFactory::motionProfile(
    1.0,    // Maximum linear velocity of the Chassis in m/s
    2.0,    // Maximum linear acceleration of the Chassis in m/s/s
    5.0,    // Maximum linear jerk of the Chassis in m/s/s/s
    chassis // Chassis Controller
);

pros::Motor flywheelMotor_A1(FLY_MTR1, pros::E_MOTOR_GEARSET_06, true);
pros::Motor flywheelMotor_A2(FLY_MTR2, pros::E_MOTOR_GEARSET_06, true);
pros::Motor intakeMotor_A(INTAKE_MTR, pros::E_MOTOR_GEARSET_18, true);
pros::Motor liftMotor_A(LIFT_MTR, pros::E_MOTOR_GEARSET_18, false);

pros::ADIDigitalIn triggerBL_A(TRIGGER_BL);
pros::ADIDigitalIn triggerBR_A(TRIGGER_BR);
pros::ADIDigitalIn triggerTL_A(TRIGGER_TL);
pros::ADIDigitalIn triggerTR_A(TRIGGER_TR);

void autonomous()
{
    chassis.resetSensors();
    // -1 = skills
    // 0 = blue close, mid and top flag and park
    // 1 = blue close, all flags and park
    // 2 = blue far, opponent descore
    // 3 = red close, mid and top flag and park
    int auton = -2;
    // int auton = autonSelection;
    int tmp = 0;
    switch (auton)
    {
    case -2: // test
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(200); // this might fix things

        // actual auton
        chassis.moveDistance(8_in); // going to cap with ball under it
        chassis.stop();
        // chassis.moveDistance(-10_in); // going to cap with ball under it
        // chassis.turnAngle(90);
        break;
    case -1: // skills
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(100); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor_A1.move_velocity(600);
        flywheelMotor_A2.move_velocity(600);
        chassis.moveDistance(-39_in);
        // back and turn into shooting position
        chassis.turnAngle(-267);
        chassis.moveDistance(-15_in);
        // shoot first ball when ready
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(33_in);

        // shoot second ball
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);
        flywheelMotor_A2.move_velocity(0);

        // run into bot flag
        chassis.turnAngle(-25);
        chassis.moveDistance(24_in);
        chassis.turnAngle(15);

        // line up for far cap
        chassis.moveDistance(-64_in);
        chassis.turnAngle(280);

        // park
        chassis.moveDistance(18_in);
        chassis.moveDistance(48_in);

        // move to get far cap
        // intakeMotor_A.move_velocity(200);
        // flywheelMotor_A1.move_velocity(550);
        // chassis.moveDistance(44_in);

        // // intake ball
        // while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        // {
        //     pros::delay(20);
        // }
        // intakeMotor_A.move_velocity(0);

        // // aim for mid flag and shoot
        // chassis.turnAngle(-280);
        // while (!(flywheelMotor_A1.get_actual_velocity() > 540))
        // {
        //     pros::delay(20);
        // }
        // intakeMotor_A.move_velocity(200);
        // pros::delay(500);
        // intakeMotor_A.move_velocity(0);

        // // turn for park
        // chassis.turnAngle(-250);
        // chassis.moveDistance(8_in);
        // chassis.turnAngle(270);

        // // park
        // chassis.moveDistance(30_in);
        break;
    case 0: // blue close, mid and top flag and park
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(120); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistance(30_in); // going to cap with ball under it
        chassis.waitUntilSettled();
        chassis.moveDistanceAsync(6_in);

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor_A1.move_velocity(600);
        flywheelMotor_A2.move_velocity(600);
        chassis.setMaxVelocity(150);
        chassis.moveDistance(-35_in);
        // back and turn into shooting position
        chassis.setMaxVelocity(100);
        chassis.turnAngle(238);
        chassis.moveDistance(-8_in);
        chassis.setMaxVelocity(150);
        // shoot first ball when ready
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(27_in);
        // chassis.turnAngle(-25);

        // shoot second ball
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);
        flywheelMotor_A2.move_velocity(0);

        // move to park
        // chassis.turnAngle(25);
        intakeMotor_A.move_velocity(200);
        chassis.setMaxVelocity(200);
        chassis.moveDistance(-47_in);
        chassis.turnAngle(-265);
        chassis.moveDistance(16_in);
        chassis.moveDistance(25_in);
        break;

    case 1: // blue far
        break;

    case 2: // blue stack
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(100); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions, back for claw flipout
        chassis.moveDistance(-18_in);

        chassis.turnAngleAsync(530);
        liftMotor_A.move_absolute(300, 200);
        pros::delay(800);
        liftMotor_A.move_absolute(-20, 200);
        pros::delay(1000);

        // grab cap
        chassis.moveDistance(-20_in); // wna die
        break;
    case 3: // red close, mid and top flag and park
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(130); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor_A1.move_velocity(600);
        chassis.moveDistance(-37_in);
        // back and turn into shooting position
        chassis.turnAngle(-269);
        chassis.moveDistance(-14_in);
        // shoot first ball when ready
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(30_in);
        chassis.setMaxVelocity(180);

        // shoot second ball
        while (!(flywheelMotor_A1.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);

        // move to park
        chassis.moveDistance(-42_in);
        chassis.turnAngle(230);
        chassis.moveDistance(18_in);
        chassis.moveDistance(32_in);
        break;

    case 4: // red elim setup, mid flags
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(110); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor_A1.move_velocity(475);
        chassis.moveDistance(-37_in);

        // back and turn into shooting position
        chassis.turnAngle(-274);
        chassis.moveDistance(-7_in);

        // shoot first ball when ready
        while (!(flywheelMotor_A1.get_actual_velocity() > 475))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);

        // second ball shot
        flywheelMotor_A1.move_velocity(518);
        // chassis.moveDistance(30_in);
        // chassis.setMaxVelocity(180);

        // shoot second ball
        while (!(flywheelMotor_A1.get_actual_velocity() > 518))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);

        // move to park
        chassis.moveDistance(29_in);
        chassis.turnAngle(230);
        chassis.moveDistance(18_in);
        chassis.moveDistance(32_in);
        break;

    case 5: // red far
        // setup
        liftMotor_A.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        flywheelMotor_A1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(110); // this might fix things

        // actual auton
        intakeMotor_A.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL_A.get_new_press() || triggerBR_A.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor_A.move_velocity(-200);
        while (!(triggerTL_A.get_new_press() || triggerTR_A.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor_A1.move_velocity(585);

        // backup for shooting and park location
        chassis.moveDistance(-8_in);
        chassis.turnAngle(-260);

        // shoot first ball when ready
        while (!(flywheelMotor_A1.get_actual_velocity() > 580))
        {
            pros::delay(20);
        }
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);
        pros::delay(900);

        // prepare next shot
        flywheelMotor_A1.move_velocity(580);
        chassis.moveDistance(5_in);
        chassis.moveDistance(-5_in);

        while (!(flywheelMotor_A1.get_actual_velocity() > 575))
        {
            pros::delay(20);
        }
        // shoot (me)
        intakeMotor_A.move_velocity(200);
        pros::delay(500);
        intakeMotor_A.move_velocity(0);
        flywheelMotor_A1.move_velocity(0);

        // park
        chassis.moveDistance(5_in);
        chassis.moveDistance(26_in);

        break;
    default:
        break;
    }
}
