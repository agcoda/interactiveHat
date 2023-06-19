
#include<Servo.h>


/****************motor controller prep******************/
//motor numbers
#define NONE           -1
#define NUM_SERVOS      3
#define SER_BASE_SWIVEL 0
#define SER_TIP         1
#define SER_MOUTH       2


//pins numbers
#define SER_BASE_SWIVELPIN      8
#define SER_TIPPIN      12
#define SER_MOUTHPIN    13

#define CCW 0
#define CW 1

//times
#define BTWN_MTR_DLY  100


//general defines

int motorDir, motor;

int motorToControl;
int motorAction;
int motorActionParam;

//initialize all servos to zero position
int serPos[NUM_SERVOS] = {0,0,0};

//motor limits
//{BASE, TIP, MOUTH}
const int serMin[NUM_SERVOS] = {-10,0,0};
const int serStart[NUM_SERVOS] = {-10,60,0};
const int serMax[NUM_SERVOS] = {20,120,20};

int speed = 2; //this is the stepsize
int turnSpeed = 50; // this is the delay between steps

Servo servoOneController;
Servo servoTwoController;
Servo servoThreeController;
/***********end motor controller prep******************/

/***************Serial communication prep**************/
#define NUM_COMS 11
char knwnComs[NUM_COMS] = "ABXYLRSHCQW";
char command, prevAction;

bool btnHeld = false;

//char command[SERLEN], prevAction[SERLEN];
int RXLED = 7;


/***********end Serial communication prep**************/


void setup() {
  pinMode(RXLED, OUTPUT); //output the led for debug
  //open the comms
  Serial.begin(9600);
  //blink(3,1.5);

  servoOneController.attach(SER_BASE_SWIVELPIN);
	servoTwoController.attach(SER_TIPPIN);
	servoThreeController.attach(SER_MOUTHPIN);

	//reset the position of all motors to 0 on boot
  

  motorReset();


  //blink(3,1.5);

  for(int i=0;i<NUM_SERVOS;i++){
    Serial.println("moving motor: ");
    Serial.println(i);
    while(serPos[i]<serMax[i]){
      motorMover(i, CCW);
    }

    delay(1000);
    while(serPos[i]>serStart[i]){
      motorMover(i, CW);
    }
    delay(1000);
  }

}

void loop() {
  //when data available, grab it one at a time
  serialRead();
  
}
/***** Serial comms functions *****/
void serialRead(){
  int i =0;
  char posCommand = 'N';
  //while serial, check for one of our known commands
  while (Serial.available() ){
    posCommand = Serial.read();

    //if its a known command perform the action
    for(int i=0;i < NUM_COMS;i++ )
      // if we get a new recognized command we'll change what we're doing
      if(posCommand == knwnComs[i]){
        command = posCommand; //probably not necessary to presserve the original command here
        takeNewAction();
      }


  }
  if(btnHeld){
    repeatAction();
  }

  delay(100); // don't take actions too fast

}

//Once serial returns a message we take the action
void takeNewAction(){ 
  //check if its repeated button
  bool btnRepeat = false;
  
  if(command == prevAction){
    btnRepeat = true;
    prevAction = 'N'; // just want to switch on immediate duplicate, then forget
  }
  else{
    prevAction = command;

  }
  
  
  //Check that something was properly written
  if (command != 'N'){
    //if this is a new command
    if(btnRepeat == false){
      if (command == 'A'){
        motor = SER_TIP;
        motorDir = CW;
        btnHeld = true;
        //blink(1,1);
        
      }
      else if(command == 'B'){
        motor = SER_TIP;
        motorDir = CCW;
        btnHeld = true;
        //blink(2,1);
      }
      else if(command == 'X'){
        motor = SER_MOUTH;
        motorDir = CW;
        btnHeld = true;
        //blink(3,1);
      }
      else if(command == 'Y'){
        motor = SER_MOUTH;
        motorDir = CCW;
        btnHeld = true;
        //blink(4,1);
      }

      else if(command == 'L'){
        //blink(6,1);
        motor = SER_BASE_SWIVEL;
        motorDir = CW;
        btnHeld = true;
      }
      else if(command == 'R'){
        //blink(5,1);
        motor = SER_BASE_SWIVEL;
        motorDir = CCW;
        btnHeld = true;

      }
      else if(command == 'S'){
        motorReset();
      }
    }
    //repeating button should probably stop the action
    else {
      Serial.println(serPos[motor]); // debug to get current pos of motor before stopping
      //blink(3,3);
      motor = NONE;
      btnHeld = false;
    }
    //new command was found so send it to the motor
    Serial.println("moving motor: ");
    Serial.println(motor);
    delay(1000);
    motorMover(motor, motorDir);

    Serial.println(command);

  }
  return 0;
}

//just call the motor motion with the prev vars set by takeNewAction
void repeatAction(){
  motorMover(motor, motorDir);
}

/****** motor controlling functions*****/

void motorReset(){
  //may need to add code to slow this motion back to zero with stepping 
  //take current position -step until negative then set zero

  for(int i=0;i<NUM_SERVOS;i++){
    while (serPos[i] >serStart[i]){
      serPos[i] -= speed;
      if (serPos[i] <serStart[i]){
        serPos[SER_BASE_SWIVEL]=serStart[i];
      }
    motorStepper(SER_BASE_SWIVEL, serPos[SER_BASE_SWIVEL]);
    }
	delay(BTWN_MTR_DLY);
  }

  //initial zeroing, also duplicates a zero set for integrity
  for(int i=0; i<NUM_SERVOS; i++){  
    serPos[i]=serStart[i];  
    motorStepper(i, serPos[i]);
    delay(BTWN_MTR_DLY);
  }
}

//main mover for all servos
//may want to generalize for different speeds
void motorMover(int mtr, int dir) {
  //Serial.println("motor mover called"); //debug
  if (dir == CW){
    if(serPos[mtr] - speed >= serMin[mtr]){//check that its not going beyond limits 
      serPos[mtr] -= speed;
      motorStepper(mtr, serPos[mtr]);
      delay(turnSpeed);//always delay a bit after motion or servo will freak
    }
  }
  else if(dir == CCW){
    if(serPos[mtr] + speed <= serMax[mtr]){ 
      serPos[mtr] += speed;
      motorStepper(mtr, serPos[mtr]);
      delay(turnSpeed);
    } 
  }
}

//writes position to the correct controller based on mtr
void motorStepper(int mtr, int loc){
        switch(mtr){
          case SER_BASE_SWIVEL:
          servoOneController.write(loc);
          break;

          case SER_TIP:
          servoTwoController.write(loc);
          break;

          case SER_MOUTH:
          servoThreeController.write(loc);
          break;
      }
}


//Simple LED blinker for debugging
/*
void blink(int times, float rate){
  for(int i=0;i<times;i++){
    delay(rate*100);
    digitalWrite(RXLED, HIGH);
    delay(rate*100);
    digitalWrite(RXLED, LOW);
  }
}
*/