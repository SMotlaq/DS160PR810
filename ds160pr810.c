/*!
 * @file ds160pr810.c
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

#include "ds160pr810.h"

/*!
    @brief  Initialize the DS160PR810 with the given config
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	hi2c
						A pointer to the I2C handler that is connected to DS160PR810
		@param	BANK0_I2C_ADDR
						The I2C address of the bank0 of the DS160PR810. It depends on the state of ADDR0 and ADDR1 pins of the chip.
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status DS160PR810_init(DS160PR810* self, I2C_HandleTypeDef* hi2c, uint8_t BANK0_I2C_ADDR){
	
	// Init Variables -----------------------
	self->hi2c = hi2c;
	self->BANK0_I2C_ADDR = BANK0_I2C_ADDR;
	self->reg.raw_data = 0;
	
	if(STATUS_OK != __DS160PR810_readByte(self, BANK0, DEVICE_ID1_REGISTER)) return STATUS_TimeOut;
	if(self->reg.device_id1_register.device_id1 == 0x27) return STATUS_OK;
	return STATUS_TimeOut;
}

/*!
    @brief  Resets the DS160PR810 registers
    @param  self
            A pointer to the ds160pr810 object (struct)
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status	DS160PR810_resetRegisters(DS160PR810* self){
	self->reg.raw_data = 0;
	self->reg.general_control_register.rst_i2c_regs = 1;
	if(__DS160PR810_writeByte(self, BANK0, GENERAL_CONTROL_REGISTER)) return STATUS_TimeOut;
	if(__DS160PR810_writeByte(self, BANK1, GENERAL_CONTROL_REGISTER)) return STATUS_TimeOut;
	return STATUS_OK;
}

// Privates ----------------------------------

/*!
    @brief  Write a byte of data to DS160PR810 from the ds160pr810::_reg::raw_data
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	bank_number
						Index of the bank you want to write the data
						- ::BANK0 Channel 0-3
						- ::BANK1 Channel 4-7
		@param	MemAddress
						Address of the target register
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status __DS160PR810_writeByte(DS160PR810* self, BankNumber bank_number, uint8_t MemAddress){
	uint8_t data[1];
	data[0] = self->reg.raw_data;
	if(HAL_I2C_Mem_Write(self->hi2c, (self->BANK0_I2C_ADDR + bank_number) << 1, MemAddress, I2C_MEMADD_SIZE_8BIT, data, 1, 1000)) return STATUS_TimeOut;
	return STATUS_OK;
}

/*!
    @brief  Read a byte of data from DS160PR810 and stores in the ds160pr810::_reg::raw_data
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	bank_number
						Index of the bank you want to read the data
						- ::BANK0 Channel 0-3
						- ::BANK1 Channel 4-7
		@param	MemAddress
						Address of the target register
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status __DS160PR810_readByte(DS160PR810* self, BankNumber bank_number, uint8_t MemAddress){
	uint8_t data;
	if(HAL_I2C_Mem_Read(self->hi2c, (self->BANK0_I2C_ADDR + bank_number) << 1, MemAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000)) return STATUS_TimeOut;
	self->reg.raw_data = data;
	return STATUS_OK;
}

/*!
    @brief  Configures the channel registers for the DS160PR810
						This function writes configuration values to a specific channel register
						or broadcasts the configuration to all channels depending on the `target` parameter.
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- **For single channel:**
							- ::BANK0_CHANNEL0 or ::CHANNEL0
							- ::BANK0_CHANNEL1 or ::CHANNEL1
							- ::BANK0_CHANNEL2 or ::CHANNEL2
							- ::BANK0_CHANNEL3 or ::CHANNEL3
							- ::BANK1_CHANNEL0 or ::CHANNEL4
							- ::BANK1_CHANNEL1 or ::CHANNEL5
							- ::BANK1_CHANNEL2 or ::CHANNEL6
							- ::BANK1_CHANNEL3 or ::CHANNEL7

						- **For two channels at once:**
							- ::BANK0_CHANNEL0_1 or ::CHANNEL0_1
							- ::BANK0_CHANNEL2_3 or ::CHANNEL2_3
							- ::BANK1_CHANNEL0_1 or ::CHANNEL4_5
							- ::BANK1_CHANNEL2_3 or ::CHANNEL6_7
							
						- **For all channels of a specific bank:**
							- ::BANK0_ALL_CHANNELS
							- ::BANK1_ALL_CHANNELS
						
						- **For all channels:**
							- ::ALL_BANKS
		
		@param	ChannelRegister
						The base address of the channel register to configure.
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status __DS160PR810_setChannelConfig(DS160PR810* self, ApplyTo target, uint8_t ChannelRegister){
	uint8_t register_address;	
	if(target == ALL_BANKS){ // Apply to all channels
		register_address = BROADCAST_WRITE_ALL + ChannelRegister;
		if(__DS160PR810_writeByte(self, BANK0, register_address)) return STATUS_TimeOut;
		if(__DS160PR810_writeByte(self, BANK1, register_address))	return STATUS_TimeOut;
		return STATUS_OK;
	}
	else{
		register_address = (target & 0xFE) + ChannelRegister;
		if(target & 0x01){ // Apply to BANK 1
			return __DS160PR810_writeByte(self, BANK1, register_address);
		}
		else{ // Apply to BANK 0
			return __DS160PR810_writeByte(self, BANK0, register_address);
		}
	}	
}

/*!
    @brief  Reads the configuration of a channel register of the DS160PR810
						This function reads configuration values of a specific channel register
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@param	ChannelRegister
						The base address of the channel register to read.
		@return	The status of initialization
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status __DS160PR810_getChannelConfig(DS160PR810* self, ApplyTo target, uint8_t ChannelRegister){
	uint8_t register_address;	
	if(target > 0x70){
		return STATUS_ArgumentError;
	}
	else{
		register_address = (target & 0xFE) + ChannelRegister;
		if(target & 0x01){ // Read from BANK 1
			return __DS160PR810_readByte(self, BANK1, register_address);
		}
		else{ // Read from BANK 0
			return __DS160PR810_readByte(self, BANK0, register_address);
		}
	}		
}

// Set Config --------------------------------

/*!
    @brief  Set the CTLE setting of the DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- **For single channel:**
							- ::BANK0_CHANNEL0 or ::CHANNEL0
							- ::BANK0_CHANNEL1 or ::CHANNEL1
							- ::BANK0_CHANNEL2 or ::CHANNEL2
							- ::BANK0_CHANNEL3 or ::CHANNEL3
							- ::BANK1_CHANNEL0 or ::CHANNEL4
							- ::BANK1_CHANNEL1 or ::CHANNEL5
							- ::BANK1_CHANNEL2 or ::CHANNEL6
							- ::BANK1_CHANNEL3 or ::CHANNEL7

						- **For two channels at once:**
							- ::BANK0_CHANNEL0_1 or ::CHANNEL0_1
							- ::BANK0_CHANNEL2_3 or ::CHANNEL2_3
							- ::BANK1_CHANNEL0_1 or ::CHANNEL4_5
							- ::BANK1_CHANNEL2_3 or ::CHANNEL6_7
							
						- **For all channels of a specific bank:**
							- ::BANK0_ALL_CHANNELS
							- ::BANK1_ALL_CHANNELS
						
						- **For all channels:**
							- ::ALL_BANKS
		@param	isByPassed
						Determines the status of first stage of CTLE
						- ::EQ_STAGE1_BYPASS_DISABLE
						- ::EQ_STAGE1_BYPASS_ENABLE
		@param	Stage1
						Determines the gain index of first stage of CTLE. This s value between 0 to 7.
		@param	Stage2
						Determines the gain index of second stage of CTLE. This s value between 0 to 7.
		@return	The status of config
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status DS160PR810_setCTLE(DS160PR810* self, ApplyTo target, EQStage1Bypass isByPassed, uint8_t Stage1, uint8_t Stage2){
	self->reg.raw_data = 0;
	self->reg.eq_ctrl.eq_en_bypass = isByPassed;
	self->reg.eq_ctrl.eq_bst1 = Stage1;
	self->reg.eq_ctrl.eq_bst2 = Stage2;
	
	return __DS160PR810_setChannelConfig(self, target, EQ_CTRL);
	
}

/*!
    @brief  Resets the RX detect state machine
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- **For single channel:**
							- ::BANK0_CHANNEL0 or ::CHANNEL0
							- ::BANK0_CHANNEL1 or ::CHANNEL1
							- ::BANK0_CHANNEL2 or ::CHANNEL2
							- ::BANK0_CHANNEL3 or ::CHANNEL3
							- ::BANK1_CHANNEL0 or ::CHANNEL4
							- ::BANK1_CHANNEL1 or ::CHANNEL5
							- ::BANK1_CHANNEL2 or ::CHANNEL6
							- ::BANK1_CHANNEL3 or ::CHANNEL7

						- **For two channels at once:**
							- ::BANK0_CHANNEL0_1 or ::CHANNEL0_1
							- ::BANK0_CHANNEL2_3 or ::CHANNEL2_3
							- ::BANK1_CHANNEL0_1 or ::CHANNEL4_5
							- ::BANK1_CHANNEL2_3 or ::CHANNEL6_7
							
						- **For all channels of a specific bank:**
							- ::BANK0_ALL_CHANNELS
							- ::BANK1_ALL_CHANNELS
						
						- **For all channels:**
							- ::ALL_BANKS
		@return	The status of config
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status DS160PR810_setResetRxDetectStateMachine(DS160PR810* self, ApplyTo target){
	self->reg.raw_data = 0;
	self->reg.rx_det_ctrl2.mr_rx_det_rst = 1;
	return __DS160PR810_setChannelConfig(self, target, RX_DET_CTRL2);
	
}

/*!
    @brief  Clear the reset flag of the RX detect state machine
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- **For single channel:**
							- ::BANK0_CHANNEL0 or ::CHANNEL0
							- ::BANK0_CHANNEL1 or ::CHANNEL1
							- ::BANK0_CHANNEL2 or ::CHANNEL2
							- ::BANK0_CHANNEL3 or ::CHANNEL3
							- ::BANK1_CHANNEL0 or ::CHANNEL4
							- ::BANK1_CHANNEL1 or ::CHANNEL5
							- ::BANK1_CHANNEL2 or ::CHANNEL6
							- ::BANK1_CHANNEL3 or ::CHANNEL7

						- **For two channels at once:**
							- ::BANK0_CHANNEL0_1 or ::CHANNEL0_1
							- ::BANK0_CHANNEL2_3 or ::CHANNEL2_3
							- ::BANK1_CHANNEL0_1 or ::CHANNEL4_5
							- ::BANK1_CHANNEL2_3 or ::CHANNEL6_7
							
						- **For all channels of a specific bank:**
							- ::BANK0_ALL_CHANNELS
							- ::BANK1_ALL_CHANNELS
						
						- **For all channels:**
							- ::ALL_BANKS
		@return	The status of config
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status DS160PR810_setClearRxDetectStateMachine(DS160PR810* self, ApplyTo target){
	self->reg.raw_data = 0;
	self->reg.rx_det_ctrl2.mr_rx_det_rst = 0;
	return __DS160PR810_setChannelConfig(self, target, RX_DET_CTRL2);

}

/*!
    @brief  Set the DC gain settings of the DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- **For single channel:**
							- ::BANK0_CHANNEL0 or ::CHANNEL0
							- ::BANK0_CHANNEL1 or ::CHANNEL1
							- ::BANK0_CHANNEL2 or ::CHANNEL2
							- ::BANK0_CHANNEL3 or ::CHANNEL3
							- ::BANK1_CHANNEL0 or ::CHANNEL4
							- ::BANK1_CHANNEL1 or ::CHANNEL5
							- ::BANK1_CHANNEL2 or ::CHANNEL6
							- ::BANK1_CHANNEL3 or ::CHANNEL7

						- **For two channels at once:**
							- ::BANK0_CHANNEL0_1 or ::CHANNEL0_1
							- ::BANK0_CHANNEL2_3 or ::CHANNEL2_3
							- ::BANK1_CHANNEL0_1 or ::CHANNEL4_5
							- ::BANK1_CHANNEL2_3 or ::CHANNEL6_7
							
						- **For all channels of a specific bank:**
							- ::BANK0_ALL_CHANNELS
							- ::BANK1_ALL_CHANNELS
						
						- **For all channels:**
							- ::ALL_BANKS
		@param	tx_vod
						Specifies the VOD
						- ::TX_VOD_0db 0.0db
						- ::TX_VOD_m1db6 -1.6db
						- ::TX_VOD_m3db5 -3.5db
						- ::TX_VOD_m6db -6.0db
		@param	dc_gain
						Specifies the DC gain
						- ::EQ_DC_GAIN_0db 0.0db
						- ::EQ_DC_GAIN_3db5 3.5db
		@return	The status of config
		@retval ::STATUS_OK in case of success
		@retval ::STATUS_TimeOut in case of failure
*/
Status DS160PR810_setDCGain(DS160PR810* self, ApplyTo target, TxVOD tx_vod, EQDCGain dc_gain){
	self->reg.raw_data = 0;
	self->reg.gain_ctrl.drv_sel_vod = tx_vod;
	self->reg.gain_ctrl.eq_hi_gain = dc_gain;
	return __DS160PR810_setChannelConfig(self, target, GAIN_CTRL);
	
}

