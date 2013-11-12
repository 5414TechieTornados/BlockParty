int rightSpeedDefault = 3;
int leftSpeedDefault = 3;
int rightSpeed = rightSpeedDefault;
int leftSpeed = leftSpeedDefault;

task ButtonTask()
{
 nSchedulePriority	 = kHighPriority;

  //
  // Update count of times each button is pushed. If using this program as a template, replace these
  // statements with your own custom action.
  //
  switch (nNxtButtonPressed)
  {
  	case kExitButton:

			break;

  	case kLeftButton:
		  motor[motorB] = leftSpeed;
		  leftSpeed = leftSpeed + leftSpeed;
			break;

		case kRightButton:
		  motor[motorC] = rightSpeed;
		  rightSpeed = rightSpeed + rightSpeed;
			break;

		case kEnterButton:
		motor[motorC] = 0;
		motor[motorB] = 0;
		leftSpeed = leftSpeedDefault;
		rightSpeed = rightSpeedDefault;

	}
}

task main()
{
	//
	// "Hi-jack" buttons for user program control.
	//
	// Note: program cannot be terminated if we hijack the 'exit' button. So there has to be an escape sequence
	//       that will return buttons to system control!
	//
	nNxtExitClicks    = 2;
	nNxtButtonTask    = ButtonTask;

	nxtDisplayTextLine(2, "  < to Decrease");
  nxtDisplayTextLine(3, "  > to Increase");
  nxtDisplayTextLine(4, "  Enter to Stop");
  nxtDisplayTextLine(5, "  ExitX2 to Exit");
	//
	// Do nothing. Just keep waiting
	//
	while (true)
	{
		//
		// Some dummy code to do some work.
		//
	  wait1Msec(50);
  }
	return;
}
