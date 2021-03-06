#ifndef _LORALIB_MODULE_H
#define _LORALIB_MODULE_H

#include <SPI.h>

#include "TypeDef.h"

#define SPI_READ  0b00000000
#define SPI_WRITE 0b10000000

class Module {
  public:
    Module(int cs = 7, int int0 = 2, int int1 = 3);
    
    uint8_t init(uint8_t interface, uint8_t gpio);
    
    uint8_t SPIgetRegValue(uint8_t reg, uint8_t msb = 7, uint8_t lsb = 0);
    uint8_t SPIreadRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes);
    uint8_t SPIreadRegisterBurstStr(uint8_t reg, uint8_t numBytes, char* str);
    uint8_t SPIreadRegister(uint8_t reg);
    
    uint8_t SPIsetRegValue(uint8_t reg, uint8_t value, uint8_t msb = 7, uint8_t lsb = 0);
    void SPIwriteRegisterBurst(uint8_t reg, uint8_t* data, uint8_t numBytes);
    void SPIwriteRegisterBurstStr(uint8_t reg, const char* data, uint8_t numBytes);
    void SPIwriteRegister(uint8_t reg, uint8_t data);
    
    bool getInt0State();
    bool getInt1State();
  
  private:
    int _cs;
    int _int0;
    int _int1;

};

#endif
