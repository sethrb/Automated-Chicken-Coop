//Includes
#include <LiquidCrystal.h>
//-----------------Static Variables--------------------------------------------
//-----------------LED Pins ---------------------------------------------------
//byte red = 2, yellow = 3, green = 4; 
//                 Sensor Pins ------------------------------------------------
byte waterLLPin = 6;       //Water Level Low
byte waterLMPin = A7;      //Water Level Mid
byte waterLHPin = A6;      //Water Level High
byte waterLWLPin = 8;     //Water Level waterer Low
byte waterMBtempPin = A0;   //Water Main bucket Temp
byte watererBTtempPin = A1;   //Waterer Bucket Temp
byte lightSensorPin = A2;   //Light Sensor for determining sun set and rising.
byte batteryVoltagePin = A3;  //Battery Voltage 
byte coopAirTempPin = A4;     //Air Temp in coop
byte utiltiesAirTempPin = A5;   //Air Temp in utilties enclosure
//-----------------Sensor Variables------------------------------------------- 
int waterLH, waterLM, waterLL, waterLWL, waterMBtemp, watererBTtemp, lightSensor, batteryVoltage, coopAirTemp, utiltiesAirTemp;
//-----------------Display Pins-----------------------------------------------
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//d pins 2-10 A pin 0-3,6-7
/* 
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

void setup() {  // put your setup code here, to run once:

  lcd.begin(16, 2); //Initalize the LCD with 16 col and 2 rows
  waterLH = waterLM = waterLL = waterLWL = waterMBtemp = watererBTtemp = lightSensor = batteryVoltage = coopAirTemp = utiltiesAirTemp = 0;
 // pinMode(red, OUTPUT); pinMode(yellow, OUTPUT);  pinMode(green, OUTPUT);
  Serial.begin(19200);
  while (!Serial)
 // flashLED(red, 6, 25); //if can read card flash the red led.
 // flashLED(yellow, 4, 50);
 // flashLED(green, 5, 75);
 
}

void loop() { // put your main code here, to run repeatedly:
	//get light sensor value
	//figure out math for when to turn on lights 
	//check arduino internal time for refrence
	//get water bucket levels
	//get battery levels
	//get water temperurate 
	//get air temp
	//get battery levels
	//Display info on LCD

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
