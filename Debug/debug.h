#ifndef __DEBUG_H_
#define __DEBUG_H_

// Print setting -------------------
#define DEBUG_ENABLE  1
#define DEBUG_ON_USB  1
#define DEBUG_UART    (&huart1)
// ---------------------------------

#if DEBUG_ENABLE
	#include "main.h"
  #include "stdarg.h"
  #include "string.h"
	#include "stdio.h"
	
	#if DEBUG_ON_USB
		#include "usbd_cdc_if.h"
	#else
		#include "usart.h"
	#endif
	
#endif

void DEBUG(const char* _str, ...);

#endif