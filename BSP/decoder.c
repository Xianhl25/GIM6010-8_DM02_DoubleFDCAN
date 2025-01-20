#include <stdint.h>
#include <stdio.h>
#include "decoder.h"


/**
  * ��������: ���ڽ�����Ϊuint8_t [8]�Ľ�������ת��Ϊuint32_t����
  * �������: uint8_t data[4]
  *           
  * ����ֵ��  uint32_t
  *           
  *
  * ˵��:
  *     1. 
  */
uint32_t convertUint8ArrayToUint32(uint8_t data[4])
{
	return (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

/**
  * ��������: ����32Ϊʮ���������ݸ���IEEE743Э��ת��Ϊ�����ȸ�����
  * �������: uint32_t
  *           
  * ����ֵ��  float
  *           
  *
  * ˵��:
  *     1. 
  */
float getFloat(uint32_t value) 
{
	/* ����IEEE743Э�齫ʮ����������ת��Ϊ�����ȸ����� */
	
    /* ��ȡ����λ (S) */
    unsigned int value_s = (value >> 31) & 0x01;
    
    /* ��ȡָ��λ (E) */
    unsigned int value_e = (value >> 23) & 0xFF;
    
    /* ����β��λ (M) */
    float value_M = 0.0f;
	
    for (int i = 0; i < 23; i++) 
	{
        int binaryData = (value >> i) & 1;
        
        float bitX = powf(2, (-23 + i));
		
        value_M += binaryData * bitX;
    }

    /* ���� (E - 127) */
    float E_127 = (float)value_e - 127.0f;

    /* �������յĸ�����ֵ */
    float res = powf(-1, value_s) * (1.0f + value_M) * powf(2, E_127);

    return res;
}

/**
  * ��������: ���ڽ�������ת�����ַ���������LCD��ʾ��������λС��
  * �������: float
  *           
  * ����ֵ��  string
  *           
  *
  * ˵��:
  *     1. 
  */
void convertFloatToString(float num, char *buffer)
{
	sprintf(buffer, "%.2f", num); /* ������λС�� */
}
