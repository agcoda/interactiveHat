
#include<Servo.h>


/****************motor controller prep******************/
//motor numbers
#define NONE           -1
#define NUM_SERVOS      3
#define SER_BSE 0
#define SER_TIP         1
#define SER_MOUTH       2


//pins numbers
#define SER_BSE_PIN      8
#define SER_TIP_PIN      12
#define SER_MOUTH_PIN    13

#define CCW 0
#define CW 1
#define UP 1
#define DOWN 0

//times
#define BTWN_MTR_DLY  200
#define TRN_DLY 10


//general defines

int motorDir, motor;

int motorToControl;
int motorAction;
int motorActionParam;

//initialize all servos to zero position
int serPos[NUM_SERVOS] = {0,0,0};

//motor limits
//{BASE, TIP, MOUTH}
const int serMin[NUM_SERVOS] = {0,0,0};
const int serStart[NUM_SERVOS] = {0,0,0};
const int serMax[NUM_SERVOS] = {40,70,45};

int serSpeed[NUM_SERVOS] = {1,4,4};
int serSpeedMax[NUM_SERVOS] = {4,5,5};
int serTurnSpeed[NUM_SERVOS] = {TRN_DLY*serSpeed[SER_BSE],
  TRN_DLY*serSpeed[SER_TIP], TRN_DLY*serSpeed[SER_MOUTH]};
//int turnSpeed = 200; // this is the delay between steps, >20

Servo servoOneController;
Servo servoTwoController;
Servo servoThreeController;
/***********end motor controller prep******************/

/***************Serial communication prep**************/
#define NUM_BTNS 11
#define NUM_STX 4
//ABXYLRSHCQW are all the buttons
char intComnds[NUM_BTNS] = "ABXYLRSHCQWZMV";
//ZMV are sticks/triggers
char decComnds[NUM_STX] = "ZMV";
char command, prevAction, val;
int prevMtr = NONE;

bool btnHeld = false;

//char command[SERLEN], prevAction[SERLEN];
int RXLED = 7;


/***********end Serial communication prep**************/


void setup() {
  pinMode(RXLED, OUTPUT); //output the led for debug
  //open the comms
  Serial.begin(9600);
  //blink(3,1.5);

  servoOneController.attach(SER_BSE_PIN);
	servoTwoController.attach(SER_TIP_PIN);
	servoThreeController.attach(SER_MOUTH_PIN);

	//reset the position of all motors to 0 on boot
  

  motorReset();


  //blink(3,1.5);
  //motorTest();
  

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

    //if its a known btn just need btn to perform the action
    for(int i=0;i <NUM_BTNS+NUM_STX;i++ )
      // if we get a new recognized btn we'll change what we're doing
      if(posCommand == intComnds[i]){
        command = posCommand; //probably not necessary to presserve the original command here
        Serial.print("Received:");
        Serial.print(command);
        Serial.print("\n");
        takeNewAction();
      }
      //if its not a btn check if its a stick which will send a tuple
      else if(posCommand == decComnds[i-NUM_BTNS]){
        //if it is a stick we need the second serial to see if its P or N
        command = posCommand;
        Serial.print("Received:");
        Serial.print(command);
        Serial.print("\n");
        val = Serial.read();
        Serial.print("Received val:");
        Serial.print(val);
        Serial.print("\n");
        takeNewAction();
      }
    


  }
  if(btnHeld){
    repeatAction();
  }

  //delay(100); //might need to slow down action taking?

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
      switch(command){
        case 'A':
          motor = SER_TIP;
          motorDir = CW;
          btnHeld = true;
          break;
        case 'B':
          motor = SER_TIP;
          motorDir = CCW;
          btnHeld = true;
          break;
        case 'X':
          motor = SER_MOUTH;
          motorDir = CW;
          btnHeld = true;
          break;
        case 'Y':
          motor = SER_MOUTH;
          motorDir = CCW;
          btnHeld = true;
          break;
        case 'L':
          motor = SER_BSE;
          motorDir = CW;
          btnHeld = true;
          break;
        case 'R':
          motor = SER_BSE;
          motorDir = CCW;
          btnHeld = true;
          break;

        //speed adjusters
        case 'Q':
          speedChange(prevMtr, UP);
          break;

        case 'W':
          speedChange(prevMtr, DOWN);
          break;
        case 'S':
          motorReset();
      }
      prevMtr = motor;
    }
    

    //repeating button should probably stop the action
    else {
      //Serial.println(serPos[motor]); // debug to get current pos of motor before stopping
      motor = NONE;
      btnHeld = false;
    }
    //new command was found so send it to the motor

    motorMover(motor, motorDir);


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
      serPos[i] -= serSpeed[i];
      if (serPos[i] <serStart[i]){
        serPos[i]=serStart[i];
      }
      motorStepper(i, serPos[i]);
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
  Serial.print("motor mover called: "); //debug
  Serial.print(serPos[mtr]);
  Serial.print("\n");
  if(mtr == NONE){
    return 0;
  }

  if (dir == CW){
    //check that we're not trying to move outside limit
    if(serPos[mtr] - serSpeed[mtr] >= serStart[mtr]){//check that its not going beyond limits 
      serPos[mtr] -= serSpeed[mtr];
    }
    //if we are going outside the limit, instead go limit
    else{
      serPos[mtr] = serStart[mtr];
    }
    motorStepper(mtr, serPos[mtr]);
    delay(serTurnSpeed[mtr]);//always delay a bit after motion or servo will freak
  }
  else if(dir == CCW){
    if(serPos[mtr] + serSpeed[mtr] <= serMax[mtr]){ 
      serPos[mtr] += serSpeed[mtr];
    } 
    else{
      serPos[mtr]=serMax[mtr];
    }
    motorStepper(mtr, serPos[mtr]);
    delay(serTurnSpeed[mtr]);
  }
  return 0;
}

void speedChange(int mtr, int dir){
  if (mtr == NONE){
    return 0;
  }

  if(dir == UP && (serSpeed[mtr]<serSpeedMax[mtr])){
    serSpeed[mtr] += 1;
  }
  else if (dir == DOWN && (serSpeed[mtr]>1)){
    serSpeed[mtr] -= 1;
  }
  //Update the turn speed to match, larger motions need more time
  serTurnSpeed[mtr]=serSpeed[mtr]*TRN_DLY;

  return 0;
}

//writes position to the correct controller based on mtr
void motorStepper(int mtr, int loc){
        switch(mtr){
          case SER_BSE:
          Serial.println("Mtr step 1 called"); //debug
          servoOneController.write(loc);
          break;

          case SER_TIP:
          Serial.println("Mtr step 2 called"); //debug
          servoTwoController.write(loc);
          break;

          case SER_MOUTH:
          Serial.println("Mtr step 3 called"); //debug
          servoThreeController.write(loc);
          break;
      }
}

//cycle all servos through start to max and back
void motorTest(){
    for(int i=0;i<NUM_SERVOS;i++){
    Serial.println("moving motor: ");
    Serial.println(i);
    while(serPos[i]<serMax[i]){
      motorMover(i, CCW);
    }

    delay(BTWN_MTR_DLY);
    while(serPos[i]>serStart[i]){
      motorMover(i, CW);
    }
    delay(BTWN_MTR_DLY);
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