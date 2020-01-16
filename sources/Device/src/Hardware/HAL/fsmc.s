PERIPH_BASE     EQU 0x40000000
AHB1PERIPH_BASE EQU (PERIPH_BASE + 0x00020000)
    
SHIFT_IDR       EQU 16
SHIFT_BSRR      EQU 24

GPIOA_BASE      EQU (AHB1PERIPH_BASE + 0x0000)
GPIOA           EQU GPIOA_BASE

GPIOD_BASE      EQU (AHB1PERIPH_BASE + 0x0C00)
GPIOD           EQU GPIOD_BASE

GPIOG_BASE      EQU (AHB1PERIPH_BASE + 0x1800)
GPIOG           EQU GPIOG_BASE

GPIO_PIN_5      EQU (1 << 5)
GPIO_PIN_7      EQU (1 << 7)
GPIO_PIN_12     EQU (1 << 12)
    
              AREA |.text|, CODE, READONLY
              
              EXPORT CycleSend [WEAK]
CycleSend PROC

    PUSH {R0-R3, lr}
    
    ; R2 - GPIOA->IDR
    
;GPIOD->BSRR = GPIO_PIN_5 << 16
    MOV R0, #GPIO_PIN_5 << 16
    LDR R1, =GPIOD + SHIFT_BSRR
    STR R0, [R1]
    
;while(GPIOA->IDR & GPIO_PIN_7)
cycle1
    LDR R2, =GPIOA + SHIFT_IDR
    LDR R3, [R2]
    ANDS R3, R3, #GPIO_PIN_7
    BNE   cycle1
    
;GPIOG->BSRR = GPIO_PIN_12 << 16
    MOV R0, #GPIO_PIN_12 << 16
    LDR R1, =GPIOG + SHIFT_BSRR
    STR R0, [R1]

;while((GPIOA->IDR & GPIO_PIN_7) == 0) {}
cycle2
    LDR R3, [R2]
    ANDS R3, R3, #GPIO_PIN_7
    BEQ cycle2
    
    POP {R0-R3, pc}
    
    
                  
;    IMPORT mode

; mode = Mode::PanelWrite
;    MOVS R1, #2
;    LDR  R3, |mode|
;    STRB R1, [R3]

; GPIOD->MODER &= 0xfffff0ff
;    LDR R1, =GPIOD
;    BIC R2, R1, #0xf00
;    STR R2, [R1]

; GPIOD->MODER |= 0x500
;    ORR R2, R1, #0x500
;    STR R2, [R1]
    
; GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4)
;    MOVS R3, #GPIO_PIN_4 + GPIO_PIN_5
;    LDR  R1, =GPIOD_BSRR
;    STR  R3, [R1]

              ENDP
              END
