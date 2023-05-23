//tutorial for stepper motor
//https://howtomechatronics.com/tutorials/arduino/how-to-control-stepper-motor-with-a4988-driver-and-arduino/

//New model has three servos

#include<Servo.h>


//ASCII things
#define ZEROVAL 48

/*Pseudo code
get signal from raspi
switch()
step motor 1
step motor 2
step motor 3

*/

//motor numbers
#define servoOne    1
#define servoTwo    2
#define servoThree  3

//location of values within transmitted char arrays
#define MOTORNUMLOC 0
#define MOTORACTLOC 2

//MotorActions
#define resetPosition  0

//pins numbers

#define servoOnePin      5
#define servoTwoPin      6
#define servoThreePin    9

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

//general defines


int motorToControl;
int motorAction;
int motorActionParam;

int i;
char tempCharBuffer;

String inputStr = String();
const byte numChars = 32;
char rcvBuff[numChars];

boolean newData = false;

int servoOnePosition = 0;
int servoTwoPosition = 0;
int servoThreePosition = 0;

int movementMax = 0;
int movementMin = 0;
int speed = 0;

Servo servoOneController;
Servo servoTwoController;
Servo servoThreeController;



void setup() {
	Serial.begin(9600);
	while (!Serial);

	//Start serial comms for pi to arduino
	Serial.println("READY");
	Serial.flush();

	servoOneController.attach(servoOnePin);
	servoTwoController.attach(servoTwoPin);
	servoThreeController.attach(servoThreePin);

	//reset the position of all motors to 0 on boot
	  //give enough time to move so they aren't all running at once

	servoOneController.write(0);
	delay(1000);
	servoTwoController.write(0);
	delay(1000);
	servoThreeController.write(0);
	delay(1000);

}
void loop() {
//	recvSerial();
//	Serial.flush();

	scrollLeftRight(RIGHT);
	scrollLeftRight(LEFT);

}

void scrollLeftRight(int dir) {
	motorToControl = servoOne;

	switch (dir) {
	case RIGHT:
		movementMax = 50;
		speed = 2;
		for (motorActionParam = 0; motorActionParam <= movementMax; motorActionParam += speed) {
			motorMover();
		}
		delay(1000);
		break;
	case LEFT:		
		movementMin = 0;
		speed = 2;
		for (motorActionParam = 50; motorActionParam >= movementMin; motorActionParam -= speed) {
			motorMover();
		}
		delay(1000);
		break;
	default:
		Serial.println("Direction invalid");

		break;
	}
	movementMax = 50;
	speed = 2;
	for (motorActionParam = 0; motorActionParam <= movementMax; motorActionParam+=speed) {
		motorMover();
	}
}

void recvSerial() {
	inputStr = "no input";
	//look for serial input
	static byte ndx = 0;
	char endMarker = '\n';
	char rc;

	while (Serial.available() > 0 && newData == false) {
		rc = Serial.read();
		if (rc != endMarker) {
			rcvBuff[ndx] = rc;
			ndx++;
			if (ndx >= numChars) {
				ndx = numChars - 1;
			}
		}
		else {
			rcvBuff[ndx] = '\0';
			ndx = 0;
			newData = true;
		}
	}


	if (newData == true) {
		//  for(i=0;i<5;i++){
		//Serial.println(rcvBuff[i]);
		//  }
		//Format of motor,motoract as xxx (000-120)
		motorToControl = (int)rcvBuff[MOTORNUMLOC] - ZEROVAL;
		motorActionParam = ((int)rcvBuff[MOTORACTLOC] - ZEROVAL) * 100 + ((int)rcvBuff[MOTORACTLOC + 1] - ZEROVAL) * 10 + ((int)rcvBuff[MOTORACTLOC + 2] - ZEROVAL);
		/*
		 Serial.print(" motorToControl:");
		 Serial.println(motorToControl);
		 Serial.print("motorActionParam:");
		 Serial.println(motorActionParam);
		 */

		motorMover();
		newData = false;
	}

	delay(100);
}

void motorMover() {
	switch (motorToControl) {
	case servoOne: //servo is the servo
	//Serial.println("moving servo 1");
	  //servo only takes a positional command. Just .write it to the controller
		servoOneController.write(motorActionParam);
		break;

	case servoTwo: //servo is the servo
	//Serial.println("moving servo 2");
		servoTwoController.write(motorActionParam);
		break;

	case servoThree: //servo is the servo
	//Serial.println("moving servo 3");
	  //servo only takes a positional command. Just .write it to the controller
		servoThreeController.write(motorActionParam);
		break;

	default:
		Serial.println("Invalid operation");
		break;


	}

}

void moveUpDown() {
	motorToControl = servoTwo;
	
	switch (dir) {
	case UP: