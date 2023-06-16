int command = 0;
int RXLED = 7;

void setup() {
  pinMode(RXLED, OUTPUT); //output the led for debug
  //open the comms
  Serial.begin(9600);
  for(int i=0;i<3;i++){
    delay(500);
    digitalWrite(RXLED, HIGH);
    delay(500);
    digitalWrite(RXLED, LOW);
  }
}

void loop() {
  //when data available, grab it
  if (Serial.available() > 0){
    command = Serial.read();
    //flash the LED so we can say we got somethin
    digitalWrite(RXLED, LOW);
    delay(100);
    digitalWrite(RXLED, HIGH);
  }

  if (command != 0){
    Serial.write(command);
  }

}
