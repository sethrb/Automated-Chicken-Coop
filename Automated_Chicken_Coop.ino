//Includes
#include <LiquidCrystal.h>
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
//-----------------Static Variables--------------------------------------------
//-----------------Sensor Pins ------------------------------------------------
byte waterLLPin = 6;       //Water Level Low
byte waterLMPin = 7;      //Water Level Mid
byte waterLHPin = 9;      //Water Level High
byte waterLWLPin = 10;     //Water Level waterer Low
byte waterMBtempPin = A0;   //Water Main bucket Temp
byte watererBTtempPin = A1;   //Waterer Bucket Temp
byte lightSensorPin = A2;   //Light Sensor for determining sun set and rising.
byte batteryVoltagePin = A3;  //Battery Voltage 
byte coopAirTempPin = A6;     //Air Temp in coop
byte utilitiesAirTempPin = A5;   //Air Temp in utilities enclosure
byte displayOnPin = 13; //Button to turn on and off display LED
//-----------------control Pins------------------------------------------------
byte lightRelay = 8; // Relay for the lights
//Pins open d 1,13,
//A4&&A5 are reserved for SLA and SLC 
//-----------------Sensor Variables--------------------------------------------
int waterLH, waterLM, waterLL, waterLWL, waterMBtemp, watererBTtemp; 
int batteryVoltage, coopAirTemp, utilitiesAirTemp, lightThreshhold, lightSensor; 
//-----------------other Variables---------------------------------------------
byte night, morning, onTime, offTime, displayOn;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
unsigned long timerA, timerB, timerC, timerD; //ALWAYS use unsigned long for timers...
//-----------------Display Pins------------------------------------------------
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte smiley[8] = {
	B00000, B10001, B00000, B00000, B10001, B01110, B00000,};
byte light[8] = {
	B00100, B10101, B01110, B11111, B01110, B10101, B00100,};
byte bucketEmpty[8] = {
	B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111, };
byte bucketMid[8] = {
	B10001, B10001, B10001, B11111, B11111, B11111, B11111, B11111, };
byte bucketFull[8] = {
	B10001, B11111, B11111, B11111, B11111, B11111, B11111, 0B11111, };
byte bucketNo[8] = {
	B11011, B10101, B11011, B11011, B10101, B11011, B11011, B10101, };

void setup() {  // put your setup code here, to run once:
	waterLH = waterLM = waterLL = waterLWL = waterMBtemp = watererBTtemp = 0;
	batteryVoltage = coopAirTemp = utilitiesAirTemp = night = morning = timerC = 0;
	second = minute = hour = dayOfWeek = dayOfMonth = month = year = lightSensor = 0;
	lightThreshhold = 200; //Light Threshold for determining dark
	onTime = 5;
	offTime = 9;
	Wire.begin(); 		//Initalize the i2c communication bus
	lcd.begin(16, 2); 	//Initalize the LCD with 16 col and 2 rows
	lcd.createChar(0, smiley); //Create the LCD Custom Images
	lcd.createChar(1, light);
	lcd.createChar(2, bucketEmpty);
	lcd.createChar(3, bucketMid);
	lcd.createChar(4, bucketFull);
	lcd.createChar(5, bucketNo);

	//analog pin setup
	lightSensor = analogRead(lightSensorPin); waterLM = analogRead(waterLMPin);
	waterLH = analogRead(waterLHPin); waterMBtemp = analogRead(waterMBtempPin);
	watererBTtemp = analogRead(watererBTtempPin); batteryVoltage = analogRead(batteryVoltagePin);
	coopAirTemp = analogRead(coopAirTempPin); utilitiesAirTemp = analogRead(utilitiesAirTempPin);	
	//digital pin setup
	pinMode(waterLLPin,INPUT); pinMode(waterLWLPin, INPUT); pinMode(displayOnPin, INPUT);
	pinMode(lightRelay, OUTPUT); pinMode(0, OUTPUT);
	Serial.begin(19200);
	while (!Serial); 
}

void loop() { // put your main code here, to run repeatedly:
  int holdlightsensor, loopRunTime;
  unsigned long previousMillis;
  previousMillis = millis();
	if(timerC == 0) timersToTime();
  displayOn = 1;
  if(displayOn){digitalWrite(0, HIGH);}
  else{digitalWrite(0,LOW);}
 /* 
  if(digitalRead == 1){
    if(displayOn){displayOn = 0;}
    if 
  }
*/
  
	lightSensor = analogRead(lightSensorPin);
	
	if (lightSensor < lightThreshhold){
		digitalWrite(lightRelay, HIGH);
		timerA = millis();
	}
	else if (lightSensor > lightThreshhold && (millis() - timerA) > 200){
      digitalWrite(lightRelay, LOW);
      timerA = millis();
	}
	// print out the value you read:
	Serial.println(lightSensor);	
//z	if ()
	
	//get water bucket levels
	//get battery levels
	//get water temperurate 
	//get air temp
	//get battery levels
	//Display info on LCD
	loopRunTime = millis() - previousMillis;
  
	if ((millis() - timerB) > 200){
		displayInfo(loopRunTime);
    timerB = millis();
	}
}
//**********************************Functions********************************//
//----------------------------------Display----------------------------------//
void displayInfo(int mode)
{
	lcd.clear(); //clear the screen in prep for writing
	lcd.write(byte(1));       	  	//light Icon
	lcd.write(" = ");
	lcd.print(lightSensor);
	lcd.setCursor(0,1); //col,row
	lcd.write("Water = ");
	lcd.write(byte(2));				//Bucket Empty
	lcd.write(" ");
	lcd.write(byte(0));				//Smilily face
	lcd.write(" ");
	lcd.write(mode);
}
//flash LED------------------------------------------------------------------//
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
byte setNight()
{
	readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
	if (hour < offTime && hour > onTime)
		return 1;
	else {
		return 0;
	}
}
byte setDark()
{
	lightSensor = analogRead(lightSensorPin);
	if (lightSensor < lightThreshhold){
		return 1;	
	}
	else {
		return 0;
	}
}
void timersToTime(){
  timerA = millis();
  timerB = millis();
  timerC = millis();
  timerD = millis();
}
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
byte decToBcd(byte val) // Convert normal decimal numbers to binary coded decimal
{
  return( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)	// Convert binary coded decimal to normal decimal numbers
{
  return( (val/16*10) + (val%16) );
}
void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
