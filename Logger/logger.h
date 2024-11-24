#ifndef __LOGGER_H_
#define __LOGGER_H_

#include "main.h"
#include "ds160pr810.h"
#include "debug.h"

// Dump EQ ---------------------
void log_EQ_Channel(DS160PR810* self, ApplyTo channel);
void log_EQ_Bank(DS160PR810* self, BankNumber bank_number);
void log_EQ_All(DS160PR810* self);

// Dump Gain -------------------
void log_Gain_Channel(DS160PR810* self, ApplyTo channel);
void log_Gain_Bank(DS160PR810* self, BankNumber bank_number);
void log_Gain_All(DS160PR810* self);

// Dump RX Detect Status -------
void log_RxDetectStatus_Channel(DS160PR810* self, ApplyTo channel);
void log_RxDetectStatus_Bank(DS160PR810* self, BankNumber bank_number);
void log_RxDetectStatus_All(DS160PR810* self);

// Dump Whole ------------------
void log_Channel(DS160PR810* self, ApplyTo channel);
void log_Bank(DS160PR810* self, BankNumber bank_number);
void log_All(DS160PR810* self);
void log_All2(DS160PR810* self);

#endif
