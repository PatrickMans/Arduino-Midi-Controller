/*
First trial to get a midi controller
Needs:
- LCD display
- Midi interface (build)
 */

 
// include the library
#include <LiquidCrystal.h>
#include <Wire.h> 

// initialize the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int PATCH = 0;
int OLDPATCH;
int SYSEX1 = 0xF0;
int SYSEX2 = 0x03;
int SWITCH = 0x81; 
int Mo = 0;
int St = 0;
int Dl = 0;
int Rv = 0;
int Ca = 0;
int Cb = 0;
int Cc = 0;
int Cd = 0;
 
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
 
int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 
 
    if (adc_key_in > 1000) return btnNONE; 
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;   
      
    return btnNONE;                // when all others fail, return this.
}
// the 8 arrays that form each segment of the custom numbers
byte LT[8] = 
{
 B00111,
 B01111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111
};
byte UB[8] =
{
 B11111,
 B11111,
 B11111,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};
byte RT[8] =
{
 B11100,
 B11110,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111
};
byte LL[8] =
{
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B01111,
 B00111
};
byte LB[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111,
 B11111
};
byte LR[8] =
{
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11111,
 B11110,
 B11100
};
byte UMB[8] =
{
 B11111,
 B11111,
 B11111,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111
};
byte LMB[8] =
{
 B11111,
 B00000,
 B00000,
 B00000,
 B00000,
 B11111,
 B11111,
 B11111
};
byte CLR[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};
// loop counter
int x = 0;

void Clear()
{
   lcd.setCursor(x,0);
   lcd.write("   ");
   lcd.setCursor(x,1);
   lcd.write("   ");
}

void custom0()
{ // uses segments to build the number 0
 Clear();
 lcd.setCursor(x, 0); // set cursor to column 0, line 0 (first row)
 lcd.write(byte(0));  // call each segment to create
 lcd.write(1);  // top half of the number
 lcd.write(2);
 lcd.setCursor(x, 1); // set cursor to colum 0, line 1 (second row)
 lcd.write(3);  // call each segment to create
 lcd.write(4);  // bottom half of the number
 lcd.write(5);
}

void custom1()
{
 Clear();
 lcd.setCursor(x,0);
 lcd.write(1);
 lcd.write(2);
 lcd.setCursor(x+1,1);
 lcd.write(5);
}

void custom2()
{
 Clear();
 lcd.setCursor(x,0);
 lcd.write(6);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(x, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(7);
}

void custom3()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(6);
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(x, 1);
 lcd.write(7);
 lcd.write(7);
 lcd.write(5); 
}

void custom4()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(3);
 lcd.write(4);
 lcd.write(2);
 lcd.setCursor(x+2, 1);
 lcd.write(5);
}

void custom5()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(byte(0));
 lcd.write(6);
 lcd.write(6);
 lcd.setCursor(x, 1);
 lcd.write(7);
 lcd.write(7);
 lcd.write(5);
}

void custom6()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(byte(0));
 lcd.write(6);
 lcd.write(6);
 lcd.setCursor(x, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(5);
}

void custom7()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(1);
 lcd.write(1);
 lcd.write(2);
 lcd.setCursor(x+2, 1);
 lcd.write(2);
}

void custom8()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(byte(0));
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(x, 1);
 lcd.write(3);
 lcd.write(7);
 lcd.write(5);
}

void custom9()
{
  Clear();
 lcd.setCursor(x,0);
 lcd.write(byte(0));
 lcd.write(6);
 lcd.write(2);
 lcd.setCursor(x+2, 1);
 lcd.write(5);
}

void WritePatchNR(int PATCH) {
int High;
int Low;
x = 0;
if (PATCH < 10)
{
  High = 0;
}
else
{ 
  High = int(PATCH/10);
};
switch(High){
  case 0:
     custom0();
  break;
  case 1:
     custom1();
  break;
  case 2:
     custom2();
  break;
  case 3:
     custom3();
  break;
  case 4:
     custom4();
  break;
  case 5:
     custom5();
  break;
  case 6:
     custom6();
  break;
  case 7:
     custom7();
  break;
  case 8:
     custom8();
  break;
  case 9:
     custom9();
  break;
  }
x = x + 3;

Low = int(PATCH - (High * 10));
switch(Low){
  case 0:
     custom0();
  break;
  case 1:
     custom1();
  break;
  case 2:
     custom2();
  break;
  case 3:
     custom3();
  break;
  case 4:
     custom4();
  break;
  case 5:
     custom5();
  break;
  case 6:
     custom6();
  break;
  case 7:
     custom7();
  break;
  case 8:
     custom8();
  break;
  case 9:
     custom9();
  break;
  }  
  delay(300);
}

