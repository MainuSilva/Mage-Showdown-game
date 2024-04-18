#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id;
unsigned int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if(freq > TIMER_FREQ){
        printf("Frequency is too high\n");
        return 1;
    }

    if(freq < TIMER_MIN_FREQ) {
        printf("Frequency is too low\n");
        return 1;
    }

    uint8_t st;
    if(timer_get_conf(timer, &st) != 0) {
        printf("Unable to retrieve the timer's configuration\n");
        return 1;
    }

    uint8_t control_word = TIMER_RB_SEL(timer) | TIMER_LSB_MSB | (st & (BIT(3) | BIT(2) | BIT(1) | BIT(0)));
    uint16_t div = TIMER_FREQ / freq;

    if(sys_outb(TIMER_CTRL, control_word)) {
        printf("Unable to send the control word\n");
        return 1;
    }

    uint8_t div_lsb, div_msb;

    util_get_LSB(div, &div_lsb);
    util_get_MSB(div, &div_msb);

    if(sys_outb(TIMER_0 + timer, div_lsb)) {
        printf("Unable to send lsb\n");
        return 1;
    }

    if(sys_outb(TIMER_0 + timer, div_msb)) {
        printf("Unable to send msb\n");
        return 1;
    }

    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    hook_id = TIMER0_IRQ;
    *bit_no = BIT(hook_id);

    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
        return 1;

    return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) {return 1;}

  return 0;
}

void (timer_int_handler)() {
  counter++;
  return;
}
