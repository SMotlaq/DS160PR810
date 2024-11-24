/*!
 * @file ds160pr810.h
 *
 * @mainpage DS160PR810 PCIe 4.0 16-Gbps 8-Channel Linear Redriver Library
 *
 * @section intro_sec Introduction
 *
 * The DS160PR810 is an eight channel low-power high-performance linear redriver designed to support
 * PCIe 4.0 and Ultra Path Interface (UPI) 2.0 up to 16 Gbps. The device is a protocol agnostic linear redriver
 * that can operate for many differential interfaces.
 * 
 * The DS160PR810 receivers deploy continuous-time linear equalizers (CTLE) to provide a high-frequency
 * boost. The equalizer can open an input eye that is completely closed due to inter-symbol interference
 * (ISI) induced by an interconnect medium, such as PCB traces and cables.
 * 
 * The linear redriver along with the passive channel as a whole get link trained for
 * best transmit and receive equalization settings resulting in best electrical link and lowest possible latency.
 * Low channel-channel cross-talk, low additive jitter and excellent return loss allows the device to
 * become almost a passive element in the link.
 * 
 * The devices has internal linear voltage regulator to provide clean power supply for high speed datapaths that
 * provides high immunity to any supply noise on the board.
 * 
 * The DS160PR810 implements high speed testing during production for reliable high volume manufacturing.
 * 
 * The device also has low AC and DC gain variation providing consistant equalization in high volume platform deployment.
 *
 * @section author Author
 *
 * Written by Salman Motlaq (<a href="https://github.com/SMotlaq">@SMotlaq</a> on Github)
 *
 * @section license License
 *
 * MIT License
 *
 * @section start Getting Started
 * Go to [my Github page](https://github.com/SMotlaq) to get started. You can also see the [functions list](./ds160pr810_8c.html) to learn more.
 *
 */

#ifndef __DS160PR810_H_
#define __DS160PR810_H_

#include "main.h"
#include "i2c.h"

// Channel offsets
#define OFFSET_CHANNEL_0 0x00
#define OFFSET_CHANNEL_1 0x20
#define OFFSET_CHANNEL_2 0x40
#define OFFSET_CHANNEL_3 0x60

// Channel Registers
#define RX_DET_STS		0x00
#define EQ_CTRL				0x01
#define GAIN_CTRL			0x02
#define RX_DET_CTRL1	0x03
#define RX_DET_CTRL2	0x09

// Broadcast Write Registers
#define BROADCAST_WRITE_ALL 0x80
#define BROADCAST_WRITE_0_1 0xA0
#define BROADCAST_WRITE_2_3 0xC0

// Shared Registers
#define GENERAL_CONTROL_REGISTER	0xE2
#define EEPROM_STATUS_REGISTER		0xE3
#define DEVICE_ID0_REGISTER				0xF0
#define DEVICE_ID1_REGISTER				0xF1

/// Status enum
typedef enum Status					{STATUS_OK, 							///< Means everything is OK
														 STATUS_TimeOut,					///< The commiunication timed out
														 STATUS_ArgumentError			///< Arguments are wrong
														} Status;

/// Bank indexes
typedef enum BankNumber			{BANK0,	///< Bank 0
														 BANK1	///< Bank 1
														} BankNumber;

/// Rx Detect Status
typedef enum RxDetectStatus	{RX_DET_NONE,	///< Nothing detected
														 RX_DET_N,		///< Detected on N
														 RX_DET_P,		///< Detected on P
														 RX_DET_BOTH	///< Detected on both
														} RxDetectStatus;

/// Bypass state of CTLE first stage
typedef enum EQStage1Bypass	{EQ_STAGE1_BYPASS_DISABLE,	///< Bypass is disable (Stage 1 is active)
														 EQ_STAGE1_BYPASS_ENABLE		///< Bypass is enale (Stage 1 is bypassed)
														} EQStage1Bypass;

/// DC gain
typedef enum EQDCGain				{EQ_DC_GAIN_0db,		///< 0 db
														 EQ_DC_GAIN_3db5		///< 3.5 db
														} EQDCGain;

/// TX VOD
typedef enum TxVOD					{TX_VOD_m6db,		///< -6 db
														 TX_VOD_m3db5,	///< -3.5 db
														 TX_VOD_m1db6,	///< -1.6 db
														 TX_VOD_0db			///< 0 db
														} TxVOD;

