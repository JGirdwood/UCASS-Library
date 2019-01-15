#include "UCASS.h"
#include <SPI.h>
#include <Arduino.h>

UCASS::UCASS(){

}

bool UCASS::startSPI(Stream &out){
  SPI.begin();
  out.println("SPI active");
  return true;
}

void UCASS::commandByte(unsigned char command, const char _CS, Stream &out){
  //char UCASS_ready = 0xF3;
  unsigned char Response;
  //do{
    Response = SPI.transfer(command);
    /*if (Response != UCASS_ready){
      out.println("UCASS not communicating (wait 2s)");
      delay(2000);
    }*/
    delay(10);
  //}while (Response != UCASS_ready);
}

void UCASS::startTransfer(const char _CS){
  SPI.beginTransaction(SPISettings(300000, MSBFIRST, SPI_MODE1));
  digitalWrite(_CS, LOW);
}

void UCASS::endTransfer(const char _CS){
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

float UCASS::bytetofloat(byte b0, byte b1, byte b2, byte b3){
  byte bytesin[4] = {b0, b1, b2, b3};
  float out = *(float *)&bytesin;
  return out;
}

uint16_t UCASS::bytetoint16(byte LSB, byte MSB){
  byte buf[2] = {LSB, MSB};
  uint16_t num = (*((int *)buf));
  return num;
}

void UCASS::readHistData(const char _CS, Stream &out){
  startTransfer(_CS);
  commandByte(0x30, _CS, out);
  for (byte i=0; i<=42; i++){
    delayMicroseconds(10);
    SPIin[i] = SPI.transfer(0x30);
  }
  endTransfer(_CS);
}

void UCASS::readInfoString(const char _CS, Stream &out){
  startTransfer(_CS);
  commandByte(0x3F, _CS, out);
  for (byte i=0; i<=59; i++){
    delayMicroseconds(10);
    SPIin[i] = SPI.transfer(0x3F);
  }
  endTransfer(_CS);
}

void UCASS::readConfigVars(const char _CS, Stream &out){
  startTransfer(_CS);
  commandByte(0x3C, _CS, out);
  for (byte i=0; i<=37; i++){
    delayMicroseconds(10);
    SPIin[i] = SPI.transfer(0x30);
  }
  endTransfer(_CS);
}

void UCASS::printINFOstr(Stream &out){
  for (byte i=0; i<=59; i++){
    out.print((char)SPIin[i]);
  }
  out.println();
}

void UCASS::printConfig(Stream &out, char del){
  for (byte i=0; i<=15; i++){
    out.print(bytetoint16(SPIin[2*i],SPIin[2*i+1]));
    out.print(del);
  }
  out.print(bytetofloat(SPIin[32], SPIin[33], SPIin[34], SPIin[35]));
  out.print(del);
  out.println(SPIin[37]);
}

void UCASS::printHist(Stream &out, char del){
  uint8_t index = 0;
  for (byte i=0; i<=15; i++){
    out.print(bytetoint16(SPIin[2*index],SPIin[2*index+1]));
    out.print(del);
    index = index+2;
  }
  for (byte i=0; i<=3; i++){
    out.print(SPIin[index]);
    out.print(del);
    index++;
  }
  out.print(bytetoint16(SPIin[36],SPIin[37]));
  out.print(del);
  out.print(bytetoint16(SPIin[38],SPIin[39]));
  out.print(del);
  for (byte i=0; i<=1; i++){
    out.print(SPIin[index]);
    out.print(del);
    index++;
  }
  out.print(SPIin[42]);
}



































