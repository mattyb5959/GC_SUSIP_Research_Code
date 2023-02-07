                                                                                   /*************************************************************************
Title:    ARD-LTC2499 Library Board Test
Authors:  Michael Petersen <railfan@drgw.net>, Matthieu Guerrero
File:     $Id: $
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2014 Nathan D. Holmes & Michael D. Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

/*
  This sketch will demonstrate how to use the Ard2499 library for the 
  Iowa Scaled Engineering ARD-LTC2499 shield.   For more information about the
  ARD-LTC2499, see here:  http://www.iascaled.com/store/ARD-LTC2499
  
  The jumpers for ADC address and EEPROM address should be left open, or the 
  defines for the ADC and EEPROM addresses should be changed in the setup() function.
*/
#include <SD.h>
#include "RTClib.h"
#include "SPI.h"
#include <Wire.h>
#include "Ard2499.h"
#include "SoftwareSerial.h"
#define RXPIN 8
#define TXPIN 9
SoftwareSerial ss(RXPIN, TXPIN);
String s1="xdata=";
String s12="xdata=";
String s13="xdata=";
String s14="xdata=";
String s2="\r\n";

Ard2499 ard2499board1;
// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

 RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}
byte confChan=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
    Serial.println();
  
  // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  
#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }
  

 /*logfile.println("millis,stamp,datetime,adc0,adc01,adc1...");    
#if ECHO_TO_SERIAL
  Serial.println("millis,stamp,datetime,adc0,adc01,adc1...");
#endif //ECHO_TO_SERIAL*/
 

  Wire.begin();
  ard2499board1.begin(ARD2499_ADC_ADDR_ZZZ, ARD2499_EEP_ADDR_ZZ);
  ard2499board1.ltc2499ChangeConfiguration(LTC2499_CONFIG2_60_50HZ_REJ);
  ard2499board1.ltc2499ChangeChannel(LTC2499_CHAN_SINGLE_0P);
byte retval = 0;
/*Serial.print("eeprom mac = [");
  Serial.print(ard2499board1.eui48Get());
  Serial.print("]\n");
  
  Serial.print("write eeprom[0] 0x");
  Serial.print(0xAA, HEX);
  retval = ard2499board1.eepromWrite(0, 0xAA, true);
  Serial.print(" retval=");
  Serial.print(retval);
  Serial.print("\n");

  Serial.print("read eeprom[0] 0x");
  Serial.print(ard2499board1.eepromRead(0, true), HEX);
  Serial.print("\n");
``
  Serial.print("write eeprom[0] 0x");
  Serial.print(0x55, HEX);
  retval = ard2499board1.eepromWrite(0, 0x55, true);
  Serial.print(" retval=");
  Serial.print(retval);
  Serial.print("\n");

  Serial.print("read eeprom[0] 0x");
  Serial.print(ard2499board1.eepromRead(0, true), HEX);
  Serial.print("\n");*/

  
}
byte i=0;
byte retval = 0;
  
void loop() {
  // print the results to the serial monitor:
DateTime now;
 i=0;
  byte newchannel = 0;
    long adc = 0;
    long adc0 = 0;
    long adc01 = 0;
    long adc1 = 0;
    long adc2 = 0;
    long adc23 = 0;
    long adc3 = 0;
    long adc4 = 0;
    long adc45 = 0;
    long adc5 = 0;
    long adc6 = 0;
    long adc67 = 0;
    long adc7 = 0;
    long adc8 = 0;
    long adc89    = 0;
    long adc9 = 0;
    long adc10 = 0;
    long adc1011 = 0;
    long adc11 = 0;
    long adc12 = 0;
    long adc1213 = 0;
    long adc13 = 0;
    long adc14 = 0;
    long adc1415 = 0;
    long adc15 = 0;
    byte retval = 0;
  
  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  
  digitalWrite(greenLEDpin, HIGH);
  
  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);           // milliseconds since start
  logfile.print(", ");    
#if ECHO_TO_SERIAL
  Serial.print(m);         // milliseconds since start
  Serial.print(", ");  
#endif //ECHO_TO_SERIAL