typedef enum ApplyTo				{BANK0_CHANNEL0   = OFFSET_CHANNEL_0,						///< Channel 0 of Bank 0 (Channel 0)
														 BANK0_CHANNEL1   = OFFSET_CHANNEL_1,						///< Channel 1 of Bank 0 (Channel 1)
														 BANK0_CHANNEL2   = OFFSET_CHANNEL_2,						///< Channel 2 of Bank 0 (Channel 2)
														 BANK0_CHANNEL3   = OFFSET_CHANNEL_3,						///< Channel 3 of Bank 0 (Channel 3)
														 BANK0_CHANNEL0_1 = BROADCAST_WRITE_0_1,				///< Channel 0 and 1 of Bank 0 (Channel 0 and 1)
														 BANK0_CHANNEL2_3 = BROADCAST_WRITE_2_3,				///< Channel 2 and 3 of Bank 0 (Channel 2 and 3)
	
														 BANK1_CHANNEL0   = OFFSET_CHANNEL_0 | 0x01,		///< Channel 0 of Bank 1 (Channel 4)
														 BANK1_CHANNEL1   = OFFSET_CHANNEL_1 | 0x01,		///< Channel 1 of Bank 1 (Channel 5)
														 BANK1_CHANNEL2   = OFFSET_CHANNEL_2 | 0x01,		///< Channel 2 of Bank 1 (Channel 6)
														 BANK1_CHANNEL3   = OFFSET_CHANNEL_3 | 0x01,		///< Channel 3 of Bank 1 (Channel 7)
														 BANK1_CHANNEL0_1 = BROADCAST_WRITE_0_1 | 0x01,	///< Channel 0 and 1 of Bank 1 (Channel 4 and 5)
														 BANK1_CHANNEL2_3 = BROADCAST_WRITE_2_3 | 0x01,	///< Channel 2 and 3 of Bank 1 (Channel 6 and 7)

														 CHANNEL0   = OFFSET_CHANNEL_0,									///< Channel 0 of Bank 0 (Channel 0)
														 CHANNEL1   = OFFSET_CHANNEL_1,									///< Channel 1 of Bank 0 (Channel 1)
														 CHANNEL2   = OFFSET_CHANNEL_2,									///< Channel 2 of Bank 0 (Channel 2)
														 CHANNEL3   = OFFSET_CHANNEL_3,									///< Channel 3 of Bank 0 (Channel 3)
														 CHANNEL4   = OFFSET_CHANNEL_0 | 0x01,					///< Channel 0 of Bank 1 (Channel 4)
														 CHANNEL5   = OFFSET_CHANNEL_1 | 0x01,					///< Channel 1 of Bank 1 (Channel 5)
														 CHANNEL6   = OFFSET_CHANNEL_2 | 0x01,					///< Channel 2 of Bank 1 (Channel 6)
														 CHANNEL7   = OFFSET_CHANNEL_3 | 0x01,					///< Channel 3 of Bank 1 (Channel 7)
														 CHANNEL0_1 = BROADCAST_WRITE_0_1,							///< Channel 0 and 1 of Bank 0 (Channel 0 and 1)
														 CHANNEL2_3 = BROADCAST_WRITE_2_3,							///< Channel 2 and 3 of Bank 0 (Channel 2 and 3)
														 CHANNEL4_5 = BROADCAST_WRITE_0_1 | 0x01,				///< Channel 0 and 1 of Bank 1 (Channel 4 and 5)
														 CHANNEL6_7 = BROADCAST_WRITE_2_3 | 0x01,				///< Channel 2 and 3 of Bank 1 (Channel 6 and 7)

														 BANK0_ALL_CHANNELS = BROADCAST_WRITE_ALL,			///< Channel 0 to 3 of Bank 1 (Channel 0 to 3)
														 BANK1_ALL_CHANNELS = BROADCAST_WRITE_ALL | 0x01,	///< Channel 0 to 3 of Bank 1 (Channel 4 to 7)
														 ALL_BANKS = 0xFF																///< Channel 0 to 3 of all banks (Channel 0 to 7)
														 
														} ApplyTo;
														

