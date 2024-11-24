#include "logger.h"

ApplyTo Channels[8] = {CHANNEL0, CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4, CHANNEL5, CHANNEL6, CHANNEL7};

// Dump EQ ---------------------
void log_EQ_All(DS160PR810* self){
	log_EQ_Bank(self, BANK0);
	log_EQ_Bank(self, BANK1);
}

void log_EQ_Bank(DS160PR810* self, BankNumber bank_number){
	for(int32_t i = 0; i<4; i++){
		log_EQ_Channel(self, Channels[i + ((uint8_t)bank_number * 4)]);
	}
}

void log_EQ_Channel(DS160PR810* self, ApplyTo channel){
	DEBUG("   - EQ Settings:\n\r");
	DEBUG("     - Stage1 Status: %s \n\r", DS160PR810_getCTLE_EQStage1Bypass(self, channel) ? "BY PASSED" : "ACTIVE");
	DEBUG("     -     Stage1 EQ: %d \n\r", DS160PR810_getCTLE_Stage1(self, channel));
	DEBUG("     -     Stage2 EQ: %d \n\r", DS160PR810_getCTLE_Stage2(self, channel));	
}

// Dump Gain -------------------
void log_Gain_Channel(DS160PR810* self, ApplyTo channel){
	DEBUG("   - Gain Settings:\n\r");
	DEBUG("     - CTLE DC Gain: %s \n\r", DS160PR810_getDCGain_EQDCGain(self, channel) ? "3.5 db" : "0 db");
	TxVOD val = DS160PR810_getDCGain_TxVOD(self, channel);
	switch (val)
  {
  	case TX_VOD_0db:
			DEBUG("     -       Tx VOD: 0 db \n\r");
  		break;
  	case TX_VOD_m3db5:
			DEBUG("     -       Tx VOD: -3.5 db \n\r");
  		break;
  	case TX_VOD_m1db6:
			DEBUG("     -       Tx VOD: -1.6 db \n\r");
  		break;
		case TX_VOD_m6db:
			DEBUG("     -       Tx VOD: -6.0 db \n\r");
  		break;
  }
}

void log_Gain_Bank(DS160PR810* self, BankNumber bank_number){
	for(int32_t i = 0; i<4; i++){
		log_Gain_Channel(self, Channels[i + ((uint8_t)bank_number * 4)]);
	}
}

void log_Gain_All(DS160PR810* self){
	log_Gain_Bank(self, BANK0);
	log_Gain_Bank(self, BANK1);
}

// Dump RX Detect Status -------
void log_RxDetectStatus_Channel(DS160PR810* self, ApplyTo channel){
	RxDetectStatus val = DS160PR810_getRxDetectStatus(self, channel);
	switch (val)
  {
  	case RX_DET_NONE:
			DEBUG("   - RX Detect Status: NONE\n\r");
  		break;
  	case RX_DET_P:
			DEBUG("   - RX Detect Status: Positive\n\r");
  		break;
  	case RX_DET_N:
			DEBUG("   - RX Detect Status: Negative\n\r");
  		break;
		case RX_DET_BOTH:
			DEBUG("   - RX Detect Status: Both\n\r");
  		break;
  }
}

void log_RxDetectStatus_Bank(DS160PR810* self, BankNumber bank_number){
	for(int32_t i = 0; i<4; i++){
		log_RxDetectStatus_Channel(self, Channels[i + ((uint8_t)bank_number * 4)]);
	}
}

void log_RxDetectStatus_All(DS160PR810* self){
	log_RxDetectStatus_Bank(self, BANK0);
	log_RxDetectStatus_Bank(self, BANK1);
}

// Dump Whole ------------------
void dump_channel(DS160PR810* self, ApplyTo channel){
	DEBUG("- Channel[%d]\n\r", ((channel & 0x01) << 2) + (channel >> 5));
	log_EQ_Channel(self, channel);
	log_Gain_Channel(self, channel);
	log_RxDetectStatus_Channel(self, channel);
}

void log_Bank(DS160PR810* self, BankNumber bank_number){
	for(int32_t i = 0; i<4; i++){
		dump_channel(self, Channels[i + ((uint8_t)bank_number * 4)]);
	}
}

void log_All(DS160PR810* self){
	log_Bank(self, BANK0);
	log_Bank(self, BANK1);
}

void log_All2(DS160PR810* self){
	DEBUG("---------------------------------------------------------------------------------\n\r");
	DEBUG("| Ch index | Stage 1 Stat | Stage 1 | Stage 2 | DC Gain |  Tx VOD  | RX  Detect |\n\r");
	DEBUG("---------------------------------------------------------------------------------\n\r");
	for(uint8_t i=0; i<8; i++){
		TxVOD tx_vod = DS160PR810_getDCGain_TxVOD(self, Channels[i]);
		RxDetectStatus rx_detect_stat = DS160PR810_getRxDetectStatus(self, Channels[i]);
		DEBUG("|     %d    |   %s   |    %d    |    %d    | %s | %s |    %s    |\n\r",
					i,
					DS160PR810_getCTLE_EQStage1Bypass(self, Channels[i]) ? "BYPASSED" : " ACTIVE ",
					DS160PR810_getCTLE_Stage1(self, Channels[i]),
					DS160PR810_getCTLE_Stage2(self, Channels[i]),
					DS160PR810_getDCGain_EQDCGain(self, Channels[i]) ? "3.5  db" : " 0   db",
					tx_vod == TX_VOD_0db ? "  0   db" : (tx_vod == TX_VOD_m1db6 ? "-1.6  db" : (tx_vod == TX_VOD_m3db5 ? "-3.5  db" : "-6.5  db")),
		rx_detect_stat == RX_DET_NONE ? "NONE" : (rx_detect_stat == RX_DET_N ? " N  " : (rx_detect_stat == RX_DET_P ? " P  " : "BOTH"))
		);
	}
	DEBUG("---------------------------------------------------------------------------------\n\r");
}
