#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"

#define PIN21_MASK           (1u << 21)

void blue_init(void);
void blue_toggle(void);
void PIT_init(void);
void PIT0_IRQHandler(void);

int main(void)
{
    blue_init();
    PIT_init();
    while (true) {
    }
    return 0;
}

void blue_init(void) {
    /* Enable the clock to PORT B */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    /* Select the GPIO function (Alternative 1) for pin 21 of PORT B */
    PORTB_PCR21 &= ~PORT_PCR_MUX_MASK;
    PORTB_PCR21 |= (1u << PORT_PCR_MUX_SHIFT);

    /* Set the data direction for pin 21 of PORT B to output */
    GPIOB_PDDR |= PIN21_MASK;

    /* LED off initially */
    GPIOB_PDOR |= PIN21_MASK;
}

void blue_toggle(void) {
    GPIOB_PDOR ^= PIN21_MASK; 
}

void PIT_init(void) {
    SIM_SCGC6 |= (1u << 23);
    PIT_MCR_REG(PIT) = 0u;
    PIT_LDVAL_REG(PIT, 0) = 30000000;
    PIT_TCTRL_REG(PIT, 0) |= PIT_TCTRL_TIE_MASK;
    PIT_TCTRL_REG(PIT, 0) |= PIT_TCTRL_TEN_MASK;
    NVIC_EnableIRQ(PIT0_IRQn);
}

void PIT0_IRQHandler(void) {
    blue_toggle();
    PIT_TFLG_REG(PIT,0) |= PIT_TFLG_TIF_MASK;
}
