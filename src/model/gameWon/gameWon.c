#include "gameWon.h"

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

static Button * continueButton;

void GameWonInterruptHandler(Device device) {
    static struct mouse_ev * mouseEvent;
    static bool overContinue = false;
    bool clickedContinue = false;

    switch (device) {
        case TIMER:
            switch(check_collision_won_menu()) {
                case 0:
                    if (overContinue) {
                        overContinue = false;
                        continueButton->mouseOver = false;
                        add_button_to_background(continueButton);
                        draw_button(continueButton);
                    }
                    break;
                case 1:
                    if (mouseEvent->type == LB_RELEASED) {
                        clickedContinue = true;
                        break;
                    }
                    if (!overContinue) {
                        overContinue = true;
                        continueButton->mouseOver = true;
                        add_button_to_background(continueButton);
                        draw_button(continueButton);

                    }
                    break;
            }
            break;
        case KEYBOARD:
            if (scancode == ESC_MAKE) {
                gameState = MENU;
                LoadRtc();
                LoadMainMenu(false);
            }
            break;
        case MOUSE:
            mouseEvent = mouse_events(&pack);
            mouse_update(&pack);
            break;
        case RTC:
            break;
        case SERIALPORT:
            break;
    }

    if (clickedContinue) {
        gameState = MENU;
        LoadMainMenu(false);
        LoadRtc();
    }
}

void LoadWonMenu() {
    load_background();
    mouse = load_mouse();

    xpm_image_t img;
    xpm_load((xpm_map_t) gameWin_xpm, XPM_8_8_8_8, &img);

    uint32_t * map = (uint32_t *)img.bytes;

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (*(map + j + i * img.width) != xpm_transparency_color(XPM_8_8_8_8))
                *((uint32_t *)background_menu.bytes + j + i*hres) = *(map + j + i * img.width);
        }
    }
    continueButton = createButton(BUTTON_CONTINUE,320,484);

    add_button_to_background(continueButton);

    draw_menu_background();
}
