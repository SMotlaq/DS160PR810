#include "debug.h"

void DEBUG(const char* _str, ...){
  #if DEBUG_ENABLE
    va_list args;
    va_start(args, _str);
    char buffer[150];
    memset(buffer, 0, 150);
    int buffer_size = vsprintf(buffer, _str, args);
		#if DEBUG_ON_USB
			CDC_Transmit_FS((uint8_t*) buffer, buffer_size);
			HAL_Delay(1);
		#else
			HAL_UART_Transmit(DEBUG_UART, (uint8_t*)buffer, buffer_size, 5000);
		#endif
  #endif
}