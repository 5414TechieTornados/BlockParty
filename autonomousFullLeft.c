#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRLeft,         sensorI2CCustom)
#pragma config(Sensor, S3,     IRRight,        sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     right,         tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     left,          tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontHigh,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     flag,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     frontLow,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    scoop,                tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    AutoLeft,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    AutoRight,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"

//Basket measurements
const float firstBasketInches = 18;
const float secondBasketInches = 8;
const float thirdBasketInches = 14;

//Turning measurements
const float turnDistanceLeft = 15;
const float turnDistanceRight = 13;
const float turnToLine = 29;

//Speeds
const float forwardSpeed = 50.0;
const float turnSpeed = 40;

//Directions
string forward = "forward";
string backwards = "backwards";
string rightDirection = "right";
string leftDirection = "left";

//Wait times
const float preArmScoreWait = 500;
const float postArmScoreWait = 750;
const float initializeMotorWait = 250;
const float seekerReadWait = 25;

//Servo, Sensor, and Motor values
const float autoServoValue = 150;
const float initializeMotorValue = 100;
const float autoSensorValue = 5;

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

	servoTarget[AutoLeft] = 0;
	servoTarget[AutoRight] = 0;

	wait1Msec(preArmScoreWait);

	servoTarget[AutoLeft] = autoServoValue;
	servoTarget[AutoRight] = autoServoValue;

	//used to make sure the arm finishes scoring before the robot moves
	wait1Msec(postArmScoreWait);
}

/*
	Used to move the robot based on values passed in.
	Parameters:
		distance - the distance to travel
		speed - the speed to travel
		direction - the direction to travel
*/
void moveRobot(float distance, float speed, string direction){
	//reset encoder values
	nMotorEncoder[right] = 0;
	nMotorEncoder[left] = 0;

	//setting the encoder target
	nMotorEncoderTarget[right] = convertInches(distance);
	nMotorEncoderTarget[left] = convertInches(distance);

	//blocks used to set correct motor speeds
	if(forward == direction){
		motor[right] = speed;
		motor[left] = speed;
	}
	else if(backwards == direction){
		motor[right] = -speed;
		motor[left] = -speed;
	}
	else if(rightDirection == direction){
		motor[right] = -speed;
		motor[left] = speed;
	}
	else if(leftDirection == direction){
		motor[right] = speed;
		motor[left] = -speed;
	}

	//Move the robot until encoder target is reached
	while(nMotorRunState[right] != runStateIdle || nMotorRunState[left] != runStateIdle){
	}
		motor[right] = 0;
		motor[left] = 0;
}

/*
	Used to move the robot from the starting position to the bridge
*/
void parkRobot(){
		moveRobot(turnDistanceLeft, turnSpeed, leftDirection);
		moveRobot(turnToLine, forwardSpeed, forward);
		moveRobot(turnDistanceRight, turnSpeed, rightDirection);
		moveRobot(turnToLine, forwardSpeed, forward);
}

/*
	Raises the arm from the legal starting position to prevent scraping on the field
*/
void initializeRobot(){
	motor[frontLow] = initializeMotorValue;
	wait1Msec(initializeMotorWait);
	motor[frontLow] = 0;
}

/*
	Used to stop the robot after on the bridge
*/
void stopRobot(){
	while(true){
		motor[left] = 0;
		motor[right] = 0;
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
	moveRobot(distance, speed, direction);
	parkRobot();
	stopRobot();
}

task main()
{
	waitForStart();
	initializeRobot();

	//sets seeker value
	tHTIRS2DSPMode _mode = DSP_1200;

	//Starts the first basket movements
	moveRobot(firstBasketInches, forwardSpeed, forward);
	wait1Msec(seekerReadWait);

	//Sensor found, proceed to scoring
	if(HTIRS2readDCDir(IRLeft) == autoSensorValue){
		scoreRobot(firstBasketInches - 1, forwardSpeed, backwards);
	}

	//No sensor found so move to second basket movements
	moveRobot(secondBasketInches, forwardSpeed, forward);
	wait1Msec(seekerReadWait);

	//Sensor found, proceed to scoring
	if(HTIRS2readACDir(IRLeft) == autoSensorValue){
		scoreRobot(firstBasketInches + secondBasketInches - 1, forwardSpeed, backwards);
	}

	//No sensor found so move to third basket movements
	moveRobot(thirdBasketInches, forwardSpeed, forward);
	wait1Msec(seekerReadWait);

	//Sensor found, proceed to scoring
	if(HTIRS2readACDir(IRLeft) == autoSensorValue){
		scoreRobot(firstBasketInches + secondBasketInches + thirdBasketInches- 1, forwardSpeed, backwards);
	}

	//No sensor found so score in fourth basket
	moveRobot(secondBasketInches, forwardSpeed, forward);
	scoreRobot(firstBasketInches + secondBasketInches + thirdBasketInches + secondBasketInches - 1, forwardSpeed, backwards);
}
