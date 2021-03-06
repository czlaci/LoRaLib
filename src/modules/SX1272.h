#ifndef _KITELIB_SX1272_H
#define _KITELIB_SX1272_H

#include "TypeDef.h"
#include "Module.h"
#include "SX127x.h"

//SX1272 specific register map
#define SX1272_REG_AGC_REF                            0x43
#define SX1272_REG_AGC_THRESH_1                       0x44
#define SX1272_REG_AGC_THRESH_2                       0x45
#define SX1272_REG_AGC_THRESH_3                       0x46
#define SX1272_REG_PLL_HOP                            0x4B
#define SX1272_REG_TCXO                               0x58
#define SX1272_REG_PA_DAC                             0x5A
#define SX1272_REG_PLL                                0x5C
#define SX1272_REG_PLL_LOW_PN                         0x5E
#define SX1272_REG_FORMER_TEMP                        0x6C
#define SX1272_REG_BIT_RATE_FRAC                      0x70

//SX1272 LoRa modem settings
//SX1272_REG_FRF_MSB + REG_FRF_MID + REG_FRF_LSB
#define SX1272_FRF_MSB                                0xE4        //  7     0     carrier frequency setting: f_RF = (F(XOSC) * FRF)/2^19
#define SX1272_FRF_MID                                0xC0        //  7     0         where F(XOSC) = 32 MHz
#define SX1272_FRF_LSB                                0x00        //  7     0               FRF = 3 byte value of FRF registers

//SX1272_REG_MODEM_CONFIG_1
#define SX1272_BW_125_00_KHZ                          0b00000000  //  7     6     bandwidth:  125 kHz
#define SX1272_BW_250_00_KHZ                          0b01000000  //  7     6                 250 kHz
#define SX1272_BW_500_00_KHZ                          0b10000000  //  7     6                 500 kHz
#define SX1272_CR_4_5                                 0b00001000  //  5     3     error coding rate:  4/5
#define SX1272_CR_4_6                                 0b00010000  //  5     3                         4/6
#define SX1272_CR_4_7                                 0b00011000  //  5     3                         4/7
#define SX1272_CR_4_8                                 0b00100000  //  5     3                         4/8
#define SX1272_HEADER_EXPL_MODE                       0b00000000  //  2     2     explicit header mode
#define SX1272_HEADER_IMPL_MODE                       0b00000100  //  2     2     implicit header mode
#define SX1272_RX_CRC_MODE_OFF                        0b00000000  //  1     1     CRC disabled
#define SX1272_RX_CRC_MODE_ON                         0b00000010  //  1     1     CRC enabled
#define SX1272_LOW_DATA_RATE_OPT_OFF                  0b00000000  //  0     0     low data rate optimization disabled
#define SX1272_LOW_DATA_RATE_OPT_ON                   0b00000001  //  0     0     low data rate optimization enabled, mandatory for SF 11 and 12 with BW 125 kHz

//SX1272_REG_MODEM_CONFIG_2
#define SX1272_AGC_AUTO_OFF                           0b00000000  //  2     2     LNA gain set by REG_LNA
#define SX1272_AGC_AUTO_ON                            0b00000100  //  2     2     LNA gain set by internal AGC loop

class SX1272: public SX127x {
  public:
    SX1272(Module* mod);
    
    uint8_t begin(float freq = 915.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = SX127X_SYNC_WORD, int8_t power = 17, uint16_t addrEeprom = 0);
    
    uint8_t setFrequency(float freq);
    uint8_t setBandwidth(float bw);
    uint8_t setSpreadingFactor(uint8_t sf);
    uint8_t setCodingRate(uint8_t cr);
    uint8_t setOutputPower(int8_t power);
  
  protected:
    uint8_t setBandwidthRaw(uint8_t newBandwidth);
    uint8_t setSpreadingFactorRaw(uint8_t newSpreadingFactor);
    uint8_t setCodingRateRaw(uint8_t newCodingRate);

  private:
    uint8_t config();
};

#endif