void WriteTopLine(String TEXT) {
lcd.setCursor(7,0);
lcd.print("             ");
lcd.print(TEXT);  
}

void WriteBotLine(String TEXT) {
lcd.print("             ");
lcd.setCursor(7,1);
lcd.print(TEXT);  
}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
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

 String TimeSTR;
 String TimeSTR1;
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  TimeSTR = hour + ":" + minute;
  TimeSTR = TimeSTR1 + ":" + second;
  lcd.setCursor(7,0);
  lcd.print(hour, DEC);
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");
  }
  lcd.print(second, DEC);
//  WriteTopLine(TimeSTR);
}

//  Sends the switch value to POD
void SendSwitch(int KEY, int STATUS) {
  Serial.write(SYSEX1);
  Serial.write(SYSEX2);
  Serial.write(SWITCH);
  Serial.write(KEY);
  Serial.write(STATUS);
}

void DecodeSwitchOn(String SWNAME) {
int SWCODE;

      if (SWNAME = "BankDown")
      {
            SWCODE = 0x00;
       };
      if (SWNAME = "AMP1")
       {
            SWCODE = 0x01;
       };
       if (SWNAME = "FXLOOP")
       {
            SWCODE = 0x02;
       };
       if (SWNAME = "BANKUP")
       {
            SWCODE = 0x10;
       };
       if (SWNAME = "AMP2")
       {
            SWCODE = 0x11;
       };
       if (SWNAME = "STOMP1") 
       {
            SWCODE = 0x12;
       };
       if (SWNAME = "CHANA")
       {
            SWCODE = 0x20;
       };
       if (SWNAME = "REVERB"){
            SWCODE = 0x21;
       };
       if (SWNAME = "STOMP2")
       {
            SWCODE = 0x22;
       };
       if (SWNAME = "CHANB")
       {
            SWCODE = 0x30;
       };
       if (SWNAME = "PITCH")
       {
            SWCODE = 0x31;
       };
       if (SWNAME = "STOMP3")
       {
            SWCODE = 0x32;
       };
       if (SWNAME = "CHANC")
       {
            SWCODE = 0x40;
       };
       if (SWNAME = "MOD")
       {
            SWCODE = 0x41;
       };
       if (SWNAME = "CHAND")
       {
            SWCODE = 0x50;
       };
       if (SWNAME = "DLY"){
            SWCODE = 0x51;
       };
       if (SWNAME = "FAVCHAN")
       {
            SWCODE = 0x60;
       };
       if (SWNAME = "TAP")
       {
            SWCODE = 0x61;
       };
       if (SWNAME = "FCTRLW")
       {
            SWCODE = 0x43;
       };
       if (SWNAME = "FCTRLV")
       {
            SWCODE = 0x50;
       };
   SendSwitch(SWCODE, 0x01);
   delay(140);
   SendSwitch(SWCODE, 0x00);      
}
void DecodeSwitchOff(String SWNAME) {
int SWCODE;

      if (SWNAME = "BankDown")
      {
            SWCODE = 0x00;
       };
      if (SWNAME = "AMP1")
       {
            SWCODE = 0x01;
       };
       if (SWNAME = "FXLOOP")
       {
            SWCODE = 0x02;
       };
       if (SWNAME = "BANKUP")
       {
            SWCODE = 0x10;
       };
       if (SWNAME = "AMP2")
       {
            SWCODE = 0x11;
       };
       if (SWNAME = "STOMP1") 
       {
            SWCODE = 0x12;
       };
       if (SWNAME = "CHANA")
       {
            SWCODE = 0x20;
       };
       if (SWNAME = "REVERB"){
            SWCODE = 0x21;
       };
       if (SWNAME = "STOMP2")
       {
            SWCODE = 0x22;
       };
       if (SWNAME = "CHANB")
       {
            SWCODE = 0x30;
       };
       if (SWNAME = "PITCH")
       {
            SWCODE = 0x31;
       };
       if (SWNAME = "STOMP3")
       {
            SWCODE = 0x32;
       };
       if (SWNAME = "CHANC")
       {
            SWCODE = 0x40;
       };
       if (SWNAME = "MOD")
       {
            SWCODE = 0x41;
       };
       if (SWNAME = "CHAND")
       {
            SWCODE = 0x50;
       };
       if (SWNAME = "DLY"){
            SWCODE = 0x51;
       };
       if (SWNAME = "FAVCHAN")
       {
            SWCODE = 0x60;
       };
       if (SWNAME = "TAP")
       {
            SWCODE = 0x61;
       };
       if (SWNAME = "FCTRLW")
       {
            SWCODE = 0x43;
       };
       if (SWNAME = "FCTRLV")
       {
            SWCODE = 0x50;
       };
   SendSwitch(SWCODE, 0x00); 
}

