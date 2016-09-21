//Includes
#include <LiquidCrystal.h>
//-----------------Static Variables--------------------------------------------
//-----------------LED Pins ---------------------------------------------------
//byte red = 2, yellow = 3, green = 4; 
//                 Sensor Pins ------------------------------------------------
byte waterLLPin = 6;       //Water Level Low
byte waterLMPin = A7;      //Water Level Mid
byte waterLHPin = A6;      //Water Level High
byte waterLWLPin = 7;     //Water Level waterer Low
byte waterMBtempPin = A0;   //Water Main bucket Temp
byte watererBTtempPin = A1;   //Waterer Bucket Temp
byte lightSensorPin = A2;   //Light Sensor for determining sun set and rising.
byte batteryVoltagePin = A3;  //Battery Voltage 
byte coopAirTempPin = A4;     //Air Temp in coop
byte utilitiesAirTempPin = A5;   //Air Temp in utilities enclosure
//-----------------Sensor Variables--------------------------------------------
int waterLH, waterLM, waterLL, waterLWL, waterMBtemp, watererBTtemp, lightSensor; 
int batteryVoltage, coopAirTemp, utilitiesAirTemp, lightThreshhold;
//other Variables
byte night = 0; byte morning = 0;
//-----------------Display Pins------------------------------------------------
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
	waterLH = waterLM = waterLL = waterLWL = waterMBtemp = watererBTtemp = lightSensor = 0;
	batteryVoltage = coopAirTemp = utilitiesAirTemp = 0;
	lightThreshhold = 200; 
	lcd.begin(16, 2); //Initalize the LCD with 16 col and 2 rows
	//analog pin setup
	lightSensor = analogRead(lightSensorPin); waterLM = analogRead(waterLMPin);
	waterLH = analogRead(waterLHPin); waterMBtemp = analogRead(waterMBtempPin);
	watererBTtemp = analogRead(watererBTtempPin); batteryVoltage = analogRead(batteryVoltagePin);
	coopAirTemp = analogRead(coopAirTempPin); utilitiesAirTemp = analogRead(utilitiesAirTempPin);	
	//digital pin setup
	pinMode(waterLLPin,INPUT); pinMode(waterLWLPin, INPUT);
	pinMode(8, OUTPUT);
	Serial.begin(19200);
	while (!Serial);
 // flashLED(red, 6, 25);
 // flashLED(yellow, 4, 50);
 // flashLED(green, 5, 75);
 
}

void loop() { // put your main code here, to run repeatedly:
	int holdlightsensor;
	lightSensor = analogRead(lightSensorPin);
	if (lightSensor < lightThreshhold && night != 0) {
		delay(1000);
		lightSensor = analogRead(lightSensorPin);
		if (lightSensor < lightThreshhold){
			lightSensor = analogRead(lightSensorPin);
			night = currentTimeMinutes();
			morning = night + 720;
		}
	}
	
	if (currentTimeMinutes() >= morning) {
		digitalWrite(8, HIGH);
	}
	
	
	// print out the value you read:
	Serial.println(lightSensor);	
	//figure out math for when to turn on lights 
	//check arduino internal time for refrence
	//get water bucket levels
	//get battery levels
	//get water temperurate 
	//get air temp
	//get battery levels
	//Display info on LCD

}
//**********************************Functions********************************//
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
//---------------------------Time till Daylight--------------------------------
//----------set next lights on time.
//int lightOn()
//{
//	lightSensor = analogRead(lightSensorPin);
	
	
//}


//not using a clock to provide info for now.
/* psudo code
see when it's dark 
wait twelve hours and then turn on lights

*/
//-----------currentTimeMinutes
int currentTimeMinutes()
{
	unsigned long time = 0;
	int minutes = 0;
	time = millis();
	minutes = time / 60000;
	return minutes;
}
//      Serial.print("Writing to test.txt...");
//      LogFile.print(tempC); LogFile.print(","); 
//      Serial.println("done.");
//		PWM: 3, 5, 6, 9, 10, and 11. Provide 8-bit PWM output with the analogWrite() function.
