
#define ENC_TICKS_PER_DEGREE 25

typedef enum {
	NO_DIR,
	LEFT,
	RIGHT,
    FORWARD,
    BACKWARD
} direction_t;

void initializeMotors(void)
{
	nMotorPIDSpeedCtrl[driveLeft] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[driveRight] = mtrSpeedReg;
}

/**********************************************************************************
 * Movement functions
 **********************************************************************************/

void rotateClockwise(int speed)
{
  	motor[driveRight] = -speed;
	motor[driveLeft] = speed;
}

void rotateCounterClockwise(int speed)
{
	motor[driveRight] = speed;
	motor[driveLeft] = -speed;
}

/*
 * moveForwardOn
 *
 * Turns the motors on, never turns them off.
 */
void moveForwardOn(int speed)
{
	motor[driveRight] = speed;
	motor[driveLeft] = speed;
}

void moveBackwardOn(int speed)
{
	motor[driveRight] = -speed;
	motor[driveLeft] = -speed;
}

/*
 * moveForwardOff
 *
 * Turns the motors off.
 */
void moveForwardOff()
{
	motor[driveRight] = 0;
	motor[driveLeft] = 0;
}

/*
 * allMotorsOff
 *
 * Turns off all motors on the chassis.
 */
void allMotorsOff()
{
    motor[driveLeft] = 0;
    motor[driveRight] = 0;
}

/*
 * moveForward
 *
 * Move the robot forward a given number of inches.
 */
void moveForward (int inches, int speed = 100)
{
	int encoderCounts = inches * ENCPERINCH;

	nMotorEncoder[driveRight] = 0;
	nMotorEncoder[driveLeft] = 0;

	motor[driveRight] = speed;
	motor[driveLeft] = speed;

	while (abs(nMotorEncoder[driveLeft]) < encoderCounts && abs(nMotorEncoder[driveRight]) < encoderCounts)
	{
	}

	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}

void moveForwardHalf(int inches, int speed)
{
	int encoderCounts = inches * (ENCPERINCH/2);

	nMotorEncoder[driveRight] = 0;
	nMotorEncoder[driveLeft] = 0;

	motor[driveRight] = speed;
	motor[driveLeft] = speed;

	while (abs(nMotorEncoder[driveLeft]) < encoderCounts && abs(nMotorEncoder[driveRight]) < encoderCounts)
	{
	}

	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}

/*
 * moveBackward
 *
 * Move the robot backward a given number of inches
 */
void moveBackward (int inches)
{
	int encoderCounts = inches * ENCPERINCH;

	nMotorEncoder[driveRight] = 0;
	nMotorEncoder[driveLeft] = 0;

	motor[driveRight] = -50;
	motor[driveLeft] = -50;

	while (abs(nMotorEncoder[driveLeft]) < encoderCounts && abs(nMotorEncoder[driveRight]) < encoderCounts)
	{
	}

	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}

void moveBackwardHalf(int inches, int speed)
{
	int encoderCounts = inches * (ENCPERINCH/2);

	nMotorEncoder[driveRight] = 0;
	nMotorEncoder[driveLeft] = 0;

	motor[driveRight] = -speed;
	motor[driveLeft] = -speed;

	while (abs(nMotorEncoder[driveLeft]) < encoderCounts && abs(nMotorEncoder[driveRight]) < encoderCounts)
	{
	}

	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}
