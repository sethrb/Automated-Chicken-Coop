//Static Variables
byte red = 2, yellow = 3, green = 4; 
byte sensorPin = A0;
int sensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT); pinMode(yellow, OUTPUT);  pinMode(green, OUTPUT);
  Serial.begin(19200);
  while (!Serial)
  flashLED(red, 6, 25); //if can read card flash the red led.
  flashLED(yellow, 4, 50);
  flashLED(green, 5, 75);
}

void loop() {
  // put your main code here, to run repeatedly:
//  flashLED(green, 5, 30);
  sensorValue = analogRead(sensorValue);  
  Serial.println(sensorValue); 
  if(sensorValue > 750) {
    flashLED(green, 1, 4);
  }
  else if(sensorValue <= 750 && sensorValue >= 250){
    flashLED(yellow, 1, 4);
  }
  else {
     flashLED(red, 1, 4);
     
  }
}
//**********************************Functions****************************************//
void flashLED(int colorPin, int times, int timeDelay)
{
  byte hold = 0;
    while(hold < times){
      digitalWrite(colorPin, HIGH);
      delay(timeDelay/2);
      digitalWrite(colorPin, LOW);
      delay(timeDelay/2);
      hold++;
    }
}
//      Serial.print("Writing to test.txt...");
//      LogFile.print(tempC); LogFile.print(","); 
//      Serial.println("done.");
