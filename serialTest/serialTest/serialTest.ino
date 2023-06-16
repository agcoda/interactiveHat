
const int NUM_COMS = 11;

char knwnComs[NUM_COMS] = "ABXYLRSHCQW";

char command, prevAction;

//char command[SERLEN], prevAction[SERLEN];
int RXLED = 7;

void setup() {
  pinMode(RXLED, OUTPUT); //output the led for debug
  //open the comms
  Serial.begin(9600);
  blinkLED(3,1.5);

}

void loop() {
  //when data available, grab it one at a time
  serialRead();
  
  //takeAction();
  
}

void serialRead(){
  int i =0;
  char posCommand = 'N';
  //while serial, check for one of our known commands
  while (Serial.available() ){
      posCommand = Serial.read();

      //if its a known command perform the action
      for(int i=0;i < NUM_COMS;i++ )
      
        if(posCommand == knwnComs[i]){
          command = posCommand; //probably not necessary to presserve the original command here
          takeAction();
        }

    }
}

//Once serial returns a message we take the action
void takeAction(){ 
  //check if its repeated action
  bool repeat = false;
  
  if(command == prevAction){
    repeat = true;
  }
  
  
  //Check that something was properly written
  if (command != 'N'){
    //if this is a new command
    if(repeat == false){
      if (command == 'A'){
        blinkLED(1,1);
      }
      else if(command == 'B'){
        blinkLED(2,1);
      }
      else if(command == 'X'){
        blinkLED(3,1);
      }
      else if(command == 'Y'){
        blinkLED(4,1);
      }
      else if(command == 'R'){
        blinkLED(5,1);

      }
      else if(command == 'L'){
        blinkLED(6,1);
      }
    }
    //repeating command should probably stop the action
    else {
      blinkLED(3,3);
    }
    //remember the action we took so we can stop the action on duplicate press (releasing button)

    prevAction = command;

    Serial.write(command);

  }
  return 0;
}

//Simple LED blinker for debugging
void blinkLED(int times, float speed){
  for(int i=0;i<times;i++){
    delay(speed*100);
    digitalWrite(RXLED, HIGH);
    delay(speed*100);
    digitalWrite(RXLED, LOW);
  }
}