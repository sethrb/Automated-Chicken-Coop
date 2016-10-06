#include <LiquidCrystal.h>
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
//-----------------Static Variables--------------------------------------------
//-----------------Sensor Pins ------------------------------------------------
byte waterLLPin = 6;       //Water Level Low
byte waterLMPin = 13;      //Water Level Mid
byte waterLHPin = 9;      //Water Level High
byte waterLWLPin = 10;     //Water Level waterer Low
byte waterMBtempPin = A0;   //Water Main bucket Temp
byte watererBTtempPin = A1;   //Waterer Bucket Temp
byte lightSensorPin = A2;   //Light Sensor for determining sun set and rising.
byte batteryVoltagePin = A3;  //Battery Voltage 
byte coopAirTempPin = A6;     //Air Temp in coop
byte utilitiesAirTempPin = A5;   //Air Temp in utilities enclosure
byte lightOnPin = 7; //Button to turn on and off display LED
//-----------------control Pins------------------------------------------------
byte lightRelay = 8; // Relay for the lights
//Pins open d 1,13,
//A4&&A5 are reserved for SLA and SLC 
//-----------------Sensor Variables--------------------------------------------
int waterLH, waterLM, waterLL, waterLWL, waterMBtemp, watererBTtemp; 
int coopAirTemp, utilitiesAirTemp, lightThreshhold, lightSensor, loopRunTime;
float batteryVoltage;
//-----------------other Variables---------------------------------------------
byte night, morning, onTime, offTime, lightOn;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
unsigned long timerA, timerB, timerC, timerD; //ALWAYS use unsigned long for timers...
//-----------------Display Pins------------------------------------------------
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte smiley[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000,};
byte light[8] = {B00100, B10101, B01110, B11111, B01110, B10101, B00100,};
byte bucketEmpty[8] = {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111, };
byte bucketMid[8] = {B10001, B10001, B10001, B11111, B11111, B11111, B11111, B11111, };
byte bucketFull[8] = {B10001, B11111, B11111, B11111, B11111, B11111, B11111, 0B11111, };
byte bucketNo[8] = {B11011, B10101, B11011, B11011, B10101, B11011, B11011, B10101, };
byte clockIcon[8] = {B11111, B10001, B10011, B10101, B10101, B10101, B11111, B00000, };
  
void setup() {  // put your setup code here, to run once:
	waterLH = waterLM = waterLL = waterLWL = waterMBtemp = watererBTtemp = 0;
	batteryVoltage = coopAirTemp = utilitiesAirTemp = night = morning = timerC = 0;
	second = minute = hour = dayOfWeek = dayOfMonth = month = year = lightSensor = 0;
  loopRunTime = lightOn = 0;
	lightThreshhold = 200; //Light Threshold for determining dark
	onTime = 5;       //Time to turn on Coop Light
	offTime = 9;     //Time to turn off Coop light
	Wire.begin(); 		//Initalize the i2c communication bus
	lcd.begin(16, 2); 	//Initalize the LCD with 16 col and 2 rows
	lcd.createChar(0, smiley); //Create the LCD Custom Images
	lcd.createChar(1, light); lcd.createChar(2, bucketEmpty);
	lcd.createChar(3, bucketMid); lcd.createChar(4, bucketFull);
	lcd.createChar(5, bucketNo); lcd.createChar(6, clockIcon);
	//analog pin setup
	lightSensor = analogRead(lightSensorPin); waterLM = analogRead(waterLMPin);
	waterLH = analogRead(waterLHPin); waterMBtemp = analogRead(waterMBtempPin);
	watererBTtemp = analogRead(watererBTtempPin); batteryVoltage = analogRead(batteryVoltagePin);
	coopAirTemp = analogRead(coopAirTempPin); utilitiesAirTemp = analogRead(utilitiesAirTempPin);	
	//digital pin setup
	pinMode(waterLLPin,INPUT); pinMode(waterLWLPin, INPUT); pinMode(lightOnPin, INPUT);
	pinMode(lightRelay, OUTPUT); pinMode(0, OUTPUT);
  timersToTime();
	Serial.begin(230400);
	while (!Serial); 
}

