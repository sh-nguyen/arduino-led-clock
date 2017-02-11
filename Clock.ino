#include <Time.h>
#include <TimeLib.h>

#include "Clock.h"
#include "Binary.h"


#define NBR_MTX 5 
LedControl lc=LedControl(12,11,10, NBR_MTX);
const int buttonPin = 2;  
const int faces = 2;

char ch0, ch1, ch2, ch3, ch4;
int t = 0;
String hrs, mins, days, mths, yrs;
int buttonState = 0;
byte a[5];
byte b[5];
byte c[5];
byte d[5];
byte e[5];
byte f[5];
byte g[5];
byte h[5];
byte final[40];

long referenceTimeColon;
boolean colonState = true;
long referenceTimeFace;
int faceState = 0; 

String format(String in) {
  int out = in.toInt();
  if (out < 10) {
    in = "0" + in;
  }
  return in;
}

void clockFace() {
  while (faceState==0) {
    
    hrs = String(hourFormat12());
    hrs = format(hrs);
    mins = String(minute());
    mins = format(mins);
    char half = 'P';
    
    if (isAM() == true) {
      half = 'A';
    }

    if (millis()-referenceTimeColon > 500) {
      colonState = !colonState;
      referenceTimeColon = millis();
    }

    if (colonState == true) {
      importBitmap(':',c);
    } else {
      importBitmap(' ',c);
    }
    
    importBitmap(char(hrs[0]),a);
    importBitmap(char(hrs[1]),b);
    importBitmap(char(mins[0]),d);
    importBitmap(char(mins[1]),e);
    importBitmap(char(half),f);
    importBitmap('m',g);

    
    clockConcatArray(a,b,c,d,e,f,g,final);
    lc.displayBitmap(0,final,sizeof(final));
    
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH){
      buttonState = LOW;
      faceState++;
      referenceTimeFace = millis();
      delay(500);
      lc.clearAll();
      clearArray(final);
      break;
    }

    if (millis()-referenceTimeFace > 25000){
        faceState++;
        referenceTimeFace = millis();
        delay(500);
        lc.clearAll();
        clearArray(final);
        break;
      }
  }
}

void calenderFace() {
    while (faceState == 1) {
      days = String(day());
      days = format(days);
      mths = String(month());
      mths = format(mths);
      yrs = String(year()-2000);

      importBitmap(char(days[0]),a);
      importBitmap(char(days[1]),b);
      importBitmap(char('/'),c);
      importBitmap(char(mths[0]),d);
      importBitmap(char(mths[1]),e);
      importBitmap(char('/'),f);
      importBitmap(char(yrs[0]),g);
      importBitmap(char(yrs[1]),h);

      
      calendarConcatArray(a,b,c,d,e,f,g,h,final);
      lc.displayBitmap(0,final,sizeof(final));

      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH){
        buttonState = LOW;
        faceState++;
        referenceTimeFace = millis();
        delay(500);
        lc.clearAll();
        clearArray(final);
        break;
      }

      if (millis()-referenceTimeFace > 5000){
        faceState++;
        referenceTimeFace = millis();
        delay(500);
        lc.clearAll();
        clearArray(final);
        break;
      }
    }
}

void clockConcatArray(byte a[], byte b[], byte c[], byte d[], byte e[], byte f[], byte g[], byte final[]){
  
  for (int i = 0; i < 5; ++i) {
    final[i] = a[i];
  }
  for (int i = 0; i < 5; i++) {
    final[i+5] = b[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+9] = c[i];
  }
  for (int i = 0; i < 5; i++) {
    final[i+13] = d[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+18] = e[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+23] = f[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+28] = g[i];
  }
}

void calendarConcatArray(byte a[], byte b[], byte c[], byte d[], byte e[], byte f[], byte g[], byte h[], byte final[]){
  
  for (int i = 0; i < 5; ++i) {
    final[i] = a[i];
  }
  for (int i = 0; i < 5; i++) {
    final[i+5] = b[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+9] = c[i];
  }
  for (int i = 0; i < 5; i++) {
    final[i+13] = d[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+18] = e[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+22] = f[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+26] = g[i];
  }
  for (int i = 0; i < 5; ++i) {
    final[i+31] = h[i];
  }
}

void importBitmap(char character, byte target[]){
  for (int l=0; l < 5; l++) {
    target[l] = alphabetBitmap[lc.getCharArrayPosition(character)][l];
  }
}

void clearArray(byte target[]) {
  for (int l=0; l < 80; l++) {
    target[l] = 0x00;
  }
}

void setup() {
  Serial.begin (9600);
  Serial.println("Sync");
  pinMode(buttonPin, INPUT);

  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,1);
    lc.clearDisplay(i);
  }
  delay(100);
  lc.clearAll();
  while (Serial.available() == 0){
    lc.displayChar(0,lc.getCharArrayPosition('S'));  
    lc.displayChar(1,lc.getCharArrayPosition('Y'));  
    lc.displayChar(2,lc.getCharArrayPosition('N'));  
    lc.displayChar(3,lc.getCharArrayPosition('C'));  
  }
  if (Serial.available() > 0) {
                setTime(Serial.parseInt());
                Serial.print("I received: ");
                Serial.print(hour());
                Serial.print(":");
                Serial.println(minute());
  }

  referenceTimeColon = millis();
  referenceTimeFace = millis();  
}

void loop() {
  if (faceState == faces) {
    faceState = 0;
  }
  
  clockFace();
  calenderFace();
}





