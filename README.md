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

<!---

## Advanced Options

### Using Alert

INA234 can assert an alert on several situations like convertion ready, over power, over current, bus over voltage, bus under voltage, etc. To initialize alert functionality, use `INA234_alert_init` function:
```C
INA234_alert_init(&ina234, ALERT_SHUNT_OVER_LIMIT, ALERT_ACTIVE_LOW, ALERT_TRANSPARENT, ALERT_CONV_DISABLE, 2.5)
```
Each argument is described on the [doc page](https://smotlaq.github.io/ina234/ina234_8c.html#afb44437883ad8f8d08aaf695815da7ed).

** *NOTE1* **  If you choose `ALERT_LATCHED` for alert latch mode, you have to reset the alert pin by calling `INA234_resetAlert` function after each alert assertion. ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a5810f9a740226a39ba5cc2afa6b64f77))

** *NOTE2* **  If you enabled convertion ready alert as well as limit reach functions (like shunt over voltage etc), you have to distinguish the alert source bt calling `INA234_getAlertSource` function. ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a52cc3b785dea1f5af6f0803f02fcefdb))

** *NOTE3* **  The alert pin is open-drain. So don not forget to add a pull-up resistor on this pin.

### Read Parameters Individually

You can read each parameter individually instead of `INA234_readAll` by calling each of these functions:
* `INA234_getShuntVoltage(&ina234);` to read shunt voltage (in mV)
* `INA234_getBusVoltage(&ina234);` to read bus voltage (in V)
* `INA234_getPower(&ina234);` to read power (in W)
* `INA234_getCurrent(&ina234);` to read current (in A)

Example:
```C
#include "ina234.h"

INA234 ina234;
float shunt_voltage, bus_voltage, current, power;

if(STATUS_OK == INA234_init(&ina234, 0x48, &hi2c1, 1, RANGE_20_48mV, NADC_16, CTIME_1100us, CTIME_140us, MODE_CONTINUOUS_BOTH_SHUNT_BUS)){

  shunt_voltage = INA234_getShuntVoltage(&ina234);
  bus_voltage = INA234_getBusVoltage(&ina234);
  current = INA234_getCurrent(&ina234);;
  power = INA234_getPower(&ina234);;
}
```

### Soft Reset

You can send a reset command to all of the INA234 chips on the same bus by calling `INA234_SoftResetAll` function. ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#af3d939ea27371b17fd265f19957234b2))

### Change Settings On The Fly

You can change each of the configurations on the fly using these functions:
* `INA234_setADCRange` to change the ADC full scale range ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#aba71c63deed65a0abdbf7269b5f382d8))
* `INA234_setNumberOfADCSamples` to change the number of averaging ADC samples ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a84ff6173bf6cfa44348ba259a503c804))
* `INA234_setVBusConversionTime` to change the conversion period of VBus ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a94ec7dc7cd10748c4ed822266174d0ff))
* `INA234_setVShuntConversionTime` to change the conversion period of VBus ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#ad19627414a2465c9cf1fac54f54eaa39))
* `INA234_setMode` to change the operating mode ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#ac85c8e736ffae6d248971091b374d00f))

### Getting Manufacturer and Device ID

If you want to get the manufacturer or device ID, you can use these functions:
* `INA234_getManID` ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#ae646f51adec51af1aa6377c3dffeeb6a))
* `INA234_getDevID` ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a88ff1503798836270a41d3b9f3913ca7))

For example:
```C
printf("Manufacturer ID is 0x%4X \r\n", INA234_getManID(&ina234));
printf("      Device ID is 0x%3X \r\n", INA234_getDevID(&ina234));
```

### Get Internal Errors

INA234 can also give the state of internal modules like CPU and memory. By calling `INA234_getErrors` function you can see if there is any error or not. ([see more](https://smotlaq.github.io/ina234/ina234_8c.html#a14a3383eba06ce784ed526585a0cef9a))
--->
