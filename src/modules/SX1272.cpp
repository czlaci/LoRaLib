#include "SX1272.h"

SX1272::SX1272(Module* mod) : SX127x(mod) {
  
}

uint8_t SX1272::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t addrEeprom) {
  // execute common part
  uint8_t state = SX127x::begin(syncWord, addrEeprom);
  if(state != ERR_NONE) {
    return(state);
  }
  
  // configure settings not accessible by API
  state = config();
  if(state != ERR_NONE) {
    return(state);
  }
  
  // configure publicly accessible settings
  state = setFrequency(freq);
  if(state != ERR_NONE) {
    return(state);
  }
  
  state = setBandwidth(bw);
  if(state != ERR_NONE) {
    return(state);
  }
  
  state = setSpreadingFactor(sf);
  if(state != ERR_NONE) {
    return(state);
  }
  
  state = setCodingRate(cr);
  if(state != ERR_NONE) {
    return(state);
  }
  
  state = setOutputPower(power);
  if(state != ERR_NONE) {
    return(state);
  }
  
  return(ERR_NONE);
}

uint8_t SX1272::setFrequency(float freq) {
  // check frequency range
  if((freq < 860.0) || (freq > 1020.0)) {
    return(ERR_INVALID_FREQUENCY);
  }
  
  uint8_t state = SX1272::setFrequencyRaw(freq);
  if(state == ERR_NONE) {
    SX127x::_freq = freq;
  }
  return(state);
}

uint8_t SX1272::setBandwidth(float bw) {
  uint8_t newBandwidth;
  
  // check alowed bandwidth values
  if(bw == 125.0) {
    newBandwidth = SX1272_BW_125_00_KHZ;
  } else if(bw == 250.0) {
    newBandwidth = SX1272_BW_250_00_KHZ;
  } else if(bw == 500.0) {
    newBandwidth = SX1272_BW_500_00_KHZ;
  } else {
    return(ERR_INVALID_BANDWIDTH);
  }
  
  uint8_t state = SX1272::setBandwidthRaw(newBandwidth);
  if(state == ERR_NONE) {
    SX127x::_bw = bw;
  }
  
  return(state);
}

uint8_t SX1272::setSpreadingFactor(uint8_t sf) {
  uint8_t newSpreadingFactor;
  
  // check allowed spreading factor values
  switch(sf) {
    case 6:
      newSpreadingFactor = SX127X_SF_6;
      break;
    case 7:
      newSpreadingFactor = SX127X_SF_7;
      break;
    case 8:
      newSpreadingFactor = SX127X_SF_8;
      break;
    case 9:
      newSpreadingFactor = SX127X_SF_9;
      break;
    case 10:
      newSpreadingFactor = SX127X_SF_10;
      break;
    case 11:
      newSpreadingFactor = SX127X_SF_11;
      break;
    case 12:
      newSpreadingFactor = SX127X_SF_12;
      break;
    default:
      return(ERR_INVALID_SPREADING_FACTOR);
  }
  
  uint8_t state = SX1272::setSpreadingFactorRaw(newSpreadingFactor);
  if(state == ERR_NONE) {
    SX127x::_sf = sf;
  }
  
  return(state);
}

uint8_t SX1272::setCodingRate(uint8_t cr) {
  uint8_t newCodingRate;
  
  // check allowed coding rate values
  switch(cr) {
    case 5:
      newCodingRate = SX1272_CR_4_5;
      break;
    case 6:
      newCodingRate = SX1272_CR_4_6;
      break;
    case 7:
      newCodingRate = SX1272_CR_4_7;
      break;
    case 8:
      newCodingRate = SX1272_CR_4_8;
      break;
    default:
      return(ERR_INVALID_CODING_RATE);
  }
  
  uint8_t state = SX1272::setCodingRateRaw(newCodingRate);
  if(state == ERR_NONE) {
    SX127x::_cr = cr;
  }
  
  return(state);
}

