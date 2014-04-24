#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRLeft,         sensorI2CCustom)
#pragma config(Sensor, S3,     IRRight,        sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     leftMotor,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     frontLow,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     scoop,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     frontHigh,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     frontLow2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     rightMotor,    tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    AutoLeft,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    AutoRight,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "menu_helper.h"
#include "drivers/hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"

//Basket measurements
const float firstBasketInches = 18;
const float secondBasketInches = 8;
const float thirdBasketInches = 14;

//Turning measurements
float turnDistanceLeft = 15;
float turnDistanceRight = 15;
float turnToLine = 29;
float scoringTolerance = 1;
float parkingTolerance = 1;
float leftReturnAdjustment = -.75;
float rightReturnAdjustment = 1;

//Speeds
const float forwardSpeed = 50;
const float turnSpeed = 40;
const float returnSpeed = 100;

//Directions
string forward = "forward";
string backwards = "backwards";
string rightDirection = "right";
string leftDirection = "left";

//Wait times
const float preArmScoreWait = 1000;

const float initializeMotorWait = 250;
const float seekerReadWait = 40;

//Servo, Sensor, and Motor values
const float autoRightServoValue = 240;
const float autoRightStartServoValue = 70;
const float autoLeftServoValue = 80;
const float autoLeftStartServoValue = 250;
const float initializeMotorValue = 100;
const float autoSensorValue = 5;

void preInitializeRobot(){
	servoTarget[AutoRight] = autoRightStartServoValue;
	servoTarget[AutoLeft] = autoLeftStartServoValue;
}

/*
	Converts measured distance of field to values robot encoders use
	Parameters:
		inches - distance to move
*/
float convertInches(float inches){
	return (1440/9.42) * inches;
}

/*
 Moves the autonomous arm to score the block
*/
void scoreBlock(){
	servoTarget[right?AutoRight:AutoLeft] = right?autoRightServoValue:autoLeftServoValue;
	wait1Msec(preArmScoreWait);
	servoTarget[right?AutoRight:AutoLeft] = right?autoRightStartServoValue:autoLeftStartServoValue;

	//used to make sure the arm finishes scoring before the robot moves
	//wait1Msec(postArmScoreWait);

}

/*
	Used to move the robot based on values passed in.
	Parameters:
		distance - the distance to travel
		speed - the speed to travel
		direction - the direction to travel
*/
void moveRobot(float distance, float speed, string direction, float tolerance){
	//reset encoder values
	nMotorEncoder[rightMotor] = 0;
	nMotorEncoder[leftMotor] = 0;

	float target = convertInches(distance);

	//setting the encoder target
	nMotorEncoderTarget[rightMotor] = target;
	nMotorEncoderTarget[leftMotor] = target;



	//blocks used to set correct motor speeds
	if(forward == direction){
		motor[rightMotor] = speed;
		motor[leftMotor] = speed;
	}
	else if(backwards == direction){
		motor[rightMotor] = -speed;
		motor[leftMotor] = -speed;
	}
	else if(rightDirection == direction){
		motor[rightMotor] = -speed;
		motor[leftMotor] = speed;
	}
	else if(leftDirection == direction){
		motor[rightMotor] = speed;
		motor[leftMotor] = -speed;
	}

	//Move the robot until encoder target is reached
	while(nMotorRunState[rightMotor] != runStateIdle || nMotorRunState[leftMotor] != runStateIdle){
			nxtDisplayTextLine(1, "target: %d", target);
		  nxtDisplayTextLine(2, "actual Right: %d", nMotorEncoder[rightMotor]);
		  nxtDisplayTextLine(3, "actual Left: %d", nMotorEncoder[leftMotor]);
		if(abs(target - abs(nMotorEncoder[rightMotor])) < tolerance || abs(target - abs(nMotorEncoder[leftMotor])) < tolerance){
			break;
		}
	}
		motor[rightMotor] = 0;
		motor[leftMotor] = 0;
}

/*
	Used to move the robot from the starting position to the bridge
*/
void parkRobot(){
		int dynamicDistance = right?turnDistanceRight:turnDistanceLeft;
		string dynamicDirection = right?rightDirection:leftDirection;
		moveRobot(dynamicDistance + 1, turnSpeed, dynamicDirection, parkingTolerance);
		moveRobot(turnToLine, returnSpeed, forward, parkingTolerance);
		moveRobot(dynamicDistance, turnSpeed, dynamicDirection, parkingTolerance);
		moveRobot(turnToLine + 1, returnSpeed, backwards, parkingTolerance);
}

/*
	Raises the arm from the legal starting position to prevent scraping on the field
*/
void initializeRobot(){
	motor[frontLow] = initializeMotorValue;
	motor[frontLow2] = initializeMotorValue * -1;
	wait1Msec(initializeMotorWait);
	motor[frontLow] = 0;
	motor[frontLow2] = 0;
	scoringTolerance = scoreTol;
	parkingTolerance = parkTol;
}