void SwitchSt(){
  if (St == 0) {
    St = 1;
     lcd.setCursor(7,0);
     lcd.write(4);
     WriteBotLine("STP ON ");
     DecodeSwitchOn("STOMP");
  }
  else
  {
     St = 0;
     lcd.setCursor(7,0);
     lcd.write(1); 
     WriteBotLine("STP OFF");
     DecodeSwitchOff("STOMP");

  }
  delay(300);
}

void SwitchMo(){
  if (Mo == 0) {
     Mo = 1;
     lcd.setCursor(9,0);
     lcd.write(4);
     WriteBotLine("MOD ON ");
     DecodeSwitchOn("MOD");
  }
  else
  {
     Mo = 0;
     lcd.setCursor(9,0);
     lcd.write(1);
     WriteBotLine("MOD OFF");
     DecodeSwitchOff("MOD");
  }
  delay(300);
}

void SwitchDl(){
  if (Dl == 0) {
     Dl = 1;
     lcd.setCursor(11,0);
     lcd.write(4);
     WriteBotLine("DLY ON ");
     DecodeSwitchOn("DLY");
  }
  else
  {
     Dl = 0;
     lcd.setCursor(11,0);
     lcd.write(1);
     WriteBotLine("DLY OFF");
     DecodeSwitchOff("DLY");

  }
  delay(300);
}

void SwitchRv(){
  if (Rv == 0) {
     Rv = 1;
     lcd.setCursor(13,0);
     lcd.write(4);
     WriteBotLine("RVB ON ");
     DecodeSwitchOn("REV");
  }
  else
  {
     Rv = 0;
     lcd.setCursor(13,0);
     lcd.write(1);
     WriteBotLine("RVB OFF");
     DecodeSwitchOff("REV");

  };
  delay(300);
}

void setup() {
 // assignes each segment a write number
 lcd.createChar(0,LT);
 lcd.createChar(1,UB);
 lcd.createChar(2,RT);
 lcd.createChar(3,LL);
 lcd.createChar(4,LB);
 lcd.createChar(5,LR);
 lcd.createChar(6,UMB);
 lcd.createChar(7,LMB);
  
 // sets the LCD's rows and colums:
 lcd.begin(16, 2);
 Wire.begin();
 PATCH = 0;
 OLDPATCH = 3;

 //  Set MIDI baud rate:
Serial.begin(31250);
}

void loop() {
  
if (OLDPATCH != PATCH)
{
   WritePatchNR(PATCH);
   OLDPATCH = PATCH;
}
   lcd_key = read_LCD_buttons();
   
   if (lcd_key < btnNONE) {
   switch (lcd_key){
       case btnDOWN:{
             SwitchDl();
            break;
       }
       case btnUP:{
             SwitchMo();
             break;
       }    
       case btnLEFT:{
             SwitchSt();
             break;
       }
       case btnRIGHT:{
             SwitchRv();
             break;
       }
       case btnSELECT:{
             break;
       }
       case btnNONE:{
             break;
       }
     }
   }
}