/*! 
    @brief  Class (struct) that stores variables for interacting with DS160PR180
*/
typedef struct ds160pr810{
	
	I2C_HandleTypeDef*	hi2c;										/*!< Specifies the I2C handler. */
	uint8_t 						BANK0_I2C_ADDR;
	
	union _reg {
		uint8_t raw_data;
		// LSb -------------- MSb \\
		
		struct _general_control_register{
			uint8_t frc_eeprom_rd:1, RESERVED:5, rst_i2c_regs:1, RESERVED2:1;
		} general_control_register;
		
		struct _eeprom_status_register{
			uint8_t RESERVED:4, eecfg_atmpt:2, eecfg_fail:1, eecfg_cmplt:1;
		} eeprom_status_register;
		
		struct _device_id0_register{
			uint8_t RESERVED:1, device_id0:3, RESERVED2:4;
		} device_id0_register;
		
		struct _device_id1_register{
			uint8_t device_id1;
		} device_id1_register;
		
		struct _rx_det_sts{
			uint8_t RESERVED:6, rx_det_comp_n:1, rx_det_comp_p:1;
		} rx_det_sts;
		
		struct _eq_ctrl{
			uint8_t eq_bst2:3, eq_bst1:3, eq_en_bypass:1, RESERVED:1;
		} eq_ctrl;
		
		struct _gain_ctrl{
			uint8_t drv_sel_vod:2, eq_hi_gain:1, RESERVED:5;
		} gain_ctrl;
		
		struct _rx_det_ctrl1{
			uint8_t sel_rx_det_count:1, en_rx_det_count:1, mr_rx_det_man:1, RESERVED:5;
		} rx_det_ctrl1;
		
		struct _rx_det_ctrl2{
			uint8_t RESERVED:2, mr_rx_det_rst:1, RESERVED2:5;
		} rx_det_ctrl2;

	} reg;

} DS160PR810;

Status	DS160PR810_init(DS160PR810* self, I2C_HandleTypeDef* hi2c, uint8_t BANK0_I2C_ADDR);
Status	DS160PR810_resetRegisters(DS160PR810* self);

// Privates ----------------------------------
Status __DS160PR810_writeByte(DS160PR810* self, BankNumber bank_number, uint8_t MemAddress);
Status __DS160PR810_readByte(DS160PR810* self, BankNumber bank_number, uint8_t MemAddress);
Status __DS160PR810_setChannelConfig(DS160PR810* self, ApplyTo target, uint8_t ChannelRegister);
Status __DS160PR810_getChannelConfig(DS160PR810* self, ApplyTo target, uint8_t ChannelRegister);

// Set Config --------------------------------
Status DS160PR810_setCTLE(DS160PR810* self, ApplyTo target, EQStage1Bypass isByPassed, uint8_t Stage1, uint8_t Stage2);
Status DS160PR810_setResetRxDetectStateMachine(DS160PR810* self, ApplyTo target);
Status DS160PR810_setClearRxDetectStateMachine(DS160PR810* self, ApplyTo target);
Status DS160PR810_setDCGain(DS160PR810* self, ApplyTo target, TxVOD tx_vod, EQDCGain dc_gain);

// Get Config --------------------------------
EQStage1Bypass	DS160PR810_getCTLE_EQStage1Bypass(DS160PR810* self, ApplyTo target);
uint8_t					DS160PR810_getCTLE_Stage1(DS160PR810* self, ApplyTo target);
uint8_t					DS160PR810_getCTLE_Stage2(DS160PR810* self, ApplyTo target);
TxVOD						DS160PR810_getDCGain_TxVOD(DS160PR810* self, ApplyTo target);
EQDCGain				DS160PR810_getDCGain_EQDCGain(DS160PR810* self, ApplyTo target);
RxDetectStatus	DS160PR810_getRxDetectStatus(DS160PR810* self, ApplyTo target);

// Get Other Data ----------------------------
uint8_t DS160PR810_getDevID0(DS160PR810* self, BankNumber bank_number);
uint8_t	DS160PR810_getDevID1(DS160PR810* self, BankNumber bank_number);

uint8_t	DS160PR810_getEELoadComplete(DS160PR810* self, BankNumber bank_number);
uint8_t	DS160PR810_getEELoadFail(DS160PR810* self, BankNumber bank_number);
uint8_t	DS160PR810_getEEAtemptCount(DS160PR810* self, BankNumber bank_number);

#endif