[![Doxygen Action](https://github.com/SMotlaq/DS160PR810/actions/workflows/main2.yml/badge.svg)](https://github.com/SMotlaq/DS160PR810/actions/workflows/main2.yml)
[![pages-build-deployment](https://github.com/SMotlaq/DS160PR810/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/SMotlaq/DS160PR810/actions/workflows/pages/pages-build-deployment)


# DS160PR810 HAL Based Library

The DS160PR810 is an eight channel low-power high-performance linear redriver designed to support PCIe 4.0 and Ultra Path Interface (UPI) 2.0 up to 16 Gbps. The device is a protocol agnostic linear redriver that can operate for many differential interfaces. ([Click for more info](https://www.ti.com/product/DS160PR810?keyMatch=DS160PR810))

This library is a software library that works with the DS160PR810 PCIe 4.0 16-Gbps 8-channel linear redriver. This library provides a convenient and efficient way to access the I2C interfaces of the chip, allowing developers to easily integrate this redriver into their systems.

The library is designed to be easy to use and provides a simple, intuitive API for accessing the I2C interfaces of the DS160PR810. It includes a range of functions for performing common I2C operations, such as sending and receiving data, querying the status of the chip, reading,  and configuring the DS160PR810 settings.

With this library, developers can quickly and easily integrate the DS160PR810 into their systems, enabling them to take full advantage of the chip's capabilities.

<p align="center">
  <img src="https://github.com/SMotlaq/ds160pr810/blob/images/images/all.png"/>
</p>

## Key Features

* Easy-to-use API for accessing the I2C interfaces of the DS160PR810
* Support for common I2C operations, such as sending and receiving data, querying the status of the chip, reading, and configuring the DS160PR810 settings

## Documentations

The full documents are available [here](https://smotlaq.github.io/DS160PR810/)

## Schematic Symbol and Footprint

Footprint and schematic symbols are available in [my Altium library](https://github.com/SMotlaq/altium-library).

## Donate
Is it helpfull?

<p align="left">
  <a href="http://smotlaq.ir/LQgQF">
  <img src="https://raw.githubusercontent.com/SMotlaq/LoRa/master/bmc.png" width="200" alt="Buy me a Coffee"/>
  </a>
</p>

# Getting Started

## Quick Start

1. Download the library source from the [latest release](http://github.com/smotlaq/DS160PR810/releases/latest)

2. Copy `ds160pr810.c` and `ds160pr810.h` file to your project directory and add them to your IDE if necessary.

3. It is recommended to import [Debugger](https://github.com/SMotlaq/DS160PR810/tree/master/Debug) amd [Logger](https://github.com/SMotlaq/DS160PR810/tree/master/Logger) as well, to simply use logs.

4. Inclued the library into your project:
   ```C
   #include "ds160pr810.h"
   ```

5. Create an object (instanse) from DS160PR810 struct with desired name:
   ```C
   DS160PR810 my_redriver;
   ```

6. Initialize the chip:
   ```C
   DS160PR810_init(&my_redriver, &hi2c1, 0x18);
   ```
   Each argument is described on the [doc page](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#aad727c8de8def27e0ef2a4da11551735).

7. Now you can config the redriver CTLE using `DS160PR810_setCTLE` function. For example:
   ```C
   DS160PR810_setCTLE(&my_redriver, BANK0_ALL_CHANNELS, EQ_STAGE1_BYPASS_DISABLE, 4, 2);
   ```
   Or
   ```C
   DS160PR810_setCTLE(&my_redriver, BANK1_CHANNEL0_1, EQ_STAGE1_BYPASS_DISABLE, 2, 3);
   ```
   Or
   ```C
   DS160PR810_setCTLE(&my_redriver, CHANNEL6_7, EQ_STAGE1_BYPASS_ENABLE, 1, 7);
   ```
   Full description of this function is available [here](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#aeefd615940a76cab2cdb2d52b1209f70).

8. Then you can config the DC gain and TX VOD using `DS160PR810_setDCGain` function. For example:
   ```C
   DS160PR810_setDCGain(&my_redriver, CHANNEL0_1, TX_VOD_0db, EQ_DC_GAIN_3db5);
   ```
   Or
   ```C
	 DS160PR810_setDCGain(&my_redriver, BANK0_CHANNEL2_3, TX_VOD_m3db5, EQ_DC_GAIN_0db);
   ```
   Or
   ```C
	 DS160PR810_setDCGain(&my_redriver, BANK1_ALL_CHANNELS, TX_VOD_m1db6, EQ_DC_GAIN_3db5);	
   ```
   Full description of this function is available [here](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#ad1bf85cf0f785428d683b15e4a03e428).

Here is the whole code:
```C
#include "ds160pr810.h"

DS160PR810 my_redriver;

if(STATUS_OK == DS160PR810_init(&my_redriver, &hi2c1, 0x18)){

  // Set CTLE
  DS160PR810_setCTLE(&my_redriver, BANK0_ALL_CHANNELS, EQ_STAGE1_BYPASS_DISABLE, 4, 2);
  DS160PR810_setCTLE(&my_redriver, BANK1_ALL_CHANNELS, EQ_STAGE1_BYPASS_DISABLE, 3, 1);

  // Set DC gain
  DS160PR810_setDCGain(&my_redriver, ALL_BANKS, TX_VOD_0db, EQ_DC_GAIN_0db);
  
}
```
### Using some useful tools

If you want to use UART or virtual USB COM port on your microcontroller, it is recommended to use my [Debug](https://github.com/SMotlaq/DS160PR810/tree/master/Debug) tool and include it to your project:
```C
#include "debug.h"
```

Then open the `debug.h` and config the print settings acording to your setup: (you can ignore `DEBUG_UART` if using USB)
```C
// Print setting -------------------
#define DEBUG_ENABLE  1
#define DEBUG_ON_USB  1
#define DEBUG_UART    (&huart1)
// ---------------------------------
```

By applying the above trick, you can simply use `DEBUG(...)` instead of conventional `printf(...)` to see status on the serial terminal.

There is another tool that helps you to check the redriver status better, just by importing [logger](https://github.com/SMotlaq/DS160PR810/tree/master/Logger) and using its functions. Here is the previous code with these tools:
```C
#include "ds160pr810.h"
#include "debug.h"
#include "logger.h"

DS160PR810 my_redriver;

if(STATUS_OK == DS160PR810_init(&my_redriver, &hi2c1, 0x18)){
  
  DEBUG("Redriver init done \n\r");
  
  // Set CTLE
  DS160PR810_setCTLE(&my_redriver, BANK0_ALL_CHANNELS, EQ_STAGE1_BYPASS_DISABLE, 4, 2);
  DS160PR810_setCTLE(&my_redriver, BANK1_ALL_CHANNELS, EQ_STAGE1_BYPASS_ENABLE, 0, 6);

  // Set DC gain
  DS160PR810_setDCGain(&my_redriver, ALL_BANKS, TX_VOD_0db, EQ_DC_GAIN_0db);

  DEBUG("Redriver config done \n\r");

  log_All2(&my_redriver);
  
}
else{
  DEBUG("Redriver init failed \n\r");
}
```

The output will be like this:

```
Redriver init done 
Redriver config done 
---------------------------------------------------------------------------------
| Ch index | Stage 1 Stat | Stage 1 | Stage 2 | DC Gain |  Tx VOD  | RX  Detect |
---------------------------------------------------------------------------------
|     0    |    ACTIVE    |    4    |    2    |  0   db |   0   db |    NONE    |
|     1    |    ACTIVE    |    4    |    2    |  0   db |   0   db |    NONE    |
|     2    |    ACTIVE    |    4    |    2    |  0   db |   0   db |    NONE    |
|     3    |    ACTIVE    |    4    |    2    |  0   db |   0   db |    NONE    |
|     4    |   BYPASSED   |    0    |    6    |  0   db |   0   db |    NONE    |
|     5    |   BYPASSED   |    0    |    6    |  0   db |   0   db |    NONE    |
|     6    |   BYPASSED   |    0    |    6    |  0   db |   0   db |    NONE    |
|     7    |   BYPASSED   |    0    |    6    |  0   db |   0   db |    NONE    |
---------------------------------------------------------------------------------
```

## Advanced Options

### Reset Registers

DS160PR810 can reset the register values by software. If you want to reset all of the register values you can use `DS160PR810_resetRegisters` function:
```C
DS160PR810_resetRegisters(&my_redriver);
```
([see more](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a1d98103b38ed2c1af036bd7037843503))

### Getting the Current Configurations

You can read each setting individually by calling the appropriate function:
* `DS160PR810_getCTLE_EQStage1Bypass` to read first stage status ([see more](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#ace8ba1c50c6f6a35a8df3868612c39b7))
* `DS160PR810_getCTLE_Stage1` to read the gain index of first stage [(see more)](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a1e44272733c6969bebd47ce470f65500)
* `DS160PR810_getCTLE_Stage2` to read the gain index of first stage [(see more)](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a35e3211a6abb2af46a9f2ad445661b69)
* `DS160PR810_getDCGain_EQDCGain` to read the DC gain [(see more)](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a9466df8da105a840b0c7c441b991ae00)
* `DS160PR810_getDCGain_TxVOD` to read the TX VOD [(see more)](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a48c2933fff20556f533feafff955edc9)

Example:
```C
void log_EQ_Channel(DS160PR810* self, ApplyTo channel){
	DEBUG("   - EQ Settings:\n\r");
	DEBUG("     - Stage1 Status: %s \n\r", DS160PR810_getCTLE_EQStage1Bypass(self, channel) ? "BY PASSED" : "ACTIVE");
	DEBUG("     -     Stage1 EQ: %d \n\r", DS160PR810_getCTLE_Stage1(self, channel));
	DEBUG("     -     Stage2 EQ: %d \n\r", DS160PR810_getCTLE_Stage2(self, channel));	
}

log_EQ_Channel(&my_redriver, CHANNEL0);
```

### RX Detect Status

You can read the RX detect status by calling the `DS160PR810_getRxDetectStatus` function. To see the return values [see here](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#ab5092214a1e7e65b63db2b5668887031)

### Getting Device IDs

If you want to get the device IDs, you can use these functions:
* `DS160PR810_getDevID0` ([see more](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a9f310172bdf468bde368aed6a7bcd5ce))
* `DS160PR810_getDevID1` ([see more](https://smotlaq.github.io/DS160PR810/ds160pr810_8c.html#a54b4a16d44e2365494e9031f3c829d52))

For example:
```C
DEBUG("The IDs are: \n\r Bank0: \n\r  - ID1: 0x%02X \n\r  - ID0: 0x%02X \n\r Bank1: \n\r  - ID1: 0x%02X \n\r  - ID0: 0x%02X \n\r",
	DS160PR810_getDevID1(&my_redriver, BANK0),
	DS160PR810_getDevID0(&my_redriver, BANK0),
	DS160PR810_getDevID1(&my_redriver, BANK1),
	DS160PR810_getDevID0(&my_redriver, BANK1)
);
```
