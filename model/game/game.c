#include "game.h"

static Player * player;
extern Level * level;
extern Mouse * mouse;
extern DateTime * dateTime;
extern GameState gameState;

extern struct packet pack;
extern struct mouse_ev mouse_event;
extern unsigned int counter;
extern uint8_t scancode;
extern unsigned int hres, vres;

void GeneralInterrupt(Device device) {
  switch (gameState){
    case MENU:
      MainMenuInterruptHandler(device);
      break;
    case INSTRUCTIONS:
      InstMenuInterruptHandler(device);
      break;
    case PLAY:
      SinglePlayerInterruptHandler(device);
      break;
    case PAUSEMENU:
      PauseMenuInterruptHandler(device);
      break;
    case MULTIPLAYER:
      MultiPlayerInterrupthandler(device);
      break;
    case WAITING:
      WaitingInterruptHandler(device);
      break;
    case WON:
      GameWonInterruptHandler(device);
      break;
    case LOST:
      GameLostInterruptHandler(device);
      break;
    case EXIT:
      break;
  }
}

void SinglePlayerInterruptHandler(Device device) {
  static Power * playerPower;
  srand(time(NULL));
  static unsigned int randomTimeShoot = 180;
  static bool monsterShoot = true;
  static bool powerOnMap = false;
  //static int checkLever;
  static bool up = false, down = false, left = false, right = false;

  switch (device) {
    case TIMER:
      if (player->numLives == 0) {
        gameState = LOST;
        up = false; down = false; left = false; right = false;
        check_movement(player, &up, &down, &left, &right);
        LoadLostMenu();
        break;
      }

      //CHECKS IF IT IS IN THE POSITION TO GO TO NEXT LEVEL
      if (check_final_position()) {
        gameState = WON;
        break;
      }

      //UPDATE PLAYER POSITION
      check_movement(player, &up, &down, &left, &right);

      if (monsterShoot) {
        randomTimeShoot = counter + (rand() % 280) + 120;
        monsterShoot = false;
      }

      //CHECKS IF IT COLLIDED WITH ANY MONSTER
      if (check_collision_monster(player, player->direction, level->monsterList, level->numMonsters)) {
        gameState = LOST;
      }

      if (gameState == LOST) {
        up = false; down = false; left = false; right = false;
        check_movement(player, &up, &down, &left, &right);
        LoadLostMenu();
      }

      break;
    case KEYBOARD:
      //PAUSE MENU
      if (scancode == ESC_BREAK) {
        gameState = PAUSEMENU;
        LoadRtc();
        LoadPauseMenu();
        break;
      }

      //SHOOT
      if (scancode == SPACEBAR && player->hasMana) {
        playerPower = shoot(player);
        //draw_mana_level();
        powerOnMap = true;
      }

      update_movement_direction(player, scancode, &up, &down, &left, &right);
      break;
    case MOUSE:
      break;
    case RTC:
      break;
    case SERIALPORT:
      break;
  }
}

void LoadSinglePLayer(unsigned int levelNum, bool begin) {
  static SinglePlayerGameState singlePlayerState = L1;

  player = createPlayer(0);
  singlePlayerState = L1;
  player->direction = UP;
  player->x = 105;
  player->y = 650;
  
  if (gameState == PLAY) {
      level = load_level(levelNum);
      player->hasMana = false;
    }
  else {
    draw_level_background();
    draw_monsters_level();
    draw_player(player);

    //if (!player->hasMana)
      //draw_mana_level();
  }

  scancode = 0x00;
}

bool check_final_position() {
  if (player->x > level->finalPos->posfinalx && player->x + player->img.width < level->finalPos->posfinalx + level->finalPos->lado * 16)
    if (player->y > level->finalPos->posfinaly && player->y + player->img.height < level->finalPos->posfinaly + level->finalPos->lado * 16)
      return true;
  
  return false;
}

void add_final_position_background(int *posfinalx, int *posfinaly, int *lado) {
  for(int i = 0; i < *lado * 16; i++) {
    for (int j = 0; j < *lado * 16; j++) {
      *((uint32_t*)level->level_background.bytes + (*posfinalx + i) + (*posfinaly + j)*hres) = 0xFFB624;
    }
  }
}

struct mouse_ev * mouse_events(struct packet * pack) {
  static bool lb_pressed = false, rb_pressed = false, mb_pressed = false;

  mouse_event.delta_x = pack->delta_x;
  mouse_event.delta_y = pack->delta_y;

  if (!lb_pressed && pack->lb && !rb_pressed && !mb_pressed && !pack->mb && !pack->rb) {
    lb_pressed = true;
    mouse_event.type = LB_PRESSED;
  }
  else if (!rb_pressed && pack->rb && !lb_pressed && !mb_pressed && !pack->mb && !pack->lb) {
    rb_pressed = true;
    mouse_event.type = RB_PRESSED;
  }
  else if (lb_pressed && !pack->lb && !rb_pressed && !mb_pressed && !pack->mb && !pack->rb) {
    lb_pressed = false;
    mouse_event.type = LB_RELEASED;
  }
  else if (rb_pressed && !pack->rb && !lb_pressed && !mb_pressed && !pack->mb && !pack->lb) {
    rb_pressed = false;
    mouse_event.type = RB_RELEASED;
  }
  else if (!mb_pressed && pack->mb) {
    mb_pressed = true;
    mouse_event.type = BUTTON_EV;
  }
  else if(mb_pressed && !pack->mb) {
    mb_pressed = false;
    mouse_event.type = BUTTON_EV;
  }
  else 
    mouse_event.type = MOUSE_MOV;

  return &mouse_event;
}












