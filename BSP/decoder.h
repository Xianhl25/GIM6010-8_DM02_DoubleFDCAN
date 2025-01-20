#ifndef __DECODER_H__
#define __DECODER_H__

#include "main.h"

uint32_t convertUint8ArrayToUint32(uint8_t data[4]);
float getFloat(uint32_t value);
void convertFloatToString(float num, char *buffer);

#endif