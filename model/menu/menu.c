#include "menu.h"

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

static Button ** pauseMenuButtons;
static Button ** mainMenuButtons;

void MainMenuInterruptHandler(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool overExit = false, overSingleplayer = false, overMultiplayer = false, overInstructions = false;
  bool clickedExit = false, clickedSingleplayer = false, clickedMultiplayer = false, clickedInstructions = false;

  switch (device) {
    case TIMER:
      ReceivedSPInterruptFromMenu();

      switch(check_collision_main_menu()) {
        case 0:
          if (overExit) {
            overExit = false;
            mainMenuButtons[3]->mouseOver = false;
            add_button_to_background(mainMenuButtons[3]);
            draw_button(mainMenuButtons[3]);
          }
          else if (overSingleplayer) {
            overSingleplayer = false;
            mainMenuButtons[0]->mouseOver = false;
            add_button_to_background(mainMenuButtons[0]);
            draw_button(mainMenuButtons[0]);
          }
          else if (overMultiplayer) {
            overMultiplayer = false;
            mainMenuButtons[1]->mouseOver = false;
            add_button_to_background(mainMenuButtons[1]);
            draw_button(mainMenuButtons[1]);
          }
          else if (overInstructions) {
            overInstructions = false;
            mainMenuButtons[2]->mouseOver = false;
            add_button_to_background(mainMenuButtons[2]);
            draw_button(mainMenuButtons[2]);
          }
          break;

        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickedExit = true;
            break;
          }
          if (!overExit) {
            overExit = true;
            mainMenuButtons[3]->mouseOver = true;
            add_button_to_background(mainMenuButtons[3]);
            draw_button(mainMenuButtons[3]);
          }
          break;
        case 3:
          if (mouseEvent->type == LB_RELEASED) {
            clickedSingleplayer = true;
            break;
          }
          if (!overSingleplayer) {
            overSingleplayer = true;
            mainMenuButtons[0]->mouseOver = true;
            add_button_to_background(mainMenuButtons[0]);
            draw_button(mainMenuButtons[0]);
          }
          break;
        case 4:
          if (mouseEvent->type == LB_RELEASED) {
            clickedMultiplayer = true;
            break;
          }
          if (!overMultiplayer) {
            overMultiplayer = true;
            mainMenuButtons[1]->mouseOver = true;
            add_button_to_background(mainMenuButtons[1]);
            draw_button(mainMenuButtons[1]);
          }
          break;
        case 5: 
          if (mouseEvent->type == LB_RELEASED) {
            clickedInstructions = true;
            break;
          }
          if (!overInstructions) {
            overInstructions = true;
            mainMenuButtons[2]->mouseOver = true;
            add_button_to_background(mainMenuButtons[2]);
            draw_button(mainMenuButtons[2]);
          }
          break;

      }
      break;
    case KEYBOARD:
      break;
    case MOUSE:
      mouseEvent = mouse_events(&pack);
      mouse_update(&pack);
      break;
    case RTC:
      //print_clock();
      clean_clock();
      updateDateTime();
      draw_clock();
      break;
    case SERIALPORT:
      sp_ih();
      //ReceivedSPInterruptFromMenu();
      break;
  }

  if (clickedExit)
    gameState = EXIT;
  else if (clickedSingleplayer) {
    gameState = PLAY;
    clean_clock();
    LoadSinglePLayer(1,true);
  }
  else if (clickedMultiplayer) {
    addToTransmitQueue(0xFF);
    gameState = WAITING;
    clean_clock();
    LoadWaitingMenu();
  }
  else if (clickedInstructions) {
    gameState = INSTRUCTIONS;
    draw_instructions_background();
    return;
  }
}

