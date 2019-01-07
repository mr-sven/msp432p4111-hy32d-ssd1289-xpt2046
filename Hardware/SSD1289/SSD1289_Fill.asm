; Copyright 2019 by Sven Fabricius, see LICENSE.txt
;
; @file SSD1289_Fill.asm
;
; @brief Fill the area specified by setBounds with color
;
; @author Sven Fabricius
; Contact: Sven.Fabricius@livediesel.de

;typedef struct _SSD1289_config
;{
;	volatile uint16_t * dataOut; + 0
;	volatile uint16_t * dataIn; + 4
;	volatile uint16_t * dataDir; + 8
;	volatile uint16_t * ctrlOut; + 12
;	uint16_t csPin; + 16
;	uint16_t rsPin; + 18
;	uint16_t rdPin; + 20
;	uint16_t wrPin; + 22
;} SSD1289_config;

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
; extern void SSD1289_Fill(SSD1289_config * config, uint16_t color, uint32_t count32);
	.global SSD1289_Fill
SSD1289_Fill: .asmfunc

config		.set r0
color		.set r1
count		.set r2
ctrlOut		.set r3
dataOut		.set r4
w0			.set r5
w1			.set r6
buffer		.set r7

	PUSH 	{r4-r7,lr} ; 5 regs are pushed, SP is decremented by 5*4 = 20 bytes

	LDR		ctrlOut,	[config, #CTRL_OUT_OFF]	; load pointer to Ctrl out
	LDR		dataOut,	[config, #DATA_OUT_OFF]	; load pointer to Data out

	LDRH	w1,			[ctrlOut]				; load Ctrl data

	LDRH	buffer,		[config, #WR_PIN_OFF]	; load WR pin
	BIC		w0,			w1,			buffer		; disable WR pin
	LDRH	buffer,		[config, #RS_PIN_OFF]	; load RS pin
	BIC		w0,			w0,			buffer		; disable RS pin

	STRH	w0,			[ctrlOut]				; set Ctrl out ; WR Low, RS Low
	MOV		buffer,		#RAM_RW
	STRH	buffer,		[dataOut]				; set Data out
	STRH	w1,			[ctrlOut]				; set Ctrl out ; WR Low, RS Low

	LDRH	buffer,		[config, #RS_PIN_OFF]	; load RS pin
	ORR		w0,			w0,			buffer		; enable RS pin

	ANDS	buffer,		count,		#0x0007		; Test if lower bits of count is set
	BEQ		l_fast

l_slowLoop: ; Slow data loop
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		SUBS	buffer,	buffer,		#1
		BNE		l_slowLoop						; continue while slow data

l_fast: ; Fast data
	LSRS	count,		count,		#3			; count >> 3
	BEQ		l_endAsm							; if count = 0, goto end

l_fastLoop: ; fast data loop
		; ----------------- Data 0 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 1 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 2 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 3 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 4 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 5 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 6 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		; ----------------- Data 7 -----------------
		STRH	w0,		[ctrlOut]				; set Ctrl out ; WR Low, RS High
		STRH	color,	[dataOut]				; set Data out
		STRH	w1,		[ctrlOut]				; set Ctrl out ; WR High, RS High

		SUBS	count,	count,		#1
		BNE		l_fastLoop						; continue while fast data

l_endAsm:
	POP		{r4-r7,lr}
	BX		lr
	.endasmfunc
