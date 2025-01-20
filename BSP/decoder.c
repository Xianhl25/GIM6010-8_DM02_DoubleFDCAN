#include <stdint.h>
#include <stdio.h>
#include "decoder.h"


/**
  * 函数功能: 用于将类型为uint8_t [8]的接收数组转换为uint32_t类型
  * 输入参数: uint8_t data[4]
  *           
  * 返回值：  uint32_t
  *           
  *
  * 说明:
  *     1. 
  */
uint32_t convertUint8ArrayToUint32(uint8_t data[4])
{
	return (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

/**
  * 函数功能: 用于32为十六进制数据根据IEEE743协议转换为单精度浮点数
  * 输入参数: uint32_t
  *           
  * 返回值：  float
  *           
  *
  * 说明:
  *     1. 
  */
float getFloat(uint32_t value) 
{
	/* 基于IEEE743协议将十六进制数据转换为单精度浮点数 */
	
    /* 提取符号位 (S) */
    unsigned int value_s = (value >> 31) & 0x01;
    
    /* 提取指数位 (E) */
    unsigned int value_e = (value >> 23) & 0xFF;
    
    /* 计算尾数位 (M) */
    float value_M = 0.0f;
	
    for (int i = 0; i < 23; i++) 
	{
        int binaryData = (value >> i) & 1;
        
        float bitX = powf(2, (-23 + i));
		
        value_M += binaryData * bitX;
    }

    /* 计算 (E - 127) */
    float E_127 = (float)value_e - 127.0f;

    /* 计算最终的浮点数值 */
    float res = powf(-1, value_s) * (1.0f + value_M) * powf(2, E_127);

    return res;
}

/**
  * 函数功能: 用于将浮点数转换成字符串，便于LCD显示，保留两位小数
  * 输入参数: float
  *           
  * 返回值：  string
  *           
  *
  * 说明:
  *     1. 
  */
void convertFloatToString(float num, char *buffer)
{
	sprintf(buffer, "%.2f", num); /* 保留两位小数 */
}
