// 12/08/2023 
// Aangepast van Examples/Analog/Fading.ino
int ledPin = 11;  
int speed = 10;

void loop() {
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += speed) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }

  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= speed) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
  analogWrite(ledPin, 0);

  delay(500); 
} 
