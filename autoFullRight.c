
#include "drivers/hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"

float xCoor = -1;
float yCoor = -1;
int direction = 0;

int north = 0;
int east = 1;
int south = 2;
int west = 3;

float convertInches(){
}

void moveForward(float difference){
	wait10Msec(50);
}

void moveBackward(){
	wait10Msec(50);
}

void turnLeft(){
		wait10Msec(50);
}

void turnRight(){
		wait10Msec(50);
}

void handleXCoor(float newXCoor, float speed){
	float difference = newXCoor - xCoor;
	if(direction == north){
		if((difference/abs(difference)) == 1){
			direction = east;
			turnRight();
		}
		else{
			direction = west;
			turnLeft();
		}
	}
	else if(direction == south){
		if((difference/abs(difference)) == 1){
			direction = east;
			turnLeft();
		}
		else{
			direction = west;
			turnRight();
		}
	}
	else if(direction == east){
		if((difference/abs(difference)) == 1){
		}
		else{
			direction = west;
			turnLeft();
			turnLeft();
		}
	}
	else if(direction == west){
		if((difference/abs(difference)) == 1){
			direction = east;
			turnLeft();
			turnLeft();
		}
	}
	moveForward(difference);
}

void handleYCoor(float newYCoor, float speed){
}

void simulateRobot(float newXCoor, float newYCoor, float speed){
	if(speed != 0){
		handleXCoor(newXCoor, speed);
		handleYCoor(newYCoor, speed);
	}
	xCoor = newXCoor;
	yCoor = newYCoor;
}

task main()
{

	simulateRobot(3, 2, 0);
	simulateRobot(2, 2, 100);
	simulateRobot(4, 2, 100);
}
