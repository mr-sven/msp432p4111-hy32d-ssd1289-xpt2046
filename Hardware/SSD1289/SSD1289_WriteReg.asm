;typedef struct _SSD1289_Config
;{
;	volatile uint16_t * dataOut; + 0
;	volatile uint16_t * dataIn; + 4
;	volatile uint16_t * dataDir; + 8
;	volatile uint16_t * ctrlOut; + 12
;	uint16_t csPin; + 16
;	uint16_t rsPin; + 18
;	uint16_t rdPin; + 20
;	uint16_t wrPin; + 22
;} SSD1289_Config;

DATA_OUT_OFF	.set  0x00
DATA_IN_OFF		.set  0x04
DATA_DIR_OFF	.set  0x08
CTRL_OUT_OFF	.set  0x0C
CS_PIN_OFF		.set  0x10
RS_PIN_OFF		.set  0x12
RD_PIN_OFF		.set  0x14
WR_PIN_OFF		.set  0x16

;############################################################################################
; extern void SSD1289_WriteReg(SSD1289_Config * config, uint16_t command, uint16_t data);
	.global SSD1289_WriteReg
SSD1289_WriteReg: .asmfunc

config		.set r0
command		.set r1
data		.set r2
ctrlOut		.set r3
dataOut		.set r4
w0			.set r5
w1			.set r6
buffer		.set r7

	PUSH	{r4-r7,lr} ; 5 regs are pushed, SP is decremented by 5*4 = 20 bytes

	LDR		ctrlOut,	[config, #CTRL_OUT_OFF]	; load pointer to Ctrl out
	LDR		dataOut,	[config, #DATA_OUT_OFF]	; load pointer to Data out

	LDRH	w1,			[ctrlOut]				; load Ctrl data

	LDRH	buffer,		[config, #WR_PIN_OFF]	; load WR pin
	BIC		w0,			w1,			buffer		; disable WR pin
	LDRH	buffer,		[config, #RS_PIN_OFF]	; load RS pin
	BIC		w0,			w0,			buffer		; disable RS pin

	STRH	w0,			[ctrlOut]				; set Ctrl out ; WR Low, RS Low
	STRH	command,	[dataOut]				; set Data out
	STRH	w1,			[ctrlOut]				; set Ctrl out ; WR High, RS High

	ORR		w0,			w0,			buffer		; enable RS pin

	STRH	w0,			[ctrlOut]				; set Ctrl out ; WR Low, RS High
	STRH	data,		[dataOut]				; set Data out
	STRH	w1,			[ctrlOut]				; set Ctrl out ; WR High, RS High

	POP		{r4-r7,lr}
	BX		lr
	.endasmfunc