// fetch the time
  now = RTC.now();
  // log time
  logfile.print(now.unixtime()); // seconds since 1/1/1970
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
#if ECHO_TO_SERIAL
  Serial.print(now.unixtime()); // seconds since 1/1/1970
  Serial.print(", ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
#endif //ECHO_TO_SERIAL 





 adc0 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_0P_1N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc01 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_1P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc1 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_2P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc2 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_2P_3N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc23 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_3P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc3 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_4P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc4 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_4P_5N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc45 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_5P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc5 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_6P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc6 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_6P_7N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc67 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_7P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc7 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_8P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc8 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_8P_9N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc89 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_9P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc9 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_10P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc10 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_10P_11N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc1011 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_11P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc11 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_12P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc12 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_12P_13N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc1213 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_13P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc13 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_14P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc14 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_DIFF_14P_15N;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc1415= ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_15P;
 ard2499board1.ltc2499ChangeChannel(newchannel);
 adc15 = ard2499board1.ltc2499Read();
 newchannel = LTC2499_CHAN_SINGLE_0P;
 /*adc0 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_DIFF_0P_1N);   
     adc01 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_1P);
     adc1 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_2p) ;   
     adc2 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_DIFF_2P_3N);   
     adc23 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_3P);
     adc3 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_4p) ;   
     adc4 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_DIFF_4P_5N);   
     adc45 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_5P);
     adc5 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_6p) ;   
     adc6 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_DIFF_6P_7N);   
     adc67 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_7P);
     adc7 = ard2499board1.ltc2499ReadandChangeChannel(LTC2499_CHAN_SINGLE_8p) ;   */

/*double Vadc01 = ((adc01*2.048) / 16777216.0);
double Vadc23 = ((adc23*2.048) / 16777216.0);
double Vadc45 = ((adc45*2.048) / 16777216.0);
double Vadc67 = ((adc67*2.048) / 16777216.0);
double Vadc89 = ((adc89*2.048) / 16777216.0);
double Vadc1011 = ((adc1011*2.048) / 16777216.0);
double Vadc1213 = ((adc1213*2.048) / 16777216.0);
double Vadc1415 = ((adc1415*2.048) / 16777216.0);*/
         
         
   logfile.print(", ");    
  logfile.print("C");
        logfile.print(i);
        logfile.print(", ");    
  /*logfile.print(adc0);
  //logfile.print(", ");  
  logfile.print(adc01);
  logfile.print(",");
  //logfile.print(adc1);
  //logfile.print(",");
  //logfile.print(adc2);
  //logfile.print(",");
  logfile.print(adc23);
  logfile.print(",");
  logfile.print(adc3);
  logfile.print(",");
  logfile.print(adc4);
  logfile.print(",");*/
  //logfile.print(adc45);
  //logfile.print(",");
  /*logfile.print(adc5);
  logfile.print(",");
  logfile.print(adc6);
  logfile.print(",");*/
  //logfile.print(adc67);
  //logfile.print(",");
  /*logfile.print(adc7);
  logfile.print(",");
  logfile.print(adc8);
  logfile.print(",");
  logfile.print(adc89);
  logfile.print(",");
  logfile.print(adc9);
  logfile.print(",");
  logfile.print(adc10);
  logfile.print(",");
  logfile.print(adc1011);
  logfile.print(",");
  logfile.print(adc11);
  logfile.print(",");
  logfile.print(adc12);
  logfile.print(",");
  logfile.print(adc1213);
  logfile.print(",");
  logfile.print(adc13);
  logfile.print(",");
  logfile.print(adc14);
  logfile.print(",");
  logfile.print(adc1415);
  logfile.print(",");
  logfile.print(adc15);*/
  logfile.print("PAY ATTENTION TO THESE. THEY ARE THE DIFFERENTIALS."); //If you want it to write more channels, this is where you want to uncomment stuff.
  
  logfile.print(adc01);
  s1.concat(adc0);
  s1.concat(s2);
  ss.println(s1);
  
  logfile.print(",");
  
  logfile.print(adc23);
  s12.concat(adc2);
  s12.concat(s2);
  ss.println(s12);
  
  logfile.print(",");
 
  logfile.print(adc45);
  s13.concat(adc4);
  s13.concat(s2);
  ss.println(s13);
  
  logfile.print(",");
  
  logfile.print(adc67);
  s14.concat(adc6);
  s14.concat(s2);
  ss.println(s14);
  
  logfile.print(",");
  /*logfile.print(Vadc89);
  logfile.print(",");
  logfile.print(Vadc1011);
  logfile.print(",");
  logfile.print(Vadc1213);
  logfile.print(",");
  logfile.print(Vadc1415);
  logfile.print(",");*/
  logfile.print("End");
logfile.print(", ");    
logfile.println("/n");
 

   #if ECHO_TO_SERIAL
        Serial.print("Channel ");
        Serial.print(i);
        Serial.print("... ");
        Serial.print(" ");
        //Serial.println(adc0);
        //Serial.println(adc,HEX);
        //Serial.println(" ");
        //Serial.println(adc01);
        //Serial.print(" "); 
        //Serial.println(adc1);
        //Serial.print(" "); 
        //Serial.println(adc2);
        //Serial.print(" "); 
        //Serial.println(adc23);
        //Serial.print(" "); 
        //Serial.println(adc3);
        //Serial.print(" "); 
        //Serial.println(adc4);
        //Serial.print(" "); 
        //Serial.println(adc45);
       //Serial.print(" "); 
        //Serial.println(adc5);
        //Serial.print(" "); 
        //Serial.println(adc6);
        //Serial.print(" "); 
        //Serial.println(adc67);
        //Serial.print(" "); 
        //Serial.println(adc7);
        //Serial.print(" "); 
        //Serial.println(adc8);
        //Serial.print(" "); 
        //Serial.println(adc89);
        //Serial.print(" "); 
        //Serial.println(adc9);
        //Serial.print(" "); 
        //Serial.println(adc10);
        //Serial.print(" "); 
        //Serial.println(adc1011);
        //Serial.print(" "); 
        //Serial.println(adc11);
        //Serial.print(" "); 
        //Serial.println(adc12);
        //Serial.print(" "); 
        //Serial.println(adc1213);
        //Serial.print(" "); 
        //Serial.println(adc13);
        //Serial.print(" "); 
        //Serial.println(adc14);
        //Serial.print(" "); 
        //Serial.println(adc1415);
        //Serial.print(" "); 
        //Serial.println(adc15);
        //Serial.print(" ");
        Serial.println("Begin differentials.");  //this outputs to the serial monitor. Turn this off for flight.
        Serial.print(" ");
        Serial.println(adc01);
        Serial.print(" ");
        Serial.println(adc23);
        Serial.print(" ");
        Serial.println(adc45);
        Serial.print(" ");
        Serial.println(adc67);
        Serial.print(" ");
        /*Serial.println(Vadc89);
        Serial.print(" ");
        Serial.println(Vadc1011);
        Serial.print(" ");
        Serial.println(Vadc1213);
        Serial.print(" ");
        Serial.println(Vadc1415);
        Serial.print(" ");*/
        Serial.println("End line of differentials.");
        Serial.print(" ");
        Serial.println("New Series");
        Serial.print(" ");
#endif //ECHO_TO_SERIAL
       // delay(1000);
        digitalWrite(greenLEDpin, LOW);

  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  
  // blink LED to show we are syncing data to the card & updating FAT!
  digitalWrite(redLEDpin, HIGH);
  logfile.flush();
  digitalWrite(redLEDpin, LOW);
        

   
}

void findI2CSlave(uint8_t addr)
{
  byte stat, done = 0;
  while(!done)
  {
    Wire.beginTransmission(addr);
    stat = Wire.endTransmission();
    if(!stat)
    {   
      Serial.println("Done!");
      done = 1;
    }
  }
  delay(1000);
}

byte enumerateI2C(boolean showErrors)
{
  byte addr, stat, last_ack = 0x00;
 
  /*Serial.print("Enumerating all I2C devices\n (Press any key to stop scanning I2C bus)\n-------------\n");
 
  for(addr=0x00; addr<0x7F; addr++)
  {
    Wire.beginTransmission(addr);
    stat = Wire.endTransmission();
    if(stat)
    {
      if(showErrors)
      {
        Serial.print("0x");
        Serial.print(addr, HEX);
        Serial.print(": ");
        Serial.print(stat);
        Serial.print("\n");
      }
    }  
    else
    {   
      Serial.print("0x");
      Serial.print(addr, HEX);
      Serial.print(" ");
      last_ack = addr;   
    }
  }  
  Serial.print("\n");
  return last_ack;*/
}
 
