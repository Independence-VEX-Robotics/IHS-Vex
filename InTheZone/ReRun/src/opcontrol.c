/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
//Documentation - I wil lbe taking input and storing it in an array, one for each input.
//For example, J1C1 would be Joystick 1, adding C selects channel, and if there is a button B selects button, so , and its values updated every 50 msec.
//Global variables
int J1C1[600];
int J1C2[600];
int J1C3[600];
int J1C4[600];
//Initialize joystick 1 buttons
int J1C5BU[600];
int J1C5BD[600];
int J1C6BU[600];
int J1C6BD[600];
int J1C7BU[600];
int J1C7BL[600];
int J1C7BD[600];
int J1C7BR[600];
int J1C8BU[600];
int J1C8BL[600];
int J1C8BD[600];
int J1C8BR[600];
//Initialize joystick 2
int J2C7BU[600];
int J2C7BL[600];
int J2C7BD[600];
int J2C7BR[600];
int J2C8BU[600];
int J2C8BL[600];
int J2C8BD[600];
int J2C8BR[600];
//Timer - update 1 time per driver cycle, every 50 msec
int DriveTimer = 0;
/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	while (1) {
		if (joystickGetDigital(1,7,JOY_DOWN) == 1) { // Begin Record function
			char J1C2Str [J1C2[DriveTimer]]; // Debug function, convert array of joystick 1 channel 2, input into string
			J1C2[DriveTimer] = joystickGetAnalog(1, 2);// Get analog value of vertical axis of right stick, joystick 1
			J1C3[DriveTimer] = joystickGetAnalog(1, 4); // Get analog value of vertical axis of left stick, joystick 1
			J1C5BU[DriveTimer] = joystickGetDigital(1,5,JOY_UP); // Get boolean value of upper right bumper
			J1C5BD[DriveTimer] = joystickGetDigital(1,5,JOY_DOWN); // Get boolean value of lower right bumper
			J1C6BU[DriveTimer] = joystickGetDigital(1,6,JOY_UP); // Get boolean value of upper left bumper
			J1C6BD[DriveTimer] = joystickGetDigital(1,6,JOY_DOWN); // Get boolean value of lower left bumper
			printf(J1C2Str);
			lcdPrint(uart1,1, J1C2Str);
			DriveTimer = DriveTimer + 1;
			delay(50);
		}

	}
}
