#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     irSeeker,       sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     motorB,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorC,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-irseeker-v2.h"

const float inchesToRotations = 7.851;
const float turnDistance = 3.4;
const float turnToLine = 33;
const float forwardSpeed = 50.0;
const float turnSpeed = 25;
string forward = "forward";
string backwards = "backwards";
string right = "right";
string left = "left";

float distanceToEncoderValue(float distance){
	return (distance/inchesToRotations) * 360;
}

void moveRobot(float distance, float speed, string direction){
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;

	nMotorEncoderTarget[motorB] = distanceToEncoderValue(distance);
	nMotorEncoderTarget[motorC] = distanceToEncoderValue(distance);

	if(forward == direction){
		motor[motorB] = speed;
		motor[motorC] = speed;
	}
	else if(backwards == direction){
		motor[motorB] = -speed;
		motor[motorC] = -speed;
	}
	else if(right == direction){
		motor[motorB] = -speed;
		motor[motorC] = speed;
	}
	else if(left == direction){
		motor[motorB] = speed;
		motor[motorC] = -speed;
	}

	while(nMotorRunState[motorB] != runStateIdle || nMotorRunState[motorC] != runStateIdle){
	}
		motor[motorB] = 0;
		motor[motorC] = 0;
}

task main()
{
	wait10Msec(300);

	tHTIRS2DSPMode _mode = DSP_1200;
	int direction = HTIRS2readDCDir(irSeeker);

	moveRobot(turnDistance, turnSpeed, left);
	moveRobot(turnToLine, forwardSpeed, forward);
	moveRobot(turnDistance, turnSpeed, right);
	moveRobot(turnToLine, forwardSpeed, forward);

}
