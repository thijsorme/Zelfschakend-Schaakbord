// 14/08/2023 
// Aangepast van Examples/Analog/Fading.ino
// Gebruikt met magneet in pin 7
const int ledPin = 2; 

int ledState = LOW;
unsigned long previousMillis = 0; 
const long interval = 400; 
void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
  }
}
