/*
 * bsp.h
 *
 *  Created on: Apr 22, 2021
 *      Author: Administrator
 */

#ifndef BSP_H_
#define BSP_H_

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define TRUE  1
#define FALSE 0

/* Cortext-M7 GPIO-BSRR¼Ä´æÆ÷Ê¹ÓÃ */
#define BS0_SET 0x00000001U
#define BS1_SET 0x00000002U
#define BS2_SET 0x00000004U
#define BS3_SET 0x00000008U
#define BS4_SET 0x00000010U
#define BS5_SET 0x00000020U
#define BS6_SET 0x00000040U
#define BS7_SET 0x00000080U
#define BS8_SET 0x00000100U
#define BS9_SET 0x00000200U
#define BS10_SET 0x00000400U
#define BS11_SET 0x00000800U
#define BS12_SET 0x00001000U
#define BS13_SET 0x00002000U
#define BS14_SET 0x00004000U
#define BS15_SET 0x00008000U

#define BR0_SET 0x00010000U
#define BR1_SET 0x00020000U
#define BR2_SET 0x00040000U
#define BR3_SET 0x00080000U
#define BR4_SET 0x00100000U
#define BR5_SET 0x00200000U
#define BR6_SET 0x00400000U
#define BR7_SET 0x00800000U
#define BR8_SET 0x01000000U
#define BR9_SET 0x02000000U
#define BR10_SET 0x04000000U
#define BR11_SET 0x08000000U
#define BR12_SET 0x10000000U
#define BR13_SET 0x20000000U
#define BR14_SET 0x40000000U
#define BR15_SET 0x80000000U

void user_Assert(char *file,uint32_t line);


#endif /* BSP_H_ */
