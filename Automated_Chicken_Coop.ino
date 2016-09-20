//Static Variables
byte red = 2, yellow = 3, green = 4; 
byte waterLLPin = 5;       //Water Level Low
byte waterLMPin = 6;      //Water Level Mid
byte waterLHPin = 7;      //Water Level High
byte waterLWLPin = 8;     //Water Level waterer Low
byte waterMBtempPin = A0;   //Water Main bucket Temp
byte watererBTtempPin = A1;   //Waterer Bucket Temp
byte lightSensorPin = A2;   //Light Sensor for determining sun set and rising.
byte batteryVoltagePin = A3;  //Battery Voltage 
byte coopAirTempPin = A4;     //Air Temp in coop
byte utiltiesAirTempPin = A5;   //Air Temp in utilties enclosure 
int waterLH, waterLM, waterLL, waterLWL, waterMBtemp, watererBTtemp, lightSensor, batteryVoltage, coopAirTemp, utiltiesAirTemp;

void setup() {
  // put your setup code here, to run once:
  waterLH = waterLM = waterLL = waterLWL = waterMBtemp = watererBTtemp = lightSensor = batteryVoltage = coopAirTemp = utiltiesAirTemp = 0;
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
//  sensorValue = analogRead(sensorValue);  
//  Serial.println(sensorValue); 
//  if(sensorValue > 750) {
//    flashLED(green, 1, 4);
//  }
//  else if(sensorValue <= 750 && sensorValue >= 250){
    flashLED(yellow, 1, 4);
 // }
//  else {
//     flashLED(red, 1, 4);
     
 // }
}
//**********************************Functions****************************************//
void flashLED(int colorPin, int times, int timeDelay)
{ //flashLED(red, 6, 25)
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
//d pins 2-10 A pin 0-3,6-7
