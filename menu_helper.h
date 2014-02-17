TButtons NEXT_BTN = kRightButton;
TButtons PREV_BTN = kLeftButton;
TButtons CAT_BTN = kEnterButton;

void switchBool(bool *ptr, TButtons btn)
{
	if(btn == NEXT_BTN||btn == PREV_BTN)
	{
		*ptr=!*ptr;
	}
}

void switchInt(int *ptr, TButtons btn)
{
	if(btn == NEXT_BTN){
		*ptr=*ptr+1;
	} else if(btn == PREV_BTN) {
		*ptr=*ptr-1;
	}
}

bool right=true;
bool full=true;
int scoreTol=0;
int parkTol=0;
int delay=0;

task runMenu()
{
	void* currVar;
	char currType;

	currVar = &right;
	currType = 'b';

	while(true){
		if(delay<0){
			delay=0;
		} else if(delay>10){
			delay = 10;
		}
		if(scoreTol<0){
			scoreTol = 0;
		} else if(scoreTol>15){
			scoreTol = 15;
		}
		if(parkTol<0){
			parkTol = 0;
		} else if(parkTol>5){
			parkTol = 5;
		}



		nxtDisplayString(0,"Side:    %s",right?"right":"left ");
		nxtDisplayString(1,"Type: %s",full?"first":"full  ");
		nxtDisplayString(2,"Score Tolerance:   %2i",scoreTol);
		nxtDisplayString(3,"Park Tolerance:   %3i",parkTol);
		nxtDisplayString(4,"Delay:   %2i",delay);

		nxtDisplayTextLine(7,"%s,%s,%i,%i,%i",right?"R":"L",full?"In":"Out",scoreTol,parkTol,delay);

		if(currVar == &right)
		{
			nxtDisplayStringAt(94,63,"*");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
		} else if(currVar == &full){
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55,"*");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
		} else if(currVar == &scoreTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47,"*");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
		} else if(currVar == &parkTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39,"*");
			nxtDisplayStringAt(94,31," ");
		}
		else{
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31,"*");
		}


		//this section is copied from JoystickDriver.c with changed line numbers
		if ( externalBatteryAvg < 0){
			nxtDisplayTextLine(5, "Ext Batt: OFF");       //External battery is off or not connected
		} else {
			nxtDisplayTextLine(5, "Ext Batt:%4.1f V", externalBatteryAvg / (float) 1000);
		}
		nxtDisplayTextLine(6, "NXT Batt:%4.1f V", nAvgBatteryLevel / (float) 1000);   // Display NXT Battery Voltage

		if(nNxtButtonPressed==NEXT_BTN||nNxtButtonPressed==PREV_BTN){
			if(currType=='b'){
				switchBool(currVar,nNxtButtonPressed);
			} else if (currType=='i') {
				switchInt(currVar,nNxtButtonPressed);
			}
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
		}

		if(nNxtButtonPressed==CAT_BTN){
			if(currVar == &right){
				currVar = &full;
				currType = 'b';
			} else if(currVar == &full){
				currVar = &scoreTol;
				currType = 'i';
			} else if(currVar == &scoreTol) {
				currVar = &parkTol;
				currType = 'i';
			} else if(currVar == &parkTol) {
				currVar = &delay;
				currType = 'i';
			} else {
				currVar = &right;
				currType = 'b';
			}
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
		}
	}
}

void displayAutoType()
{
	nxtDisplayTextLine(7,"%s,%s,%i,%i,%i",right?"R":"L",full?"In":"Out",scoreTol,parkTol,delay);
}
