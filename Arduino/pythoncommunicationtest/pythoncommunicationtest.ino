// date: 15/08/2023
// aanpassing van 2dblinking, om te gebruiken bij extra software
int incomingByte = 0;
int move[4] = {0,0,0,0};
int writingAt = 0;
bool receivingData = true;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkMove(int move[4]){
  delay(1000);
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < move[i]; j++){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
    delay(500);
  }
}

void loop() {
  if (Serial.available() > 0) {
    if(!receivingData){
      Serial.print(0);
      return;
    }
    // read the incoming byte:
    incomingByte = Serial.read() - 48;

    if(incomingByte >= 1 && incomingByte <= 8){
      move[writingAt] = incomingByte;
      writingAt++;
      
      if(writingAt == 4){
        receivingData = false;
        writingAt = 0;
        blinkMove(move);
        receivingData = true;
        Serial.print(1);
      }

    } else {
      Serial.print(2);
    }
  }

  delay(5);
}