uint8_t SX1272::setOutputPower(int8_t power) {
  if((power < -1) || (power > 20)) {
    return(ERR_INVALID_OUTPUT_POWER);
  }
  
  SX127x::standby();
  
  uint8_t state;
  if(power < 15) {
    // power is less than 15 dBm, enable PA0 on RFIO
    state = _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, SX127X_PA_SELECT_RFO, 7, 7);
    state |= _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, power + 1, 3, 0);
    state |= _mod->SPIsetRegValue(SX1272_REG_PA_DAC, SX127X_PA_BOOST_OFF, 2, 0);
  } else if((power >= 15) && (power < 18)) {
    // power is 15 - 17 dBm, enable PA1 + PA2 on PA_BOOST
    state = _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, SX127X_PA_SELECT_BOOST, 7, 7);
    state |= _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, power - 2, 3, 0);
    state |= _mod->SPIsetRegValue(SX1272_REG_PA_DAC, SX127X_PA_BOOST_OFF, 2, 0);
  } else if(power >= 18) {
    // power is 18 - 20 dBm, enable PA1 + PA2 on PA_BOOST and enable high power control
    state = _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, SX127X_PA_SELECT_BOOST, 7, 7);
    state |= _mod->SPIsetRegValue(SX127X_REG_PA_CONFIG, power - 5, 3, 0);
    state |= _mod->SPIsetRegValue(SX1272_REG_PA_DAC, SX127X_PA_BOOST_ON, 2, 0);
  }

  if(state == ERR_NONE) {
    _power = power;
  }
  
  return(state);
}

uint8_t SX1272::setBandwidthRaw(uint8_t newBandwidth) {
  // set mode to standby
  SX127x::standby();
  
  // write register
  return(_mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, newBandwidth, 7, 6));
}

uint8_t SX1272::setSpreadingFactorRaw(uint8_t newSpreadingFactor) {
  // set mode to standby
  SX127x::standby();
  
  // write registers
  uint8_t state = 0;
  if(newSpreadingFactor == SX127X_SF_6) {
    state |= _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, SX1272_HEADER_IMPL_MODE | SX1272_RX_CRC_MODE_OFF, 2, 1);
    state |= _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_2, SX127X_SF_6 | SX127X_TX_MODE_SINGLE, 7, 3);
    state |= _mod->SPIsetRegValue(SX127X_REG_DETECT_OPTIMIZE, SX127X_DETECT_OPTIMIZE_SF_6, 2, 0);
    state |= _mod->SPIsetRegValue(SX127X_REG_DETECTION_THRESHOLD, SX127X_DETECTION_THRESHOLD_SF_6);
  } else {
    state |= _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, SX1272_HEADER_EXPL_MODE | SX1272_RX_CRC_MODE_ON,  2, 1);
    state |= _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_2, newSpreadingFactor | SX127X_TX_MODE_SINGLE, 7, 3);
    state |= _mod->SPIsetRegValue(SX127X_REG_DETECT_OPTIMIZE, SX127X_DETECT_OPTIMIZE_SF_7_12, 2, 0);
    state |= _mod->SPIsetRegValue(SX127X_REG_DETECTION_THRESHOLD, SX127X_DETECTION_THRESHOLD_SF_7_12);
  }
  
  return(state);
}

uint8_t SX1272::setCodingRateRaw(uint8_t newCodingRate) {
  // set mode to standby
  SX127x::standby();
  
  // write register
  return(_mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, newCodingRate, 5, 3));
}

uint8_t SX1272::config() {
  // configure common registers
  uint8_t state = SX127x::config();
  if(state != ERR_NONE) {
    return(state);
  }
  
  // enable LNA gain setting by register
  state = _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_2, SX1272_AGC_AUTO_OFF, 2, 2);
  if(state != ERR_NONE) {
    return(state);
  }
  
  // calculate symbol length and set low datarate optimization, if needed
  uint16_t base = 1;
  float symbolLength = (float)(base << _sf) / (float)_bw;
  if(symbolLength >= 0.016) {
    state = _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, SX1272_LOW_DATA_RATE_OPT_ON, 0, 0);
  } else {
    state = _mod->SPIsetRegValue(SX127X_REG_MODEM_CONFIG_1, SX1272_LOW_DATA_RATE_OPT_OFF, 0, 0);
  }
  
  return(state);
}
