; Copyright 2019 by Sven Fabricius, see LICENSE.txt
;
; @file SSD1289_DrawChar.asm
;
; @brief draws the char data at the specified position
;
; @author Sven Fabricius
; Contact: Sven.Fabricius@livediesel.de

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

RAM_RW			.set  0x0022

;############################################################################################
; extern void SSD1289_DrawChar(SSD1289_Config * config, uint16_t color, uint8_t width, uint8_t height, const uint8_t * data);
	.global SSD1289_DrawChar
SSD1289_DrawChar: .asmfunc

config		.set r0
widthCnt	.set r0
color		.set r1
width		.set r2
height		.set r3
data		.set r4
dataOut		.set r5
ctrlOut		.set r6
w0			.set r7
wr			.set r8
rd			.set r9
buffer		.set r10

	PUSH	{r4-r10, lr} ; 8 regs are pushed, SP is decremented by 8*4 = 32 bytes

	LDR		ctrlOut,	[config, #CTRL_OUT_OFF]	; load pointer to Ctrl out
	LDR		dataOut,	[config, #DATA_OUT_OFF]	; load pointer to Data out
	LDR		data,		[sp, #(0+32)] 			; Data from stack

	LDRH	w0,			[ctrlOut]				; load Ctrl data

	LDRH	buffer,		[config, #RS_PIN_OFF]	; load RS pin
	BIC		wr,			w0,			buffer		; disable RS pin
	LDRH	buffer, 	[config, #WR_PIN_OFF]	; load WR pin
	BIC		wr,			wr,			buffer		; disable WR pin

	STRH	wr,			[ctrlOut]				; set Ctrl out ; WR Low, RS Low
	MOV		buffer,		#RAM_RW
	STRH	buffer,		[dataOut]				; set Data out
	STRH	w0,			[ctrlOut]				; set Ctrl out ; WR Low, RS Low

	LDRH	buffer, 	[config, #RS_PIN_OFF]	; load RS pin
	ORR		wr,			wr,			buffer		; enable RS pin

	LDRH	buffer,		[config, #RD_PIN_OFF]	; load RD pin
	BIC		rd,			w0,			buffer		; disable RD pin

	STRH 	color,		[dataOut]				; set Data out

l_setWidth:										; from now config (R0) will be used as width counter
	MOV		widthCnt,	#0						; set width counter
	SUBS	height,		height,		#1			; decrement height
	BEQ		l_endAsm

l_loadData:
	LDRB	buffer,		[data],		#1			; load data and inc pointer

l_shiftData:
	LSRS	buffer,		buffer,		#1			; right shift use carry
	ITE		CS									; thumb carry set execution
	STRHCS 	wr, 		[ctrlOut]				; set Ctrl out ; WR Low, RS High ; write color
	STRHCC 	rd, 		[ctrlOut]				; set Ctrl out ; RD Low, RS High ; dummy read to hold transparency

	STRH 	w0,			[ctrlOut]				; set Ctrl out ; RD High, RS High
	ADD		widthCnt,	widthCnt,	#1			; inc width count
	CMP		widthCnt,	width					; compare width and maxwidth
	BEQ		l_setWidth							; reset width and load next data
	TST		widthCnt,	#0x07					; test if all shifted out
	BEQ		l_loadData							; load next data
	B		l_shiftData							; shift out next data

l_endAsm:
	POP		{r4-r10, lr}
	BX		lr
	.endasmfunc
