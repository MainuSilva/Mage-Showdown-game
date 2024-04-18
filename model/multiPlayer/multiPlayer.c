#include "multiPlayer.h"

#include "../../images/stats/blueHeart.xpm"
#include "../../images/stats/redHeart.xpm"
#include "../../images/backgrounds/menu_backgroud.xpm"

static xpm_image_t WaitingScreen, Arena;

#define SPACEBAR 0xB9

extern GameState gameState;

static uint8_t numPlayersOnline = 0;
extern int counter;

extern uint8_t scancode;
static uint8_t auxcode;

static Player * player1;
static Player * player2;

static Level * level;

static int playerNum;

static bool VmConnected = false;

void ReceivedSPInterruptFromMenu() {
  auxcode = readFromQueue();
  if (auxcode == 0x00)
    return;
  else if (auxcode == VM_CONNECTED) {
    VmConnected = true;
    addToTransmitQueue(RECEIVE_VM_CONNECTED);
  }
  else if (auxcode == VM_DISCONNECTED) {
    VmConnected = false;
  }
  else if (auxcode == RECEIVE_VM_CONNECTED) {
    VmConnected = true;
  }
  else if (auxcode == MULTIPLAYER_CONNECT) { // ENTROU 
    numPlayersOnline = 1;
    playerNum = 2;
  }
  else if (auxcode == MULTIPLAYER_DISCONNECT) { // SAIU
    numPlayersOnline = 0;
    playerNum = 1;
  }
}

void WaitingInterruptHandler(Device device) {
  switch (device) {
    case TIMER:
      auxcode = readFromQueue();

      if (auxcode == MULTIPLAYER_CONNECT) { // ENTROU
        numPlayersOnline = 2;
        playerNum = 1;
      }
      else if (auxcode == MULTIPLAYER_DISCONNECT) { // SAIU
        numPlayersOnline = 1;
        playerNum = 1;
      }
      else if (auxcode == VM_CONNECTED) {
        VmConnected = true;
        addToTransmitQueue(RECEIVE_VM_CONNECTED);
        numPlayersOnline = 1;
        playerNum = 1;
      }
      else if (auxcode == VM_DISCONNECTED) {
        VmConnected = false;
        numPlayersOnline = 1;
        playerNum = 1;
      }
      else if (auxcode == RECEIVE_VM_CONNECTED) {
        VmConnected = true;
        numPlayersOnline = 1;
        playerNum = 1;
      }
      

      if (numPlayersOnline == 2) {
        gameState = MULTIPLAYER;
        LoadMultiPlayer();
        break;
      }

      if (counter % 30 == 0)
        addToTransmitQueue(MULTIPLAYER_CONNECT);
      break;
    case KEYBOARD:
      if (scancode == ESC_MAKE) {
        gameState = MENU;
        numPlayersOnline--;
        addToTransmitQueue(MULTIPLAYER_DISCONNECT);
        LoadRtc();
        LoadMainMenu(false);
        break;
      }
      break;
    case MOUSE:
      break;
    case RTC:
      break;
    case SERIALPORT:
      sp_ih();
      break;
  }
}

static bool up2 = false, down2 = false, left2 = false, right2 = false;
static bool player2Shoot = false;