void PauseMenuInterruptHandler(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool overExit = false, overContinue = false;
  bool clickedExit = false, clickedContinue = false;

  switch (device) {
    case TIMER:
      switch(check_collision_pause_menu()) {
        case 0:
          if (overExit) {
            overExit = false;
            pauseMenuButtons[1]->mouseOver = false;
            add_button_to_background(pauseMenuButtons[1]);
            draw_button(pauseMenuButtons[1]);
          }
          else if (overContinue) {
            overContinue = false;
            pauseMenuButtons[0]->mouseOver = false;
            add_button_to_background(pauseMenuButtons[0]);
            draw_button(pauseMenuButtons[0]);
          }
          break;
        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickedExit = true;
            break;
          }
          if (!overExit) {
            overExit = true;
            pauseMenuButtons[1]->mouseOver = true;
            add_button_to_background(pauseMenuButtons[1]);
            draw_button(pauseMenuButtons[1]);
          }
          break;
        case 2:
          if (mouseEvent->type == LB_RELEASED) {
            clickedContinue = true;
            break;
          }
          if (!overContinue) {
            overContinue = true;
            pauseMenuButtons[0]->mouseOver = true;
            add_button_to_background(pauseMenuButtons[0]);
            draw_button(pauseMenuButtons[0]);
          }
          break;
      }
      break;
    case KEYBOARD:
      if (scancode == ESC_MAKE) {
        gameState = PLAY;
        LoadSinglePLayer(level->levelnum, false);
      }
      break;
    case MOUSE:
      mouseEvent = mouse_events(&pack);
      mouse_update(&pack);
      break;
    case RTC:
      print_clock();
      clean_clock();
      updateDateTime();
      draw_clock();
      break;
    case SERIALPORT:
      break;
  }

  if (clickedExit) {
    gameState = MENU;
    LoadRtc();
    LoadMainMenu(false);
  }
  else if (clickedContinue){
    gameState = PLAY;
    LoadSinglePLayer(level->levelnum, false);
  }
  else 
    return;
}

void LoadMainMenu(bool firstLoad) {
  load_background();

  mouse = load_mouse();

  mainMenuButtons = (Button **) malloc(4 * sizeof(Button *));

  mainMenuButtons[0] = createButton(BUTTON_SINGLEPLAYER,400,300);
  mainMenuButtons[1] = createButton(BUTTON_MULTIPLAYER,360,375);
  mainMenuButtons[2] = createButton(BUTTON_HELP,350,435);
  mainMenuButtons[3] = createButton(BUTTON_EXIT,405,510);

  xpm_image_t logotip;
  xpm_load((xpm_map_t) gameLogo_xpm, XPM_8_8_8_8, &logotip);

  add_button_to_background(mainMenuButtons[0]);
  add_button_to_background(mainMenuButtons[1]);
  add_button_to_background(mainMenuButtons[2]);
  add_button_to_background(mainMenuButtons[3]);
  add_to_background(&logotip, 240, 90);

  draw_menu_background();
}

void LoadPauseMenu() {
  load_background();
  mouse = load_mouse();

  pauseMenuButtons = (Button **) malloc(2 * sizeof(Button *));

  pauseMenuButtons[0] = createButton(BUTTON_CONTINUE,320,384);
  pauseMenuButtons[1] = createButton(BUTTON_EXIT,320,576);
  
  add_button_to_background(pauseMenuButtons[0]);
  add_button_to_background(pauseMenuButtons[1]);

  draw_menu_background();
}

void load_background() {
    xpm_load((xpm_map_t) menu_xpm, XPM_8_8_8_8, &background_menu);
    xpm_load((xpm_map_t) menu_xpm, XPM_8_8_8_8, &clean_background_menu);
}

void draw_menu_background() {
  uint32_t* map = (uint32_t*) background_menu.bytes;

  for(unsigned int i = 0; i < background_menu.width; i++) {
    for (unsigned int j = 0; j < background_menu.height; j++) {
      changePixelColor(i,j,*(map + i + j*hres));
    }
  }
}

void add_to_background(xpm_image_t * img, int x, int y) {
  uint32_t * map, * imgMap;

  map = (uint32_t *)background_menu.bytes;

  imgMap = (uint32_t *)img->bytes;

  for(int i = 0; i < img->width; i++) {
    for (int j = 0; j < img->height; j++) {
      if (*(imgMap + i + j*img->width) != xpm_transparency_color(XPM_8_8_8_8))
        *(map + (x + i) + (y + j) * hres) = *(imgMap + i + j*img->width);
    }
  }
}




