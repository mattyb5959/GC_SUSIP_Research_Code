 /*************************************************************************
Title:    Gaseous Compounds Firmware for USIPV
Authors:  Matthieu Guerrero ,<matyu.guerrero@gmail.com>; Chris Kelley, <cpkelley@cougarnet.uh.edu>
File:     $Id: $
License:  GNU General Public License v3

LICENSE:
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
  This sketch uses gaseous compund sensors connected to an Ard2499 based
  Iowa Scaled Engineering ARD-LTC2499 shield and a realtime clock module
  to transmit data uning an iMet-4RSB radiosonde to a ground station from
  a high altitude baloon.

  For the ARD-LTC2499 ADC shield:
  The jumpers for ADC address and EEPROM address should be left open, or the 
  defines for the ADC and EEPROM addresses should be changed in the setup() function.
  For more information about the ARD-LTC2499, see here:  
  http://www.iascaled.com/store/ARD-LTC2499
*/

#include <SD.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <Ard2499.h>

RTC_PCF8523 rtc; //For the rtc
Ard2499 ard2499board1;
File logfile; // the logging file


// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  5000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 10000 // mills between calls to flush() - to write data to the card

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

// the number of sensors being used
#define NUM_SENSORS 8

// the number of channels per sensor
#define CHAN_PER_SENSOR 3

// the number of channels being read
#define NUM_CHANNELS (NUM_SENSORS * CHAN_PER_SENSOR)

String crlf="\r\n"; //carriage return and linefeed to end packets
uint32_t syncTime = 0; // time of last sync()
int loopcount = 0; //loop counter
float room_temp = 25.0; //for the pt sensor

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); // for usb serial out
  Serial.println();
  Serial1.begin(9600); // for iMet data TX1 and RX1 (pins 18 and 19)
  
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

  //RTC setup
  Wire.begin(); 
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); //set RTC time

  //Failure status
  if (!rtc.initialized()) 
    {
    logfile.println("RTC failed"); 
    #if ECHO_TO_SERIAL
      Serial.println("RTC failed");
    #endif  //ECHO_TO_SERIAL 
  }

  //Arduino RTC mat
  ard2499board1.begin(ARD2499_ADC_ADDR_ZZZ, ARD2499_EEP_ADDR_ZZ);
  ard2499board1.ltc2499ChangeConfiguration(LTC2499_CONFIG2_60_50HZ_REJ);
}



