#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  ballLow,        sensorTouch)
#pragma config(Sensor, dgtl2,  ballHigh,       sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           feeder,        tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           LUflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           LDflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           LBMdrive,      tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           LFdrive,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           RBMdrive,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           RFdrive,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           RDflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port9,           RUflywheel,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          intake1,       tmotorVex393TurboSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
int motorSpeed = 0;
float velocity;
float flywheelSetpoint = 130;
float flywheelKp = .2;
int position1;
int position2;
bool ballIsInMiddle;

task flywheelVelocity(){
	while(true){
		position1 = nMotorEncoder(RDflywheel);
		wait1Msec(200);
		position2 = nMotorEncoder(RDflywheel);
		wait1Msec(200);
		velocity = (position2 - position1);
	}
}

task flywheelP(){
	float error;

	startTask(flywheelVelocity);

	while(true){

		error = flywheelSetpoint - velocity;
		motorSpeed += (flywheelKp*error);

		if(motorSpeed > 127)
			motorSpeed = 127;
		else if (motorSpeed < 80)
			motorSpeed = 80;

		motor[LDflywheel] = motorSpeed;
		motor[LUflywheel] = motorSpeed;
		motor[RDflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;
		wait1Msec(35);
	}
}

void speedUpFlywheel(){
	while(motorSpeed < 90){
		if(motorSpeed < 90)
			motorSpeed += 2;
		else
			motorSpeed = 90;

		motor[LUflywheel] = motorSpeed;
		motor[LDflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;

		wait1Msec(250);
	}
	//startTask(flywheelP);
}


void slowDownFlywheel(){
	if(motorSpeed > 0)
		motorSpeed -= 2;
	else
		motorSpeed = 0;

	motor[LUflywheel] = motorSpeed;
	motor[LDflywheel] = motorSpeed;
	motor[RUflywheel] = motorSpeed;
	motor[RDflywheel] = motorSpeed;

	wait1Msec(250);
}

task shooter(){
	while(true){
		if(vexRT(Btn6U)){
			while(!vexRT(Btn6D))
				speedUpFlywheel();
		}
		else if(vexRT(Btn6D)){
			while(!vexRT(Btn6U))
				slowDownFlywheel();
		}
		wait1Msec(25);
	}
}



	task drive(){
		while(true){
			motor[LFdrive] = vexRT(Ch3);
			motor[LBMdrive] = vexRT(Ch3);
			motor[RFdrive] = vexRT(Ch2);
			motor[RBMdrive] = vexRT(Ch2);
			wait1Msec(25);
		}
	}

	task intake(){
		ballIsInMiddle = false;
		bool feederToggle = false;
		while(true){
			if(vexRT(Btn5U)){
				motor[intake1] = 127;
				/* UNCOMMENT THIS CODE WHEN SENSORS INSTALLED
				if(SensorValue[ballLow])
				ballIsInMiddle = true;
				else
				ballIsInMiddle = false;
				if(ballHigh) {
				motor[intake1] = 127;
				wait1Msec(200);
				while(!SensorValue[ballHigh]) { wait1Msec(25); }
				motor[intake1] = 0;
				} else {
				motor[intake1] = 127;
				while(!SensorValue[ballHigh]) { wait1Msec(25); }
				motor[intake1] = 0;
				}
				*/
				wait1Msec(30);
			}
			else if(vexRT(Btn5D)){
				feederToggle = !feederToggle;
			}
			else {
				motor[intake1] = 0;
				motor[feeder] = 0;
			}
			if(feederToggle)
				motor[feeder] = 90;
			else
				motor[feeder] = 0;
			wait1Msec(25);
		}
	}

	void pre_auton()
	{
		bStopTasksBetweenModes = true;
	}


	task autonomous()
	{
		AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
	}

	task usercontrol()
	{
		startTask(shooter);
		startTask(drive);
		startTask(intake);
		startTask(flywheelVelocity);
		//speedUpFlywheel();

		while(true){
			if(vexRT(Btn8U)){
				motor[LUflywheel] = 127;
				motor[LDflywheel] = 127;
				motor[RUflywheel] = 127;
				motor[RDflywheel] = 127;
			}
			else if(vexRT(Btn8D)){
				motor[LDflywheel] = 90;
				motor[RUflywheel] = 90;
				motor[RDflywheel] = 90;
			}


			if(vexRT(Btn8L)){
				stopTask(flywheelP);
				slowDownFlywheel();
			}
			wait1Msec(25);
		}
	}
