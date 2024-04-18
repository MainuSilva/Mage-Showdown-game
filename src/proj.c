// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "devices/timer/i8254.h"
#include "devices/video_card/graphics_card.h"
#include "devices/keyboard_mouse/i8042.h"
#include "devices/keyboard_mouse/kbd.h"
#include "devices/keyboard_mouse/mouse.h"
#include "model/game/Level.h"
#include "model/entities/player.h"
#include "model/menu/menu.h"
#include "model/game/game.h"

bool KbdReadSecond = false;
bool MouseReadSecond = false;
bool MouseReadThird = false;

extern unsigned int counter;
extern uint8_t scancode;
struct packet pack;
struct mouse_ev mouse_event;

uint32_t irq_set_timer = BIT(TIMER0_IRQ);
uint32_t irq_set_kbd = BIT(KBD_IRQ);
uint32_t irq_set_mouse = BIT(MOUSE_IRQ);
uint32_t irq_set_rtc = BIT(RTC_IRQ);
uint32_t irq_set_sp = BIT(SP_COM1_IRQ);

uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no, rtc_bit_no, sp_bit_no;

uint8_t kbd_bytes[2];

uint8_t ms_bytes[3];

GameState gameState = MENU;

extern DateTime * dateTime;
extern Level * level;

int main(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int setup() {

    if (set_graphics_card_mode(0x118) != 0) {
        vg_exit();
        return 1;
    }

    LoadMainMenu(true);

    //Subscribes timer interrupts
    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("Unable to subscribe to the timer's interrupts\n");
        return 1;
    }

    //Subscribes keyboard interrupts
    if (kbd_subscribe_int(&kbd_bit_no) != 0) {
        printf("Unable to subscribe to the keyboard's interrupts\n");
        return 1;
    }

    if (rtc_subscribe_int(&rtc_bit_no) != 0) {
        printf("Unable to subscribe to the RTC's interrupts\n");
        return 1;
    }

    if (mouse_enable_data_report() != 0) {
        printf("Unable to enable mouse data report\n");
        return 1;
    }

    if (mouse_subscribe_int(&mouse_bit_no) != 0) {
        printf("Unable to subscribe to the mouse's interrupts\n");
        return 1;
    }


    if (sp_subscribe_int(&sp_bit_no) != 0) {
        printf("Unable to subscribe to the Serial Port's interrupts\n");
        return 1;
    }

    LoadMultiPlayerXPM();
    dateTime = LoadDateTime();
    LoadRtc();
    addToTransmitQueue(VM_CONNECTED);

    return 0;
}

int leave() {
    addToTransmitQueue(VM_DISCONNECTED);

    //UNSUBSCRIBES
    if (rtc_unsubscribe_int() != 0) {
        printf("Unable to unsubscribe to the RTC's interrupts\n");
        return 1;
    }

    if (kbd_unsubscribe_int() != 0) {
        printf("Unable to unsubscribe to the keyboard's interrupts\n");
        return 1;
    }

    if (timer_unsubscribe_int() != 0) {
        printf("Unable to unsubscribe to the timer's interrupts\n");
        return 1;
    }

    if (sp_unsubscribe_int() != 0) {
        printf("Unable to unsubscribe to the Serial Port's interrupts\n");
        return 1;
    }

    if (mouse_unsubscribe_int() != 0) {
        printf("Unable to unsubscribe to the mouse's interrupts\n");
        return 1;
    }

    if (mouse_disable_data_report() != 0) {
        printf("Unable to disable mouse data report\n");
        return 1;
    }

    if (vg_exit() != 0) {
        printf("Unable to disable graphics mode\n");
        return 1;
    }

    free(level);
    free_memory();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

    if (setup() != 0) { return 1;}

    int ipc_status,r;
    message msg;

    while(gameState != EXIT) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {

            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        if (KbdReadSecond) {
                            KbdReadSecond = false;
                            kbd_bytes[1] = scancode;
                        }
                        else {
                            kbd_bytes[0] = scancode;
                            if (scancode == SIZE) {
                                KbdReadSecond = true;
                            }
                        }
                        GeneralInterrupt(KEYBOARD);
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        if (!MouseReadSecond && !MouseReadThird && (scancode & BIT(3))){
                            ms_bytes[0] = scancode;
                            MouseReadSecond = true;
                        }
                        else if (MouseReadSecond) {
                            ms_bytes[1] = scancode;
                            MouseReadThird = true;
                            MouseReadSecond = false;
                        }
                        else if (MouseReadThird) {
                            ms_bytes[2] = scancode;
                            MouseReadThird = false;
                            for(unsigned int i = 0; i < 3; i++)
                                pack.bytes[i] = ms_bytes[i];
                            buildPacket(&pack);
                            GeneralInterrupt(MOUSE);
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_rtc) {
                        rtc_ih();
                        GeneralInterrupt(RTC);
                    }
                    if (msg.m_notify.interrupts & irq_set_sp) {
                        GeneralInterrupt(SERIALPORT);
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        GeneralInterrupt(TIMER);
                        double_buffer();
                    }
                    break;
                default:
                    break;
            }
        }
        if (gameState != MULTIPLAYER)
            sp_write();
    }

    if (leave() != 0) { return 1;}
    return 0;
}
