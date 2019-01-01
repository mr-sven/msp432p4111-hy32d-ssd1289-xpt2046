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
; extern void SSD1289_Fill(SSD1289_Config * config, uint16_t color, uint32_t count32);
	.global SSD1289_Fill
SSD1289_Fill: .asmfunc

Config		.set r0
Color		.set r1
Count		.set r2
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
	mov Buffer, #RAM_RW
	strh Buffer, [DataOut]     ; set Data out
	strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS Low

	ldrh Buffer, [Config, #RS_PIN_OFF] ; load RS pin
	orr W0, W0, Buffer         ; enable RS pin

	ands Buffer, Count, #0x0007 ; Test if lower bits of count is set
	beq Fast

Slow_L: ; Slow data loop
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		subs Buffer, Buffer, #1
		bne Slow_L ; continue while slow data

Fast: ; Fast data
	lsrs Count, Count, #3 ; count >> 3
	beq EndFillASM ; if count = 0, goto end

Fast_L: ; fast data loop
		; ----------------- Data 0 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 1 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 2 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 3 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 4 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 5 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 6 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		; ----------------- Data 7 -----------------
		strh W0, [CtrlOut]         ; set Ctrl out ; WR Low, RS High
		strh Color, [DataOut]      ; set Data out
		strh W1, [CtrlOut]         ; set Ctrl out ; WR High, RS High

		subs Count, Count, #1
		bne Fast_L ; continue while fast data

EndFillASM:
	pop {r4-r7,lr}
	bx lr
	.endasmfunc
