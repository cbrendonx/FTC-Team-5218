#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     touchSensor,    sensorTouch)
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
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
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
#include "DrivetrainSquare.c"
#include "Lib/Lib12-13.c"
#include "AutoCommon.c"

/*
 * Semaphore for controlling servo movement.
 */
bool blockArm = false;
bool runningAutoPlace = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void initializeRobot()
{
  	servo[gravityShelf] = SHELFDOWN;
  	servo[IRServo] = IRDOWN;
	servo[Ramp] = RAMP_START;

    nMotorPIDSpeedCtrl[driveLeft]  = mtrSpeedReg;
    nMotorPIDSpeedCtrl[driveRight] = mtrSpeedReg;
    nMotorPIDSpeedCtrl[driveSide]  = mtrSpeedReg;

    bFloatDuringInactiveMotorPWM = false;

	return;
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * waitForSensorArm
 *
 * Use a task as a semaphore in order to prevent processing
 * of a servo move until after a prior request to move the servo
 * has finished.  1 second is probably overkill, but the driver
 * should not be cycling this arm that fast anyway.
 */
task waitForSensorArm()
{
    blockArm = true;
    wait1Msec(1000);
    blockArm = false;
}

void forwarddrivetrain()
{
    if (joystick.joy1_TopHat == -1)
    {
		if(abs(joystick.joy1_y1) > 20)
		{
	    	motor[driveLeft] = joystick.joy1_y1;
		}
		else
		{
		    motor[driveLeft] = 0;
		}

		if(abs(joystick.joy1_y2) > 20)
		{
	    	motor[driveRight] = joystick.joy1_y2;
		}
		else
		{
		    motor[driveRight] = 0;
		}
    }
    else
    {
        if (joystick.joy1_TopHat == 0)
        {
            moveForwardOn(20);
        }
        else if (joystick.joy1_TopHat == 4)
        {
            moveBackwardOn(20);
        }
    }
}

void sidewaysdrivetrain()
{
    if (joystick.joy1_TopHat == -1)
    {
		if (joy1Btn(8))
		{
			motor[driveSide] = -100;
		}
		else if (joy1Btn(7))
		{
			motor[driveSide] = 100;
		}
		else
		{
			motor[driveSide] = 0;
		}
    }
    else
    {
        if (joystick.joy1_TopHat == 2)
        {
            moveSidewaysOn(RIGHT, 20);
        }
        else if (joystick.joy1_TopHat == 6)
        {
            moveSidewaysOn(LEFT, 20);
        }
    }
}

void movegravityShelf()
{
	int val;

    if (joy2Btn(5))
    {
		servo[gravityShelf] = SHELFREMOVE;
    }

	if (joy2Btn(7))
	{
		servo[gravityShelf] = SHELFDOWN;
	}

    if (joy2Btn(1))
    {
        raiseShelfToAutoPlacePosition(NO_DIR);
    }

	if (joy2Btn(2))
	{
		val = ServoValue[gravityShelf];
        nxtDisplayTextLine(3, "Current: %d", val);
		val += SHELFINCREMENT;
        nxtDisplayTextLine(3, "Dest: %d", val);
		servo[gravityShelf] = val;
	}

	if (joy2Btn(4))
	{
		val = ServoValue[gravityShelf];
        nxtDisplayTextLine(3, "Current: %d", val);
		val -= SHELFINCREMENT;
        nxtDisplayTextLine(3, "Dest: %d", val);
		servo[gravityShelf] = val;
	}
}

void sensorStalk()
{
    int val;

	if(joy2Btn(6))
	{
        if (blockArm) {
            return;
        }
        val = ServoValue[IRServo];
        if (val < (IRUP + 20)) {
            servo[IRServo] = IRDOWN;
            StartTask(waitForSensorArm, kDefaultTaskPriority);
        } else {
            servo[IRServo] = IRUP;
            StartTask(waitForSensorArm, kDefaultTaskPriority);
        }
	}
	else if(joy2Btn(8))
	{
	    servo[IRServo] = IRRING;
	}
}

void moveRamp ()
{
    if (joy1Btn(1))
    {
        servo[Ramp] = RAMP_START;
    }
    else if (joy1Btn(3))
    {
        servo[Ramp] = RAMP_DEPLOY;
    }
}

void placeRingEx(direction_t dir)
{
    runningAutoPlace = true;
    lookForWhiteLine(dir);
    findPeg();
    placeRing(dir);
    runningAutoPlace = false;
    allMotorsOff();
}

task placeRingRightTask()
{
    placeRingEx(RIGHT);
}

task placeRingLeftTask()
{
    placeRingEx(LEFT);
}

void autoPlace(void)
{
    if (joy1Btn(2))
    {
        StartTask(placeRingRightTask, kDefaultTaskPriority);
    }

    if (joy1Btn(4))
    {
        StartTask(placeRingLeftTask, kDefaultTaskPriority);
    }
}

void stopAutoPlace(void)
{
	runningAutoPlace = false;
    StopTask(placeRingRightTask);
    StopTask(placeRingLeftTask);
    allMotorsOff();
}

task main()
{
    initializeRobot();

    waitForStart();

    while (true)
    {
        getJoystickSettings(joystick);

        if (runningAutoPlace && (!joystick.joy1_Buttons))
        {
            continue;
        }
        else
        {
            if (runningAutoPlace) {
                stopAutoPlace();
            }
	  	    forwarddrivetrain();
	  	    sidewaysdrivetrain();
	  	    movegravityShelf();
	  	    sensorStalk();
	        moveRamp();
            autoPlace();
        }
    }
}