// date: 11/08/2023
int incomingByte = 0;
int previousTime = 0;
int currentTime = 0;
int interval = 0;
bool ledIsOn = false;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read() - 48;

    if(incomingByte >= 0 && incomingByte <= 9){
      Serial.print("I received: ");
      Serial.print(incomingByte);

      interval = incomingByte*100;
      Serial.print(" -  Interval is:");
      Serial.println(interval);

      previousTime = currentTime;
    } else {
      Serial.println("Invalid Input");
    }
  }

  currentTime = millis();

  if(currentTime - previousTime >= interval && interval > 0) {
    previousTime = currentTime;
    if(ledIsOn){
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    ledIsOn = !ledIsOn;
  }

  delay(5);
}