void MultiPlayerInterrupthandler(Device device) {
  static Power * player1Power;
  static Power * player2Power;

  static bool power1OnMap = false;
  static bool power2OnMap = false;

  uint8_t sp_bit_no;

  static bool up1 = false, down1 = false, left1 = false, right1 = false;
  //static bool up2 = false, down2 = false, left2 = false, right2 = false;

  if (playerNum == 1) {
    sp_read();
    do {
      auxcode = readFromQueue();
      handle_data(auxcode);
    } while (!sp_read() || auxcode);
    auxcode = readFromQueue();
    handle_data(auxcode);
    sp_write();
  }
  if (playerNum == 2) {
    sp_write();
    sp_read();
    do {
      auxcode = readFromQueue();
      handle_data(auxcode);
    } while (!sp_read() || auxcode);
    auxcode = readFromQueue();
    handle_data(auxcode);
  }

  if (player2Shoot) {
    player2Power = shoot(player2);
    power2OnMap = true;
    player2Shoot = false;
  }

  switch (device) {
    case TIMER:
      if (player2->numLives == 0) {
        gameState = WON;
        sp_unsubscribe_int();
        LoadWonMenu();
        sp_subscribe_int(&sp_bit_no);
        break;
      }
      if (player1->numLives == 0) {
        gameState = LOST;
        sp_unsubscribe_int();
        LoadLostMenu();
        sp_subscribe_int(&sp_bit_no);
        break;
      }

      if (numPlayersOnline < 2) {
        gameState = WAITING;
        sp_unsubscribe_int();
        numPlayersOnline = 0;
        LoadWaitingMenu();
        sp_subscribe_int(&sp_bit_no);
        break;
      }

      //UPDATE PLAYER POSITION
      check_movement(player1, &up1, &down1, &left1, &right1);
      check_movement(player2, &up2, &down2, &left2, &right2);

      break;
    case KEYBOARD:
      //PAUSE MENU
      if (scancode == ESC_MAKE) {
        gameState = MENU;
        addToTransmitQueue(MULTIPLAYER_DISCONNECT);
        sp_unsubscribe_int();
        LoadRtc();
        LoadMainMenu(false);
        numPlayersOnline--;
        sp_subscribe_int(&sp_bit_no);
        break;
      }

      //SHOOT
      if (scancode == SPACEBAR && player1->hasMana) {
        player1Power = shoot(player1);
        power1OnMap = true;
        addToTransmitQueue(scancode);
      }

      //UPDATE MOVING DIRECTION
      update_movement_direction(player1, scancode, &up1, &down1, &left1, &right1);
      
      if (scancode == A_MAKE || scancode == S_MAKE || scancode == D_MAKE || scancode == W_MAKE || scancode == LEFT_ARROW_MAKE || scancode == DOWN_ARROW_MAKE || scancode == RIGHT_ARROW_MAKE ||scancode == UP_ARROW_MAKE ||
      scancode == A_BREAK || scancode == S_BREAK || scancode == D_BREAK || scancode == W_BREAK || scancode == LEFT_ARROW_BREAK || scancode == DOWN_ARROW_BREAK || scancode == RIGHT_ARROW_BREAK || scancode == UP_ARROW_BREAK ) {
        addToTransmitQueue(scancode);
        //sp_write();
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

void LoadMultiPlayerXPM() {
  xpm_load((xpm_map_t) menu_xpm, XPM_8_8_8_8, &Arena);
}

void LoadWaitingMenu() {
  numPlayersOnline++;

  uint32_t* map = (uint32_t*) WaitingScreen.bytes;

  for(unsigned int i = 0; i < WaitingScreen.width; i++) {
    for (unsigned int j = 0; j < WaitingScreen.height; j++) {
      changePixelColor(i,j,*(map + i + j * WaitingScreen.width));
    }
  }
}

void LoadMultiPlayer() {
  player1 = createPlayer(playerNum);

  if (playerNum == 1)
    player2 = createPlayer(2);
  else
    player2 = createPlayer(1);
  
  
  level = load_level(0);

  draw_hearts();
}

void draw_hearts() {
  xpm_image_t img;

  xpm_load((xpm_map_t) blueHeart_xpm, XPM_8_8_8_8, &img);
  uint32_t * map = (uint32_t*)img.bytes;
  int x = 100;
  int y = 734;

  for (int k = 0; k < 3; k++) {
    for(int i = 0; i < 32; i++) {
      for (int j = 0; j < 32; j++) {
        if (*(map + i + j * img.width) != xpm_transparency_color(XPM_8_8_8_8))
          changePixelColor(x + i, y + j,*(map + i + j * img.width));
      }
    }
    x += 40;
  }

  xpm_load((xpm_map_t) redHeart_xpm, XPM_8_8_8_8, &img);
  map = (uint32_t*)img.bytes;
  x = 830;
  y = 734;

  for (int k = 0; k < 3; k++) {
    for(int i = 0; i < 32; i++) {
      for (int j = 0; j < 32; j++) {
        if (*(map + i + j * img.width) != xpm_transparency_color(XPM_8_8_8_8))
          changePixelColor(x + i, y + j,*(map + i + j * img.width));
      }
    }
    x += 40;
  }

  free(&img);
}

void clean_heart(int playerNum, int numLives) {
  int x = 100;
  int y = 734;

  if (playerNum == 2)
    x = 830;

  x += 40 * numLives;

  for(int i = 0; i < 150; i++) {
      for (int j = 0; j < 32; j++) {
        changePixelColor(x + i, y + j,*(level->level_back + (x + i) + (y + j) * 1024));
      }
    }
}

void handle_data(uint8_t auxcode) {
  if (auxcode == 0x00)
    return;
  else if (auxcode == MULTIPLAYER_DISCONNECT) { // SAIU
    numPlayersOnline--;
    playerNum = 1;
  }
  else if (auxcode == VM_DISCONNECTED) {
    numPlayersOnline--;
    playerNum = 1;
  }
  else if (auxcode == SPACEBAR) {
    player2Shoot = true;
  }
  else {
    update_movement_direction(player2, auxcode, &up2, &down2, &left2, &right2);
  }
}

