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

Config		.set r0
Command		.set r1
Data		.set r2
CtrlOut		.set r3
DataOut		.set r4
W0			.set r5
W1			.set r6
Buffer		.set r7

	push {r4-r7,lr} ; 5 regs are pushed, SP is decremented by 5*4 = 20 bytes

	ldr CtrlOut, [Config, #CTRL_OUT_OFF] ; load pointer to Ctrl out
	ldr DataOut, [Config, #DATA_OUT_OFF]  ; load pointer to Data out

	ldrh W1, [CtrlOut]         ; load Ctrl data

	ldrh Buffer, [Config, #WR_PIN_OFF] ; load WR pin
	bic W0, W1, Buffer         ; disable WR pin
	ldrh Buffer, [Config, #RS_PIN_OFF] ; load RS pin
	bic W0, W0, Buffer         ; disable RS pin

	strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS Low
	strh Command, [DataOut]    ; set Data out
	strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS Low

	orr W0, W0, Buffer         ; enable RS pin

	strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
	strh Data, [DataOut]       ; set Data out
	strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

	pop {r4-r7,lr}
	bx lr
	.endasmfunc
