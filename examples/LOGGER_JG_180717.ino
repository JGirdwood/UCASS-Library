/*******************************************************************************************
This program is intended for use with an Arduino UNO with an SD card shield to log UCASS and
MET data. The ICSP pins are used for communication with both the UCASS and the SD card. The 
chip select pin for the UCASS must be opperated at 3.3v so a resistor divider must be utilised.

Author: Joe Girdwood
Email: j.girdwood@herts.ac.uk
********************************************************************************************/

//Libraries
#include "MET.h"
#include "UCASS.h"

//Variables
const bool echotoserial = true;       //Set to true if a serial output is desired
const int res = 100;                  //Resolution of measurements
#define mon Serial                    //Serial data stream
const int baud = 9600;                //Serial baud rate

//Constructors
MET met;                //constructor for MET class
UCASS ucass;

//Pins
const char sdLEDpin = 2;               //LED used for errors
const char HIH4000pin = A3;            //Input from HIH4000
const char DCRESpin = A2;              //DC restoration pin
const char bridgeHIpin = A1;           //thermistor bridge pin
const char bridgeLOpin = A0;           //ditto
const char UCASScs = 9;                //UCASS chip select
const char SDcs = 10;                  //SD card chip select

void setup() {
  
  //Setting up Pins
  pinMode(sdLEDpin,OUTPUT);
  pinMode(DCRESpin,INPUT);
  pinMode(bridgeHIpin,INPUT);
  pinMode(bridgeLOpin,INPUT);
  pinMode(HIH4000pin,INPUT);
  pinMode(SDcs,OUTPUT);
  pinMode(UCASScs,OUTPUT);
  digitalWrite(UCASScs,HIGH);
  
  mon.begin(baud);
  while(!mon){}

  bool SDOK = met.startSD(SDcs, sdLEDpin, mon);     //Setting up SD card
  bool logOK = met.createlog(mon);                  //Creating Log File
  bool RTCOK = met.startRTC(mon);                   //Setting up RTC
  bool SPIOK = ucass.startSPI(mon);
  bool err = SDOK*logOK*RTCOK*SPIOK;                //Determining error
  met.error(err, mon, sdLEDpin);                    //Acting on error
  met.setupMonitor(mon);                            //Setting up files

  ucass.readInfoString(UCASScs,mon);
  ucass.printINFOstr(mon);
  met.openLog(mon);
  ucass.printINFOstr(met.Log);
  met.closeLog(SDcs);

  ucass.readConfigVars(UCASScs,mon);
  mon.println();
  mon.println(F("bb0\tbb1\tbb2\tbb3\tbb4\tbb5\tbb6\tbb7\tbb8\tbb9\tbb10\tbb11\tbb12\tbb13\tbb14\tGSC\tID"));
  ucass.printConfig(mon, '\t');
  met.openLog(mon);
  met.Log.println(F("bb0,bb1,bb2,bb3,bb4,bb5,bb6,bb7,bb8,bb9,bb10,bb11,bb12,bb13,bb14,GSC,ID"));
  ucass.printConfig(met.Log, ',');
  met.closeLog(SDcs);
  
  mon.println();
  mon.println(F("time\ttemp\thum\tb0\tb1\tb2\tb3\tb4\tb5\tb6\tb7\tb8\tb9\tb10\tb11\tb12\tb13\tb14\tb1tof\tb3tof\tb5tof\tb7tof\tperiod\tcsum\tglitch\tlongtof\trejrat"));
  met.openLog(mon);
  met.Log.println(F("time,temp,hum,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b1tof,b3tof,b5tof,b7tof,period,csum,glitch,longtof,rejrat"));
  met.closeLog(SDcs);
}

void loop() {
  
  //Defining Temporal Resolution
  delay(res);
  unsigned long t = millis();
  
  //Reading
  ucass.readHistData(UCASScs, mon);
  met.METread(HIH4000pin, bridgeHIpin, bridgeLOpin);
  
  //Printing
  mon.print(t);
  mon.print('\t');
  met.METprint(mon, '\t');
  mon.print('\t');
  ucass.printHist(mon,'\t');
  mon.println();
  
  met.openLog(mon);
  met.Log.print(t);
  met.Log.print(',');
  met.METprint(met.Log, ',');
  met.Log.print(',');
  ucass.printHist(met.Log, ',');
  met.Log.println();
  met.closeLog(SDcs);
}













