#ifndef _ucass
#define _ucass
#endif

#include <SPI.h>
#include <Arduino.h>

class UCASS
{
public:
  //Constructor
  UCASS(); 

  //Functions
  void readConfigVars(const char _CS, Stream &out);
  void readInfoString(const char _CS, Stream &out);
  void readHistData(const char _CS, Stream &out);
  bool startSPI(Stream &out);
  void printConfig(Stream &out, char del);
  void printINFOstr(Stream &out);
  void printHist(Stream &out, char del);

  //Variables
  unsigned char SPIin[60];
  
private:
  //Functions
  void commandByte(unsigned char command, const char _CS, Stream &out);
  void endTransfer(const char _CS);
  void startTransfer(const char _CS);
  float bytetofloat(byte b0, byte b1, byte b2, byte b3);
  uint16_t bytetoint16(byte LSB, byte MSB);
  
};

