/*
	Used to stop the robot after on the bridge
*/
void stopRobot(){
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;

	while(blockRobot){
		nMotorEncoder[rightMotor] = 0;
		nMotorEncoder[leftMotor] = 0;

		while(true){
			float encoderRight = nMotorEncoder[rightMotor];
			float encoderLeft = nMotorEncoder[leftMotor];

			bFloatDuringInactiveMotorPWM = false;


			nxtDisplayTextLine(1, "target: %d", encoderRight);
			nxtDisplayTextLine(2, "target: %d", encoderLeft);
		  nxtDisplayTextLine(3, "Right: %d", nMotorEncoder[rightMotor]);
		  nxtDisplayTextLine(4, "Left: %d", nMotorEncoder[leftMotor]);

			while(encoderRight == nMotorEncoder[rightMotor] || encoderLeft == nMotorEncoder[leftMotor]){
			}

			float newEncoderRight = nMotorEncoder[rightMotor];
			float newEncoderLeft = nMotorEncoder[leftMotor];

			if(newEncoderRight/abs(newEncoderRight) == -1){
				nMotorEncoderTarget[rightMotor] = abs(newEncoderRight) - encoderRight;
				nMotorEncoderTarget[leftMotor] = abs(newEncoderLeft) - encoderLeft;
				motor[leftMotor] = 50;
				motor[rightMotor] = 50;

				while(nMotorRunState[rightMotor] != runStateIdle || nMotorRunState[leftMotor] != runStateIdle){
					nxtDisplayTextLine(1, "target: %d", nMotorEncoderTarget[rightMotor]);
					nxtDisplayTextLine(2, "target: %d", nMotorEncoderTarget[leftMotor]);
			  	nxtDisplayTextLine(3, "actual Right: %d", nMotorEncoder[rightMotor]);
		  		nxtDisplayTextLine(4, "actual Left: %d", nMotorEncoder[leftMotor]);
		  		if(abs(nMotorEncoder[rightMotor]) - abs(nMotorEncoderTarget[rightMotor]) > 100 || abs(nMotorEncoder[leftMotor]) - abs(nMotorEncoderTarget[leftMotor]) > 100){
						break;
					}
				}
			}

			motor[leftMotor] = 0;
			motor[rightMotor] = 0;
		}
		while(!blockRobot){
		}
	}
}
/*
	Used to commence scoring with the block and parking based on the values
	Parameters:
		distance - distance to travel
		speed - speed to travel
		direction - direction to travel
*/
void scoreRobot(float distance, float speed, string direction){
	scoreBlock();
	moveRobot(distance, speed, direction, parkingTolerance);
	parkRobot();
	stopRobot();
}

void startRobot(){
	//sets seeker value
	tHTIRS2DSPMode _mode = DSP_1200;
	int seekerValue = -5;
	float adjustment = right?rightReturnAdjustment:leftReturnAdjustment;

	//Starts the first basket movements
  moveRobot(firstBasketInches, forwardSpeed, forward, convertInches(scoringTolerance));
	wait1Msec(seekerReadWait);
	seekerValue = HTIRS2readACDir(right?IRRight:IRLeft)
	//Sensor found, proceed to scoring
	if(seekerValue == autoSensorValue || !full){
		scoreRobot(firstBasketInches - adjustment, returnSpeed, backwards);
	}

	//No sensor found so move to second basket movements
	moveRobot(secondBasketInches, forwardSpeed, forward, convertInches(scoringTolerance));
	wait1Msec(seekerReadWait);
	seekerValue = HTIRS2readACDir(right?IRRight:IRLeft)
	//Sensor found, proceed to scoring
	if(seekerValue == autoSensorValue){
		scoreRobot(firstBasketInches + secondBasketInches - adjustment, returnSpeed, backwards);
	}

	//No sensor found so move to third basket movements
	moveRobot(thirdBasketInches, forwardSpeed, forward, convertInches(scoringTolerance));
	wait1Msec(seekerReadWait);
	seekerValue = HTIRS2readACDir(right?IRRight:IRLeft)
	//Sensor found, proceed to scoring
	if(seekerValue == autoSensorValue){
		scoreRobot(firstBasketInches + secondBasketInches + thirdBasketInches - adjustment, returnSpeed, backwards);
	}

	//No sensor found so score in fourth basket
	moveRobot(secondBasketInches, forwardSpeed, forward, convertInches(scoringTolerance));
	scoreRobot(firstBasketInches + secondBasketInches + thirdBasketInches + secondBasketInches - adjustment, returnSpeed, backwards);
}

task main()
{
	bFloatDuringInactiveMotorPWM = false;
	bDisplayDiagnostics = false;
	preInitializeRobot();
	StartTask(runMenu);
		bDisplayDiagnostics = false;
	waitForStart(); // Wait for the beginning of autonomous phase.

	StopTask(runMenu);
	eraseDisplay();
	bDisplayDiagnostics = false;
  initializeRobot();
	wait1Msec(delay*1000);
	startRobot();
	stopRobot();
}
