#include<Wire.h>
#include<SPI.h>
#include<SD.h>
const byte DS1307 = 0x68;
const byte NumberOfFields = 7;
int second, minute, hour, day, wday, month, year;
char* p;
File myFile;
String datetime="";
int save = 0;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card…");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  SD.remove("datetime.txt");
}

void loop()
{
  String time;
  if (Serial.available() > 0) {
    time = Serial.readString();
    char time1[50];
    for (int i = 0; i <= time.length(); i++) {
      time1[i] = time[i];
    }
    p = strtok(time1, ":");
    int time2[7] ;
    int idx = 1;
    time2[0] = ctos(p);
    while (p != NULL) {
      p = strtok(NULL, ":");

      if (p != NULL) {
        time2[idx++] = ctos(p);
      }
    }
    idx = 0;
    if ((time2[0] >= 0 && time2[0] <= 23) && (time2[1] >= 0 && time2[2] <= 59) && (time2[2] >= 0 && time2[2] <= 60) && (time2[3] >= 1 && time2[3] <= 7) && (time2[4] >= 1 && time2[4] <= 31) && (time2[5] >= 1 && time2[5] <= 12) && (time2[6] >= 0 && time2[6] <= 99)) {
      setTime(time2[0], time2[1], time2[2], time2[3], time2[4], time2[5], time2[6]);
      Serial.println("Set time succeed!");
    }
    else {
      Serial.println("Set time failed.");
    }
    
    time = "";
  }
  readDS1307();
  digitalClockDisplay();
  delay(1000);
  save++;
  if (save == 5) {
    myFile = SD.open("datetime.txt", FILE_WRITE);
    if (myFile) {
      myFile.println(datetime);
      myFile.close();
      Serial.println("done.");
    }
    else {
      Serial.println("error opening datetime.txt");
    }
    save = 0;
  }
  
}

int ctos(char *p) {
  String tmp=p;
  return tmp.toInt();
  
}
int bcd2dec(byte num)
{
  return ((num / 16 * 10) + (num % 16));
}
int dec2bcd(byte num)
{
  return ((num / 10 * 16) + (num % 10));
}
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x00));
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wd));
  Wire.write(dec2bcd(d));
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}
void digitalClockDisplay() {
  datetime = "";
  Serial.print(hour);
  datetime += (String)hour;
  printDigits(minute);
  printDigits(second);
  if (wday == 1) {
    Serial.print(" CN");
    datetime += " CN";
  }
  else {
    Serial.print(" Thu:");
    Serial.print(wday);
    datetime += " Thu:" + (String)wday;
  }
  Serial.print(" ");
  Serial.print(day);
  datetime += " " + (String)day;
  Serial.print("/");
  Serial.print(month);
  datetime += "/" + (String)month;
  Serial.print("/");
  Serial.print(year);
  datetime += "/" + (String)year;
  Serial.println();
}
void readDS1307()
{
  Wire.beginTransmission(DS1307);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307, NumberOfFields);
  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read());
  hour = bcd2dec(Wire.read() & 0x3f);
  wday = bcd2dec(Wire.read());
  day = bcd2dec(Wire.read());
  month = bcd2dec(Wire.read());
  year = bcd2dec(Wire.read());
  year += 2000;
}
void printDigits(int digits) {
  Serial.print(":");
  datetime += ":";
  if (digits < 10) {
    Serial.print('0');
    datetime += "0";
    Serial.print(digits);
    datetime += (String)digits;
  }
  else {
    datetime += (String)digits;
    Serial.print(digits);
  }
}
