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

void autonomous()
{
    int autonStart = pros::millis(); // note the start time
    chassis.resetSensors();
    // -1 = skills
    // 0 = blue close, mid and top flag and park
    // 1 = blue close, all flags and park
    // 2 = blue far, opponent descore
    // 3 = red close, mid and top flag and park
    int auton = 0;
    // int auton = autonSelection; // this is to enable auton selector
    // std::cout << autonSelection << std::endl;
    bool preload = false;
    int tmp = 0;
    switch (auton)
    {
    case -2: // test

        // TODO: lower deceleration/acceleration rate
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(200);

        // actual auton
        chassis.turnAngle(200);
        // chassis.moveDistance(24_in);
        // chassis.waitUntilSettled();
        // chassis.moveDistance(-24_in);
        // std::cout << chassis.getSensorVals()[0] << std::endl;
        // std::cout << chassis.getSensorVals()[1] << std::endl;
        // chassis.moveDistance(-20_in);
        // chassis.moveDistance(1_in);
        break;
    case -1: // skills
        // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(100); // this might fix things

        // actual auton
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 200)) // 1 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_velocity(-200);
        while (!(triggerTL.get_new_press() || triggerTR.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor.move_velocity(600);
        chassis.moveDistance(-39_in);
        // back and turn into shooting position
        chassis.turnAngle(-267);
        chassis.moveDistance(-15_in);
        // shoot first ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(33_in);

        // shoot second ball
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);
        flywheelMotor.move_velocity(0);

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
        break;
    case 0: // blue close, mid and top flag and park
        // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

        // actual auton

        // check if we missed preload (idiot)
        if (triggerTL.get_value() || triggerTR.get_value())
        {
            preload = true;
        }
        
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(39_in); // going to cap with ball under it

        // wait until we intake ball to bot
        tmp = 0;
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 100)) // 2 sec timeout
        {
            tmp++;
            pros::delay(20);
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_relative(-200, 200);

        // there is now a ball in both positions
        flywheelMotor.move_velocity(600);
        chassis.moveDistance(-37_in);
        // back and turn into shooting position
        chassis.turnAngle(350);
        chassis.moveDistance(-7_in);
        // shoot first ball when ready
        tmp = 0;
        while (!(flywheelMotor.get_actual_velocity() > 590) && !(tmp > 50))
        {
            tmp++;
            pros::delay(20);
        }
        intakeMotor.move_relative(2000, 200);

        // shoot second ball and move to flip bot flag
        chassis.moveDistanceAsync(50_in);
        pros::delay(300); // this is the timing for the run and shoot
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);
        flywheelMotor.move_velocity(0);
        chassis.waitUntilSettled();

        chassis.moveDistance(-35_in);

        // turn to flip cap
        chassis.turnAngle(-150);

        // move to cap
        capflipMotor.move_absolute(-700, 200);
        chassis.moveDistance(18_in);

        // flip cap
        capflipMotor.move_absolute(0, 150);
        pros::delay(200);

        // park
        chassis.turnAngle(-150);
        chassis.moveDistance(18_in);
        chassis.turnAngle(-275);

        // onwards soldiers! take the platform! fuck im bored
        chassis.moveDistance(30_in);
        chassis.moveDistance(20_in);

        break;

    case 1: // blue front heavy
        // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(150); // this might fix things

        // actual auton
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 100)) // 2 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_velocity(-200);
        while (!(triggerTL.get_new_press() || triggerTR.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor.move_velocity(600);
        chassis.moveDistance(-38_in);
        // back and turn into shooting position
        chassis.turnAngle(490);
        chassis.moveDistance(-7_in);
        // shoot first ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(27_in);

        // shoot second ball
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(600);
        intakeMotor.move_velocity(0);
        flywheelMotor.move_velocity(0);

        // hit third flag
        chassis.turnAngle(80);
        chassis.moveDistance(26_in);
        chassis.turnAngle(-40);

        // its parking time
        chassis.setMaxVelocity(200);
        chassis.moveDistance(-70_in);

        // turn to drive onto platform
        chassis.turnAngle(-550);

        // align with platform
        chassis.moveDistance(17_in);
        intakeMotor.move_velocity(200);

        // drive onto platform
        chassis.moveDistanceAsync(30_in);
        break;

    case 2: // blue stack
        // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(150); // this might fix things

        // actual auton
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 400)) // 2 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_velocity(-200);
        while (!(triggerTL.get_new_press() || triggerTR.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(0);

        // there is now a ball in both positions, back for claw flipout
        chassis.moveDistance(-6_in);

        // turn and park
        chassis.turnAngle(560);
        chassis.moveDistance(8_in);
        chassis.moveDistance(23_in);

        // turn for shoot
        flywheelMotor.move_velocity(550);
        chassis.turnAngle(-210);
        chassis.turnAngleAsync(0);

        // shoot ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 545))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);

        pros::delay(500);
        flywheelMotor.move_velocity(0);
        break;
    case 3: // red close, mid and top flag and park
        break;

    case 4: // red front heavy
        // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(150); // this might fix things

        // actual auton
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 100)) // 2 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_velocity(-200);
        while (!(triggerTL.get_new_press() || triggerTR.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(0);

        // there is now a ball in both positions
        flywheelMotor.move_velocity(600);
        chassis.moveDistance(-38_in);
        // back and turn into shooting position
        chassis.turnAngle(-575);
        chassis.moveDistance(-7_in);
        chassis.turnAngleAsync(-10);
        // shoot first ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);

        // second ball shot position
        chassis.moveDistance(27_in);

        // shoot second ball
        while (!(flywheelMotor.get_actual_velocity() > 590))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(600);
        intakeMotor.move_velocity(-200);
        flywheelMotor.move_velocity(0);

        // hit third flag
        chassis.turnAngle(-70);
        chassis.moveDistance(26_in);
        chassis.turnAngle(50);

        // its parking time
        chassis.setMaxVelocity(200);
        chassis.moveDistance(-67_in);

        // turn to drive onto platform
        chassis.turnAngle(520);

        // align with platform
        chassis.moveDistance(17_in);
        intakeMotor.move_velocity(200);

        // drive onto platform
        chassis.moveDistanceAsync(30_in);
        break;

    case 5: // red far
            // setup
        flywheelMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.setMaxVelocity(150); // this might fix things

        // actual auton
        intakeMotor.move_velocity(200);
        chassis.moveDistanceAsync(37_in); // going to cap with ball under it

        // wait until we intake ball to bot
        while (!(triggerBL.get_new_press() || triggerBR.get_new_press()) && !(tmp > 400)) // 2 sec timeout
        {
            pros::delay(20);
            tmp++;
        }

        // theres a ball at the top, we want to pull it down back to the trigger
        intakeMotor.move_velocity(-200);
        while (!(triggerTL.get_new_press() || triggerTR.get_new_press()))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(0);

        // there is now a ball in both positions, back for claw flipout
        chassis.moveDistance(-7_in);

        // turn and park
        chassis.turnAngle(-560);
        chassis.moveDistance(7_in);
        chassis.moveDistance(23_in);

        // turn for shoot
        flywheelMotor.move_velocity(550);
        chassis.turnAngle(220);
        chassis.turnAngleAsync(0);

        // shoot ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 545))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);
        pros::delay(500);
        // second ball
        flywheelMotor.move_velocity(480);
        // shoot ball when ready
        while (!(flywheelMotor.get_actual_velocity() > 475))
        {
            pros::delay(20);
        }
        intakeMotor.move_velocity(200);
        pros::delay(500);
        intakeMotor.move_velocity(0);

        flywheelMotor.move_velocity(0);
        break;
    default:
        break;
    }
}
