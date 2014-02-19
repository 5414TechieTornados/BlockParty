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

bool right=false;
bool full=true;
int scoreTol=1;
int parkTol=2;
int delay=0;
bool set = false;

task runMenu()
{
	void* currVar;
	char currType;

	currVar = &right;
	currType = 'b';

	while(true){
		if(delay<0){
			delay=15;
		} else if(delay>15){
			delay = 0;
		}
		if(scoreTol<0){
			scoreTol = 5;
		} else if(scoreTol>5){
			scoreTol = 0;
		}
		if(parkTol<0){
			parkTol = 5;
		} else if(parkTol>5){
			parkTol = 0;
		}



		nxtDisplayString(0,"Side     : %s",right?"right":"left ");
		nxtDisplayString(1,"Type     : %s",full?"first":"full  ");
		nxtDisplayString(2,"Score Tol: %i",scoreTol);
		nxtDisplayString(3,"Park Tol : %i",parkTol);
		nxtDisplayString(4,"Delay    : %i",delay);
		nxtDisplayString(5,"Finalize?: %s",set?"Yes":"No ");

		if(currVar == &right)
		{
			nxtDisplayStringAt(94,63,"*");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &full){
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55,"*");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &scoreTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47,"*");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &parkTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39,"*");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &delay){
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31,"*");
			nxtDisplayStringAt(94,23," ");
		}	else{
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23,"*");
		}



		//this section is copied from JoystickDriver.c with changed line numbers
		if ( externalBatteryAvg < 0){
			nxtDisplayTextLine(6, "Ext Batt : OFF");       //External battery is off or not connected
		} else {
			nxtDisplayTextLine(6, "Ext Batt :%4.1f V", externalBatteryAvg / (float) 1000);
		}
		nxtDisplayTextLine(7, "NXT Batt :%4.1f V", nAvgBatteryLevel / (float) 1000);   // Display NXT Battery Voltage

		if((nNxtButtonPressed==NEXT_BTN||nNxtButtonPressed==PREV_BTN) && (!set || currVar == &set)){
			if(currType=='b'){
				switchBool(currVar,nNxtButtonPressed);
			} else if (currType=='i') {
				if(currVar == &scoreTol){
					nxtDisplayString(2,"Score Tol:    ",scoreTol);
				}
				if(currVar == &parkTol){
					nxtDisplayString(3,"Park Tol :    ",parkTol);
				}
				if(currVar == &delay){
					nxtDisplayString(4,"Delay    :    ",delay);
				}
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
			} else if(currVar == &delay) {
				currVar = &set;
				currType = 'b';
			}else {
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
	nxtDisplayTextLine(7,"%s,%s,%i,%i,%i",right?"R":"L",full?"Ful":"1st",scoreTol,parkTol,delay);
}
