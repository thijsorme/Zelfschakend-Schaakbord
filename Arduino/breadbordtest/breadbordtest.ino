
// date: 12/08/2023
// int incomingByte = 0;
// int move[4] = {0,0,0,0};
// int writingAt = 0;
// bool receivingData = true;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(7, INPUT);
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
  int value = digitalRead(7);
  Serial.print(value);
  digitalWrite(LED_BUILTIN, (int)!(bool)value);
  delay(100);

  // if (Serial.available() > 0 && receivingData) {
  //   // read the incoming byte:
  //   incomingByte = Serial.read() - 48;

  //   if(incomingByte >= 1 && incomingByte <= 8){
  //     move[writingAt] = incomingByte;
  //     writingAt++;
      
  //     if(Serial.available() == 0){
  //       for(int i = 0; i < 4; i++){
  //         Serial.print(move[i]);
  //         Serial.print(" ");
  //       }
  //       Serial.print("; ");
  //       Serial.println(writingAt);
  //     }
      


  //     if(writingAt == 4){
  //       receivingData = false;
  //       writingAt = 0;
  //       blinkMove(move);
  //       receivingData = true;

  //     }

  //   } else {
  //     Serial.println("Invalid Input");
  //   }
  // }

  delay(5);
}

