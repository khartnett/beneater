PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003
T1CL = $0004
T1CH = $6005
ACR = $600B
IER = $600E

E  = %10000000
RW = %01000000
RS = %00100000

ACIA_DATA = $5000
ACIA_STATUS = $5001
ACIA_CMD = $5002
ACIA_CTRL = $5003

  .org $8000

reset:
  ldx #$ff
  txs

  lda #%11111111 ; Set all pins on port B to output
  sta DDRB
  lda #%10111111 ; Set all pins on port A to input (this is 10111111 in video)
  sta DDRA

  jsr lcd_init
  lda #%00101000 ; Set 4-bit mode; 2-line display; 5x8 font
  jsr lcd_instruction
  lda #%00001110 ; Display on; cursor on; blink off
  jsr lcd_instruction
  lda #%00000110 ; Increment and shift cursor; don't shift display
  jsr lcd_instruction
  lda #%00000001 ; Clear display
  jsr lcd_instruction

  lda #$00
  sta ACIA_STATUS ; soft reset (value does not matter)

  lda #$1f ; N-8-1, 19200 baud
  sta ACIA_CTRL

  lda #$0b ; no parity, no echo, no interrupts
  sta ACIA_CMD

rx_wait:
  lda ACIA_STATUS
  and #$08 ; check rx buffer status flag
  beq rx_wait ; loop if rx buffer empty

  lda ACIA_DATA
  jsr print_char
  jmp rx_wait

lcd_wait:
  pha
  lda #%11110000  ; LCD data is input
  sta DDRB

; not sure what's above here


lcd_instruction:
  jsr lcd_wait
  pha
  lsr
  lsr
  lsr
  lsr            ; Send high 4 bits
  sta PORTB
  ora #E         ; Set E bit to send instruction
  sta PORTB
  eor #E         ; Clear E bit
  sta PORTB
  pla
  and #%00001111 ; Send low 4 bits
  sta PORTB
  ora #E         ; Set E bit to send instruction
  sta PORTB
  eor #E         ; Clear E bit
  sta PORTB
  rts

print_char:
  jsr lcd_wait
  pha
  lsr
  lsr
  lsr
  lsr             ; Send high 4 bits
  ora #RS         ; Set RS
  sta PORTB
  ora #E          ; Set E bit to send instruction
  sta PORTB
  eor #E          ; Clear E bit
  sta PORTB
  pla
  and #%00001111  ; Send low 4 bits
  ora #RS         ; Set RS
  sta PORTB
  ora #E          ; Set E bit to send instruction
  sta PORTB
  eor #E          ; Clear E bit
  sta PORTB
  rts

