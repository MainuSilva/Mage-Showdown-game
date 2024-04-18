#include "instructions.h"

extern uint8_t scancode;
extern unsigned int hres, vres;
extern struct packet pack;

extern GameState gameState;
extern Player * player;
extern Level * level;
extern Mouse * mouse;
extern DateTime * dateTime;

xpm_image_t background_menu;
xpm_image_t clean_background_menu;


void InstMenuInterruptHandler(Device device) {

    switch (device) {
        case TIMER:
            break;
        case KEYBOARD:
            if (scancode == ESC_BREAK) {
                gameState = MENU;
                LoadRtc();
                LoadMainMenu(false);
            }
            break;
        case MOUSE:
            break;
        case RTC:
            break;
        case SERIALPORT:
            break;
    }
}

void draw_instructions_background() {
    xpm_image_t img;
    xpm_load((xpm_map_t) instMenu_xpm, XPM_8_8_8_8, &img);
    mouse = load_mouse();

    uint32_t* map = (uint32_t*) img.bytes;

    for(unsigned int i = 0; i < img.width; i++) {
        for (unsigned int j = 0; j < img.height; j++) {
            changePixelColor(i,j,*(map + i + j*hres));
        }
    }
}

void clean_instructions_background() {
    uint32_t* map = (uint32_t*) background_menu.bytes;

    for (int i = 0; i < (int)hres; i++) {
        for (int j = 0; j < (int)vres; j++) {
            if (i < (int)hres - 1 && j < (int)vres - 1)
                changePixelColor(i,j,*(map + i + j * hres));
        }
    }
}