void loop() { // put your main code here, to run repeatedly:
	int holdlightsensor;
	unsigned long previousMillis;
	previousMillis = millis();
  lightOn = digitalRead(lightOnPin);
  if (lightOn == 1){lightOn = 0;}
  else {lightOn = 1;} 

	if(millis() - timerC > 5000){ morning = setNight(); }//figure out if lights should come on.
	if(morning == 1) { digitalWrite(lightRelay, HIGH); }//If they should turn them on
  else if (morning == 0 && lightOn == 1) { digitalWrite(lightRelay, HIGH);}
  else {digitalWrite(lightRelay, LOW);}
		

	lightSensor = analogRead(lightSensorPin);
  /*  
	if (lightSensor < lightThreshhold){
		digitalWrite(lightRelay, HIGH);
		timerA = millis();
	}
 
	else if (lightSensor > lightThreshhold && (millis() - timerA) > 200 && morning == 0){
		digitalWrite(lightRelay, LOW);
		timerA = millis();
	}
	*/
	batteryVoltage = getBatteryVoltage();
	//get water bucket levels
	//get water temperurate 
	//get air temp
	//get battery levels
	
	
  
	if ((millis() - timerB) > 200){ //Display info on LCD and serial
		displayInfo(loopRunTime);
		serialInfo(loopRunTime);
      timerB = millis();
	}
  loopRunTime = millis() - previousMillis;
}
//**********************************Functions********************************//
//----------------------------------Display----------------------------------//
void displayInfo(int mode)
{
	//setup for display work
	readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
	float displayBatteryVoltage = getBatteryVoltage();
	lcd.clear(); //clear the screen in prep for writing
	//First Line
	lcd.write(byte(1));       	  	//light Icon
	lcd.print(lightSensor); lcd.print(" "); lcd.setCursor(5,0); //col,row
	lcd.write(byte(6)); lcd.print(String(mode));
	lcd.setCursor(8,0); //col,row
	lcd.print(String(morning));
	lcd.setCursor(10,0);
	lcd.print(String(displayBatteryVoltage));
	//Second Line
	lcd.setCursor(0,1); //col,row
	lcd.write(byte(2));				//Bucket Empty
	lcd.write(" ");
	lcd.print(String(month)); lcd.print("/"); lcd.print(String(dayOfMonth));
	lcd.print(" "); lcd.print(String(hour)); lcd.print(":"); 
	if(minute <= 9){lcd.print("0");} 
	lcd.print(String(minute)); lcd.print(" "); lcd.print(String(second));
}
//----------------------------------Serial Display---------------------------//
void serialInfo(int mode)
{
  int tempF = get_temperature(0);
  int counter = 0;
  int seconds = 0;
  int minutes = 0;
  int thousandths = 0;
  while(counter < 30){
    Serial.println();
    counter++;
	}
  Serial.print("--------------------------------------------------");
  Serial.println();
  Serial.print("|     Automated Chicken Coop Serial output       |");
  Serial.println();
  Serial.print("| Lightsensor"); Serial.print("\t"); Serial.print(lightSensor); 
  Serial.print("\t");
  Serial.write("Runtime"); Serial.print("\t"); Serial.print(String(mode));
  Serial.print("                |");
  Serial.println();
  Serial.print("| "); displayTime(); Serial.print("           |");
  Serial.println();
  Serial.print("| wildly off temp = "); Serial.print(tempF);
  if(morning == 1) {Serial.print(" Light should be on");}
  else {Serial.print(" Light Should be Off");}  
  Serial.print("       |"); Serial.println();
	Serial.print("| Voltage = "); Serial.print(getBatteryVoltage());
  Serial.print("\t"); Serial.print("RunTime = "); 
  if (millis() >= 1000) {
    seconds = millis() / 1000;
    thousandths = millis() - (seconds * 1000);
    Serial.print("Seconds "); Serial.print(seconds); Serial.print("."); Serial.print(thousandths);
  }
  else if(millis() >= 60000){
    minutes = millis() / 60000;
    seconds = (millis() / 1000) - minutes*60;
    thousandths = millis() - (seconds * 1000);
    Serial.print(minutes); Serial.print(":"); Serial.print(seconds); Serial.print("."); Serial.print(thousandths);
  }
  else {Serial.print(millis());}
  Serial.println(); Serial.print("Button "); Serial.print(lightOn);
  
}

//---------------------------Time till Daylight--------------------------------
byte setNight()
{
	readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
	if (hour < offTime && hour > onTime)
		return 1;
	else {
    if(millis() < 6000) {return 1;}
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
    Serial.print("Sunday");
    break;
  case 2:
    Serial.print("Monday");
    break;
  case 3:
    Serial.print("Tuesday");
    break;
  case 4:
    Serial.print("Wednesday");
    break;
  case 5:
    Serial.print("Thursday");
    break;
  case 6:
    Serial.print("Friday");
    break;
  case 7:
    Serial.print("Saturday");
    break;
  }
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
//-----------------Get Battery Voltage---------------------------------------//
float getBatteryVoltage()
{
	float vout, vin, R1, R2;
	vout = vin = 0; R1 = 30000.0; R2 = 7500.0;
	float value = analogRead(batteryVoltagePin);
	vout = (value * 5.0) / 1024.0; // see text
	vin = vout / (R2/(R1+R2));
	return vin;
}
//-----------------Arduino Chip temp read
long readTemp() //From Tinkerit https://code.google.com/archive/p/tinkerit/wikis/SecretThermometer.wiki
{ 
  long result; // Read temperature sensor against 1.1V reference 
  ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3); 
  delay(2); // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC); // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = (result - 125) * 1075; 
  return result; 
}

int get_temperature(int pin) 
{
	int cTemp;
	int fTemp;
	// We need to tell the function which pin the sensor is hooked up to. We're using
	// the variable pin for that above
	// Read the value on that pin
	int temperature = analogRead(pin);
	// Calculate the temperature based on the reading and send that value back
	float voltage = temperature * 5.0;
	voltage = voltage / 1024.0;
	cTemp = ((voltage - 0.5) * 100);
	fTemp = celsius_to_fahrenheit(cTemp);
	return cTemp;
}
int celsius_to_fahrenheit(int temp) 
{
  return (temp * 9 / 5) + 32;
}

/*This Segment doesn't work :-/  Turning off display background by pushing pin.
  if(timerC == 0) timersToTime();
   displayOn = 0;
 // if(displayOn = 1){digitalWrite(0, HIGH);}
  //else{digitalWrite(0,LOW);}
digitalWrite(0,LOW);
  if(digitalRead == 1 && (millis() - timerD) > 100){
    if(displayOn) 
    {
      displayOn = 0;
     }
    else {
      displayOn = 1;
      }
    timerD = millis();
  }
*/