// Get Config --------------------------------

/*!
    @brief  Reads the DS160PR810 CTLE first stage status.
						This function reads status of the first stage of CTLE.
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The status of first stage
		@retval ::EQ_STAGE1_BYPASS_DISABLE Stage1 is active
		@retval ::EQ_STAGE1_BYPASS_ENABLE Stage1 is bypassed
*/
EQStage1Bypass DS160PR810_getCTLE_EQStage1Bypass(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, EQ_CTRL)) return -1;
	return self->reg.eq_ctrl.eq_en_bypass ? EQ_STAGE1_BYPASS_ENABLE : EQ_STAGE1_BYPASS_DISABLE;
}

/*!
    @brief  Reads the DS160PR810 CTLE first stage gain index.
						This function reads gain index of the first stage of CTLE.
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The gain index of first stage
		@retval From 0 to 7
*/
uint8_t DS160PR810_getCTLE_Stage1(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, EQ_CTRL)) return -1;
	return self->reg.eq_ctrl.eq_bst1;
}

/*!
    @brief  Reads the DS160PR810 CTLE second stage gain index.
						This function reads gain index of the second stage of CTLE.
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The gain index of second stage
		@retval From 0 to 7
*/
uint8_t DS160PR810_getCTLE_Stage2(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, EQ_CTRL)) return -1;
	return self->reg.eq_ctrl.eq_bst2;	
}

