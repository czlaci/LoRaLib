#ifndef _LORALIB_SX1276_H
#define _LORALIB_SX1276_H

#include "TypeDef.h"
#include "SX1278.h"

class SX1276: public SX1278 {
  public:
    SX1276(int nss, float freq, uint32_t bw, uint8_t sf, uint8_t cr, int dio0, int dio1, uint8_t syncWord);
    
    uint8_t config(uint32_t bw, uint8_t sf, uint8_t cr, float freq, uint8_t syncWord);
};

#endif