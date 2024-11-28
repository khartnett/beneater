PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003

E  = %10000000
RW = %01000000
RS = %00100000

  .org $8000
; not sure what's above here
reset:
  ldx #$ff
  txs

  lda #%11111111 ; Set all pins on port B to output
  sta DDRB
  lda #%00000000 ; Set all pins on port A to input (this is 10111111 in video)
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

  lda #1
  sta PORTA

rx_wait:
  bit PORTA ; Put PortA.6 into V flag
  bvs rx_wait ; loop if no start bit
  jsr half_bit_delay

  ldx #8
read_bit:
  bit PORTA ; Put PortA.6 into V flag
  bvs recv_1
  clc  ; we read a 0, put a 0 into the C flag
  jmp rx_done
recv_1:
  sec  ; We read a 1, put a 1 into the C flag
rx_done:
  ror ; Rotate a registaer right, putting C flag as new MSB
  dex
  bne read_bit

  ; All 8 bits are now in A
  jsr print_char

  jsr bit_delay
  jsr rx_wait


bit_delay:
  phx
  ldx #13
bit_delay_1:
  dex
  bne bit_delay_1
  plx
  rts


half_bit_delay:
  phx
  ldx #6
half_bit_delay_1:
  dex
  bne half_bit_delay_1
  plx
  rts

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
