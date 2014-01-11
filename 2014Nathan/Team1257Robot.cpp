#include "Team1257Robot.h"
#include <Wait.h>
#include <cstdarg>
Team1257Robot::Team1257Robot():
team1257Robot(LEFT,RIGHT),
LeftStick(LEFT),
RightStick(RIGHT),
armServo1(LARM),
armServo2(RARM),
armBend1(LBEND),
armBend2(RBEND)
{
	team1257LCD = DriverStationLCD::GetInstance();
	team1257Robot.SetExpiration(.1);
	leftStickIsEnabled = false;
	rightStickIsEnabled = false;
	SetWhichDrive(TANK_DRIVE);
}
void Team1257Robot::Autonomous()
{
	while(IsAutonomous())
	{
		Drive(.3,.3);
		Wait(4);
		Drive(0,0);
		while(true);
	}
}
void Team1257Robot::OperatorControl()
{
	CheckSetDriveSticks();
	this->printf("Driving with %i",this->GetWhichDrive());
	while(IsOperatorControl())
	{
		switch(GetWhichDrive())
		{
		case TANK_DRIVE:
			TankDrive();
			break;
		case ARCADE_DRIVE:
			ArcadeDrive();
			break;
		case ZTWIST_DRIVE:
			ZTwistDrive();
			break;
		case XY_DRIVE:
			XYDrive();
			break;
		}
		//TODO Handle Servos
	}
}
void Team1257Robot::TankDrive()
{
	if(this->leftStickIsEnabled && this->rightStickIsEnabled)
		this->GetDrive()->SetLeftRightMotorOutputs(-.5*this->GetLeftStick()->GetY(),-.5*this->GetRightStick()->GetY());
	else if (this-leftStickIsEnabled)
		this->GetDrive()->SetLeftRightMotorOutputs(-.5*this->GetLeftStick()->GetY(),-.5*this->GetLeftStick()->GetTwist());
}
void Team1257Robot::ArcadeDrive()
{
	if(this->rightStickIsEnabled && this->leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),1,this->GetRightStick(),2,false);
	}
	else if (this->leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),false);
	}
	else if (this->rightStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick(),false);
	}
	else this->Drive(0,0);
}
void Team1257Robot::ZTwistDrive()
{
	if(rightStickIsEnabled)
	{
		team1257Robot.ArcadeDrive(RightStick.GetZ(), RightStick.GetTwist(),false);
	}
	else if (leftStickIsEnabled)
	{
		team1257Robot.ArcadeDrive(LeftStick.GetZ(),LeftStick.GetTwist(),false);
	}
}
void Team1257Robot::XYDrive()
{
	if(rightStickIsEnabled)
	{
		team1257Robot.ArcadeDrive(RightStick.GetX(), RightStick.GetY(),false);
	}
	else if (leftStickIsEnabled)
	{
		team1257Robot.ArcadeDrive(LeftStick.GetX(),LeftStick.GetY(),false);
	}
}
void Team1257Robot::Drive(float left, float right)
{
	this->GetDrive()->SetLeftRightMotorOutputs(-left, -right);
}
int Team1257Robot::GetWhichDrive()
{
	return this->whichDrive;
}
void Team1257Robot::SetWhichDrive(int value)
{
	this->whichDrive = value;
}
void Team1257Robot::CheckSetDriveSticks()
{
	if(this->GetLeftStick()->GetRawButton(5) && this->GetLeftStick()->GetRawButton(6))
	{
		SetWhichDrive(TANK_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(1) && this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(TANK_DRIVE);
		rightStickIsEnabled = leftStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		rightStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		rightStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		rightStickIsEnabled = true;
	}
}
void Team1257Robot::SetArmsX(float value)
{
	this->GetServoInstanceFromNumber(1)->Set(value);
	this->GetServoInstanceFromNumber(2)->Set(-value);
}
void Team1257Robot::SetArmsY(float value)
{
	this->GetServoInstanceFromNumber(3)->Set(value);
	this->GetServoInstanceFromNumber(4)->Set(value);
}
void Team1257Robot::printf(char * Template , ...)
{
	va_list subs;
	va_start(subs, Template);
	this->GetLCD()->VPrintf(this->GetLCD()->kUser_Line1,1,Template,subs);
	va_end(subs);
}
RobotDrive * Team1257Robot::GetDrive()
{
	return &(this->team1257Robot);
}
Joystick * Team1257Robot::GetRightStick()
{
	return &(this->RightStick);
}
Joystick * Team1257Robot::GetLeftStick()
{
	return &(this->LeftStick);
}
Servo * Team1257Robot::GetServoInstanceFromNumber(int num)
{
	switch(num)
	{
	case 1:
		return &(this->armServo1);
		break;
	case 2:
		return &(this->armServo2);
		break;
	case 3:
		return &(this->armBend1);
		break;
	case 4:
		return &(this->armBend2);
		break;
	default:
		return &armServo1;
		break;
	}
}
DriverStationLCD * Team1257Robot::GetLCD()
{
	return this->team1257LCD;
}
RobotBase * FRC_userClassFactory() 
{ 
	return new Team1257Robot(); 
} 
extern "C" { 
	int32_t FRC_UserProgram_StartupLibraryInit() 
	{ 
		RobotBase::startRobotTask((FUNCPTR)FRC_userClassFactory); 
		return 0; 
	} 
}