/*!
    @brief  Reads the TX VOD of the DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The TX VOD
		@retval	::TX_VOD_0db 0.0db
		@retval	::TX_VOD_m1db6 -1.6db
		@retval	::TX_VOD_m3db5 -3.5db
		@retval	::TX_VOD_m6db -6.0db
*/
TxVOD DS160PR810_getDCGain_TxVOD(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, GAIN_CTRL)) return -1;
	return (TxVOD)self->reg.gain_ctrl.drv_sel_vod;	
}

/*!
    @brief  Reads the DC gain of the DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The DC gain
		@retval	::EQ_DC_GAIN_0db 0.0db
		@retval	::EQ_DC_GAIN_3db5 3.5db
*/
EQDCGain DS160PR810_getDCGain_EQDCGain(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, GAIN_CTRL)) return -1;
	return (EQDCGain)self->reg.gain_ctrl.eq_hi_gain;	
}

/*!
    @brief  Reads the RX detect status of the DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	target Specifies the target.
						- ::BANK0_CHANNEL0 or ::CHANNEL0
						- ::BANK0_CHANNEL1 or ::CHANNEL1
						- ::BANK0_CHANNEL2 or ::CHANNEL2
						- ::BANK0_CHANNEL3 or ::CHANNEL3
						- ::BANK1_CHANNEL0 or ::CHANNEL4
						- ::BANK1_CHANNEL1 or ::CHANNEL5
						- ::BANK1_CHANNEL2 or ::CHANNEL6
						- ::BANK1_CHANNEL3 or ::CHANNEL7

		@return	The RX detect status
		@retval	::RX_DET_NONE
		@retval	::RX_DET_N
		@retval	::RX_DET_P
		@retval	::RX_DET_BOTH
*/
RxDetectStatus	DS160PR810_getRxDetectStatus(DS160PR810* self, ApplyTo target){
	if(__DS160PR810_getChannelConfig(self, target, GAIN_CTRL)) return -1;
	return (RxDetectStatus)((self->reg.rx_det_sts.rx_det_comp_n << 1) + self->reg.rx_det_sts.rx_det_comp_p);
}

