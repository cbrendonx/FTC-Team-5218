#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     IRSeeker,       sensorI2CCustom)
#pragma config(Sensor, S3,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S4,     lightSensor,    sensorLightInactive)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     driveRight,    tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     driveLeft,     tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_1,     grabberArm,    tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_2,     driveSide,     tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C4_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    gravityShelf,         tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    IRServo,              tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    Ramp,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an TETRIX robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#include "../library/sensors/drivers/hitechnic-sensormux.h"
#include "../library/sensors/drivers/hitechnic-irseeker-v2.h"
#include "../library/sensors/drivers/hitechnic-compass.h"
#include "../library/sensors/drivers/lego-light.h"
#include "../library/sensors/drivers/lego-touch.h"

#include "Lib/Lib12-13.c"
#include "AutoCommon.c"

#define FORWARDTOPEGS 2000

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of autonomous mode, you may want to perform some initialization on your robot.
// Things that might be performed during initialization include:
//   1. Move motors and servos to a preset position.
//   2. Some sensor types take a short while to reach stable values during which time it is best that
//      robot is not moving. For example, gyro sensor needs a few seconds to obtain the background
//      "bias" value.
//
// In many cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation. Customize as appropriate for
// your specific robot.
//
// The types of things you might do during the autonomous phase (for the 2008-9 FTC competition)
// are:
//
//   1. Have the robot follow a line on the game field until it reaches one of the puck storage
//      areas.
//   2. Load pucks into the robot from the storage bin.
//   3. Stop the robot and wait for autonomous phase to end.
//
// This simple template does nothing except play a periodic tone every few seconds.
//
// At the end of the autonomous period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	direction_t dir;
    char tmp[50];

	initializeRobot();

    // Wait for the beginning of autonomous phase.
	waitForStart();

	// Move forward a predetermined amount.
    moveForward(61);

   // moveForwardToIRBeacon(120);

    pauseDebug(tmp, 1);

	// Read IR sensor.
	dir = lookForIRBeacon();

	// We found the IR beacon, we should be to the
	// dir direction of the white line.  Move to the line.
	lookForWhiteLine(dir);

    pauseDebug("I think i'm on the line", 1);

	// If we were knocked off target, realign ourself.
    // FIXME: The compass does not appear to be accurate.
    //        This is actually knocking us off the peg.
	//dir = alignToPeg();

    /*
     * Don't do anything just below as the neither the line
     * nor the compass sensors are currently accurate.
     */
    dir = NO_DIR;

	/*
	* The rotatation may have knocked us off the white line
	* If we rotated right, look back to the left, if we rotated
	* left, do the opposite.
	*/
	switch (dir) {
		case RIGHT:
			lookForWhiteLine(LEFT);
			break;
		case LEFT:
			lookForWhiteLine(RIGHT);
			break;
		case NO_DIR:
		default:
	}

	placeRing();

	while (true)
	{}
}
