/*
 * bsp.c
 *
 *  Created on: Apr 22, 2021
 *      Author: Administrator
 */
#include "bsp.h"

/* 本地宏 */

#define USING_RTX5 0  /* 使用裸机还是RTX5（嵌入式实时系统） */

/**
  * 函数功能: 检查代码异常，打印出异常的位置（文件名与行号）
  * 输入参数: file: 源代码文件名称。关键字__FILE__表示源代码文件名。
  *        line: 代码行号，关键字__LINE__表示源代码行号
   * 返回值：  void

  * 说明:
  *    1、在需要断言的地方直接调用user_Assert(__FILE__,__LINE__);
  */
void user_Assert(char *file,uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n",file, (unsigned int)line);

    /* 这是一个死循环，断言失败时程序会卡此处死机，以便于用户差错 */
	if(line == 0)
	{
		return;
	}
	while(1)
	{
#if USING_RTX5
		 osKernelLock();  /* 将所有线程的调度关闭 */
		
		 printf("Wrong parameters value: file %s on line %d\r\n",file, (unsigned int)line);
		 HAL_Delay(300);
	
#else		
		 printf("Wrong parameters value: file %s on line %d\r\n",file, (unsigned int)line);
		 HAL_Delay(300);
		
#endif
	}
}