// Get Other Data ----------------------------

/*!
    @brief  Reads device ID0 of the specific bank of th DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	bank_number The bank index
						- ::BANK0
						- ::BANK1
		@return	The device ID0
*/
uint8_t DS160PR810_getDevID0(DS160PR810* self, BankNumber bank_number){
	if(__DS160PR810_readByte(self, bank_number, DEVICE_ID0_REGISTER)) return 0;
	return self->reg.device_id0_register.device_id0;
}

/*!
    @brief  Reads device ID1 of the specific bank of th DS160PR810
    @param  self
            A pointer to the ds160pr810 object (struct)
		@param	bank_number The bank index
						- ::BANK0
						- ::BANK1
		@return	The device ID1
*/
uint8_t	DS160PR810_getDevID1(DS160PR810* self, BankNumber bank_number){
	if(__DS160PR810_readByte(self, bank_number, DEVICE_ID1_REGISTER)) return 0;
	return self->reg.device_id1_register.device_id1;
}

uint8_t	DS160PR810_getEELoadComplete(DS160PR810* self, BankNumber bank_number){
	if(__DS160PR810_readByte(self, bank_number, EEPROM_STATUS_REGISTER)) return 0;
	return self->reg.eeprom_status_register.eecfg_cmplt;
}

uint8_t	DS160PR810_getEELoadFail(DS160PR810* self, BankNumber bank_number){
	if(__DS160PR810_readByte(self, bank_number, EEPROM_STATUS_REGISTER)) return 0;
	return self->reg.eeprom_status_register.eecfg_fail;
}

uint8_t	DS160PR810_getEEAtemptCount(DS160PR810* self, BankNumber bank_number){
	if(__DS160PR810_readByte(self, bank_number, EEPROM_STATUS_REGISTER)) return 0;
	return self->reg.eeprom_status_register.eecfg_atmpt;
}