void loop() {
  
  long adc[NUM_CHANNELS];
  String sensor[NUM_SENSORS];

  byte channel[] = {LTC2499_CHAN_SINGLE_0P, 
                    LTC2499_CHAN_DIFF_0P_1N,
                    LTC2499_CHAN_SINGLE_1P,
                    LTC2499_CHAN_SINGLE_2P, 
                    LTC2499_CHAN_DIFF_2P_3N,
                    LTC2499_CHAN_SINGLE_3P,
                    LTC2499_CHAN_SINGLE_4P, 
                    LTC2499_CHAN_DIFF_4P_5N,
                    LTC2499_CHAN_SINGLE_5P, 
                    LTC2499_CHAN_SINGLE_6P, 
                    LTC2499_CHAN_DIFF_6P_7N,
                    LTC2499_CHAN_SINGLE_7P,
                    LTC2499_CHAN_SINGLE_8P, 
                    LTC2499_CHAN_DIFF_8P_9N,
                    LTC2499_CHAN_SINGLE_9P,
                    LTC2499_CHAN_SINGLE_10P, 
                    LTC2499_CHAN_DIFF_10P_11N,
                    LTC2499_CHAN_SINGLE_11P,
                    LTC2499_CHAN_SINGLE_12P, 
                    LTC2499_CHAN_DIFF_12P_13N,
                    LTC2499_CHAN_SINGLE_13P, 
                    LTC2499_CHAN_SINGLE_14P, 
                    LTC2499_CHAN_DIFF_14P_15N,
                    LTC2499_CHAN_SINGLE_15P};

  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));

  digitalWrite(greenLEDpin, HIGH); //turn on green LED to idicate value read operation


  // fetch the time using arduino rtc (mat)
  DateTime now = rtc.now();
  long unixtime = now.unixtime();

  // get milliseconds since starting
  uint32_t m = millis();



  // sets ltc2499 channel to first channel then increments through
  // all channels and writes values to adc array
  ard2499board1.ltc2499ChangeChannel(channel[0]);
  for (int chanIndex=0; chanIndex<NUM_CHANNELS; chanIndex++){
    adc[chanIndex] = ard2499board1.ltc2499ReadAndChangeChannel(channel[(chanIndex+1) % NUM_CHANNELS]);
  }

  digitalWrite(greenLEDpin, LOW); //turn off green LED to show read complete

  // build xdata string packets for each sensor
  for (int sensIndex=0; sensIndex<NUM_SENSORS; sensIndex++){
    sensor[sensIndex] = "xdata=500"; // first section of packet
    sensor[sensIndex].concat(String(sensIndex, HEX)); // one ASCII hex char for sensor ID
    sensor[sensIndex].concat(longToHexString(adc[ (sensIndex * CHAN_PER_SENSOR) ] ) ); // 8 ascii hex char for sensor data 
    sensor[sensIndex].concat(longToHexString(adc[ (sensIndex * CHAN_PER_SENSOR) + 1 ] ) ); // 8 ascii hex char for sensor data
    sensor[sensIndex].concat(longToHexString(adc[ (sensIndex * CHAN_PER_SENSOR) + 2 ] ) ); // 8 ascii hex char for sensor data
    sensor[sensIndex].concat(longToHexString(unixtime)); //unixtimestamp 8 ascii hex char
    sensor[sensIndex].concat(intToHexString(loopcount)); //unixtimestamp 4 ascii hex char
    sensor[sensIndex].concat(crlf); // terminate xdata packet
  }

  
  //send xdata packets to iMet
  for (int sensIndex=0; sensIndex<(NUM_SENSORS); sensIndex++){
    Serial1.println(sensor[sensIndex]);
    //may need to add delay here to keep from flooding the iMet
  }


  //write data to log file
  for (int sensIndex=0; sensIndex<NUM_SENSORS; sensIndex++){
    logfile.print(String(sensIndex, HEX));
    logfile.print(", ");
    logfile.print(adc[ (sensIndex * CHAN_PER_SENSOR) ] ); 
    logfile.print(", ");
    logfile.print(adc[ (sensIndex * CHAN_PER_SENSOR) + 1 ] ) ;
    logfile.print(", ");
    logfile.print(adc[ (sensIndex * CHAN_PER_SENSOR) + 2 ] ) ;
    logfile.print(", ");
  }
  logfile.print(unixtime);
  logfile.print(", ");
  logfile.print(loopcount);
  logfile.print(", ");
  logfile.print(m); //milliseconds since start
  logfile.println(", ");

  // echo data to usb serial console 
  #if ECHO_TO_SERIAL
      for (int sensIndex=0; sensIndex<NUM_SENSORS; sensIndex++){
      Serial.print(String(sensIndex, HEX));
      Serial.print(", ");
      Serial.print(adc[ (sensIndex * CHAN_PER_SENSOR) ] ); 
      Serial.print(", ");
      Serial.print(adc[ (sensIndex * CHAN_PER_SENSOR) + 1 ] ) ;
      Serial.print(", ");
      Serial.print(adc[ (sensIndex * CHAN_PER_SENSOR) + 2 ] ) ;
      Serial.print(", ");
    }
    Serial.print(unixtime);
    Serial.print(", ");
    Serial.print(loopcount);
    Serial.print(", ");
    Serial.print(m); //milliseconds since start
    Serial.println(", ");
  #endif //ECHO_TO_SERIAL


  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) >= SYNC_INTERVAL){
    syncTime = millis();
    // blink LED to show we are syncing data to the card & updating FAT!
    digitalWrite(redLEDpin, HIGH);
    logfile.flush();
    digitalWrite(redLEDpin, LOW);
  }

  loopcount++;
}

//this function converts a long into an 8 character hex string
String longToHexString(long number) {
  char hexString[8]; // to store the hexadecimal string (long value has 8 bytes)
  // convert long value to hexadecimal string
  sprintf(hexString, "%08lx", number);  
  return String(hexString);
}

//this function converts a long into an 8 character hex string
String intToHexString(int number) {
  char hexString[8]; // to store the hexadecimal string
  // convert int value to hexadecimal string
  sprintf(hexString, "%04x", number);  
  return String(hexString);
}

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}

