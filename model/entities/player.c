#include "player.h"

extern unsigned int hres, vres;
extern uint8_t scancode;
extern Level * level;
extern GameState gameState;

//Player * player;

Player * createPlayer (unsigned int playerNum) {
  Player * player = (Player *) malloc (sizeof(Player));

  player->x = 0;
  player->y = 0;
  player->xspeed = 3;
  player->yspeed = 3;
  player->hasMana = false;
  player->playerNum = playerNum;

  switch (playerNum) {
  case 0: // SINGLE-PLAYER
    xpm_load((xpm_map_t) mageDown_xpm, XPM_8_8_8_8, &player->down);
    xpm_load((xpm_map_t) mageUp_xpm, XPM_8_8_8_8, &player->up);
    xpm_load((xpm_map_t) mageLeft_xpm, XPM_8_8_8_8, &player->left);
    xpm_load((xpm_map_t) mageRight_xpm, XPM_8_8_8_8, &player->right);
    player->img = player->right;
    break;
  
  case 1: // MULTI-PLAYER - PLAYER 1
    player->x = 100;
    player->y = 100;
    xpm_load((xpm_map_t) mageDown_xpm, XPM_8_8_8_8, &player->down);
    xpm_load((xpm_map_t) mageUp_xpm, XPM_8_8_8_8, &player->up);
    xpm_load((xpm_map_t) mageLeft_xpm, XPM_8_8_8_8, &player->left);
    xpm_load((xpm_map_t) mageRight_xpm, XPM_8_8_8_8, &player->right);
    player->img = player->right;
    break;

  case 2: // MULTI-PLAYER - PLAYER 2
    player->x = 875;
    player->y = 600;
    xpm_load((xpm_map_t) mage2Down_xpm, XPM_8_8_8_8, &player->down);
    xpm_load((xpm_map_t) mage2Up_xpm, XPM_8_8_8_8, &player->up);
    xpm_load((xpm_map_t) mage2Left_xpm, XPM_8_8_8_8, &player->left);
    xpm_load((xpm_map_t) mage2Right_xpm, XPM_8_8_8_8, &player->right);
    player->img = player->right;
    break;

  default:
    break;
  }
  
  if (playerNum != 0)
    player->numLives = 3;
  else
    player->numLives = 1;

  //player->img = player->IdleRight[0];

  if (playerNum != 2)
    player->direction = RIGHT;
  else
    player->direction = LEFT;

  return player;
}

void draw_player(Player * player) {

  switch(player->direction) {
    case UP:{
        player->img = player->up;
        uint32_t* map = (uint32_t*) player->img.bytes;
        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
                if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                    changePixelColor(player->x+i,player->y+j,*(map + i + j*player->img.width));
            }
        }
      break;
    }

    case DOWN:{
        player->img = player->down;
        uint32_t* map = (uint32_t*) player->img.bytes;
        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
                if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                    changePixelColor(player->x+i,player->y+j,*(map + i + j*player->img.width));
            }
        }
      break;
    }

    case LEFT:{
        player->img = player->left;
        uint32_t* map = (uint32_t*) player->img.bytes;

        for(int i = 0; i < player->img.width; i++) {
            for (int j = 0; j < player->img.height; j++) {
                if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                    changePixelColor(player->x+i,player->y+j,*(map + i + j*player->img.width));
            }
      }
      break;
    }

    case RIGHT:{
        player->img = player->right;
        uint32_t* map = (uint32_t*) player->img.bytes;

        for(int i = 0; i < player->img.width; i++) {
          for (int j = 0; j < player->img.height; j++) {
          if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
            changePixelColor(player->x+i,player->y+j,*(map + i + j*player->img.width));
        }
      }
      break;
    }
  }
}

void clean_player(Player * player) {
  for (int i = player->x; i <= player->x + player->img.width; i++) {
    for (int j = player->y; j <= player->y + player->img.height; j++) {
      if (i < (int)hres - 1 && j < (int)vres - 1)
        changePixelColor(i,j,*(level->level_back + i + j * hres));
    }
  }

}

void update_movement_direction(Player * player, uint8_t scancode, bool * up, bool * down, bool * left, bool * right) {

  if (scancode == 0x11 /*Make-code W*/ || scancode == 0x48 /*Make-code Up-arrow*/) {
    *up = true;
    player->direction = UP;
  }
  else if (scancode == 0x1F /*Make-code S*/ || scancode == 0x50 /*Make-code Down-arrow*/) {
    *down = true;
    player->direction = DOWN;
  }
  else if (scancode == 0x20 /*Make-code D*/ || scancode == 0x4D /*Make-code Right-arrow*/) {
    *right = true;
    player->direction = RIGHT;
  }
  else if (scancode == 0x1E /*Make-code A*/ || scancode == 0x4B /*Make-code Left-arrow*/) {
    *left = true;
    player->direction = LEFT;
  }
  else if (scancode == 0x91 /*Break-code W*/ || scancode == 0xC8 /*Break-code Up-arrow*/)
    *up = false;  
  else if (scancode == 0x9F /*Break-code S*/ || scancode == 0xD0 /*Break-code Down-arrow*/)
    *down = false;
  else if (scancode == 0xA0 /*Break-code D*/ || scancode == 0xCD /*Break-code Right-arrow*/)
    *right = false;
  else if (scancode == 0x9E /*Break-code A*/ || scancode == 0xCB /*Break-code Left-arrow*/)
    *left = false;

}

void check_movement(Player * player, bool * upPtr, bool * downPtr, bool * leftPtr, bool * rightPtr) {

  static bool up, down, left, right;

  up = *upPtr; down = *downPtr; left = *leftPtr; right = *rightPtr; 

  if (up && !right && !left && !down)
    player->direction = UP;
  else if (down && !right && !left && !up) 
    player->direction = DOWN;
  else if (!up && right && !left && !down)
    player->direction = RIGHT;
  else if (!up && !right && left && !down) 
    player->direction = LEFT;

  if (up && !check_collision_walls(player,UP)) {

    clean_player(player);
    if (player->y - player->yspeed < 0)
      player->y = 0;
    else  
      player->y -= player->yspeed;

    if (right && !check_collision_walls(player,RIGHT)) {
      if (player->x + player->xspeed + player->img.width > (int)hres)
        player->x = (int)hres - player->img.width;
      else  
        player->x += player->xspeed;
    }
    if (left && !check_collision_walls(player,LEFT)) {
      if (player->x - player->xspeed < 0)
        player->x = 0;
      else  
        player->x -= player->xspeed;
    }
    if (down && !check_collision_walls(player,DOWN)) {
      if (player->y + player->yspeed > (int)vres - player->img.height - 4)
        player->y = (int)vres - player->img.height - 4;
      else  
        player->y += player->yspeed;
    } 
    draw_player(player);
  }

  else if (down && !check_collision_walls(player,DOWN)) {

    clean_player(player);
    if (player->y + player->yspeed > (int)vres - player->img.height - 4)
      player->y = (int)vres - player->img.height - 4;
    else  
      player->y += player->yspeed;
    
    if (right && !check_collision_walls(player,RIGHT)) {
      if (player->x + player->xspeed > (int)hres - player->img.width)
        player->x = (int)hres - player->img.width;
      else  
        player->x += player->xspeed;
    }
    if (left && !check_collision_walls(player,LEFT)) {
      if (player->x - player->xspeed < 0)
        player->x = 0;
      else  
        player->x -= player->xspeed;
    }
    draw_player(player);
  }

  else if (right && !check_collision_walls(player,RIGHT)) {

    clean_player(player);
    if (player->x + player->xspeed + player->img.width > (int)hres)
      player->x = (int)hres - player->img.width;
    else  
      player->x += player->xspeed;
    
    if (left && !check_collision_walls(player,LEFT)) {
      if (player->x - player->xspeed < 0)
        player->x = 0;
      else  
        player->x -= player->xspeed;
    }
    draw_player(player);
  }

  else if (left && !check_collision_walls(player, LEFT)) {

    clean_player(player);
    if (player->x - player->xspeed < 0)
      player->x = 0;
    else  
      player->x -= player->xspeed;
    draw_player(player);
  }
  
  if (check_collision_monster(player, player->direction, level->monsterList, level->numMonsters)) {
    up = false;
    down = false;
    left = false;
    right = false;
  } 
  
}

bool check_collision_walls(Player * player, Direction direction) {
  switch (direction) {
    case UP:
      if (player->y + player->img.width >= (int) hres)
        player->y = hres - player->img.width - 5;
      for (int i = player->x + 3; i <= player->x + player->img.width - 3; i++) {
        for (int j = player->y; j >= player->y - player->yspeed; j--) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;

    case DOWN:
      if (player->y + player->img.width >= (int) hres)
        player->y = hres - player->img.width - 5;
      for (int i = player->x + 3; i <= player->x + player->img.width - 3; i++) {
        for (int j = player->y + player->img.height; j <= player->y + player->img.height + player->yspeed; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;

    case LEFT:
      if (player->y + player->img.width >= (int) hres)
        player->y = hres - player->img.width - 5;
      for (int i = player->x; i >= player->x - player->xspeed; i--) {
        for (int j = player->y + 3; j <= player->y + player->img.height - 3; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;

    case RIGHT:
      if (player->y + player->img.width >= (int) hres)
        player->y = hres - player->img.width - 5;
      for (int i = player->x + player->img.width; i <= player->x + player->xspeed + player->img.width; i++) {
        for (int j = player->y + 3; j <= player->y + player->img.height - 3; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;    
  }

  return false;
}

bool check_collision_monster(Player * player, Direction direction, Monster ** monsters, unsigned int numMonsters) {
  switch (direction) {
    case UP:
       for (int i = player->x + 16; i <= player->x + player->img.width - 16; i++) {
        for (int j = player->y + 16; j >= player->y - player->yspeed + 16; j--) {
          for (unsigned int k = 0; k < numMonsters; k++) {
            if (monsters[k]->dead)
              continue;
            if (i > monsters[k]->x && i < monsters[k]->x + monsters[k]->img.width &&
                j > monsters[k]->y && j < monsters[k]->y + monsters[k]->img.height) {
                  return true;
            }
          }
        }
      }
      break;

    case DOWN:
      for (int i = player->x + 16; i <= player->x + player->img.width - 16; i++) {
        for (int j = player->y + player->img.height - 16; j <= player->y + player->img.height + player->yspeed - 16; j++) {
          for (unsigned int k = 0; k < numMonsters; k++) {
            if (monsters[k]->dead)
              continue;
            if (i > monsters[k]->x && i < monsters[k]->x + monsters[k]->img.width &&
                j > monsters[k]->y && j < monsters[k]->y + monsters[k]->img.height) {
                  return true;
            }
          }
        }
      }
      break;

    case LEFT:
      for (int i = player->x + 16; i >= player->x - player->xspeed + 16; i--) {
        for (int j = player->y + 16; j <= player->y + player->img.height - 16; j++) {
          for (unsigned int k = 0; k < numMonsters; k++) {
            if (monsters[k]->dead)
              continue;
            if (i > monsters[k]->x && i < monsters[k]->x + monsters[k]->img.width &&
                j > monsters[k]->y && j < monsters[k]->y + monsters[k]->img.height) {
                  return true;
            }
          }
        }
      }
      break;
    case RIGHT:
      for (int i = player->x + player->img.width - 16; i <= player->x + player->xspeed + player->img.width - 16; i++) {
        for (int j = player->y + 16; j <= player->y + player->img.height - 16; j++) {
          for (unsigned int k = 0; k < numMonsters; k++) {
            if (monsters[k]->dead)
              continue;
            if (i > monsters[k]->x && i < monsters[k]->x + monsters[k]->img.width &&
                j > monsters[k]->y && j < monsters[k]->y + monsters[k]->img.height) {
                  return true;
            }
          }
        }
      }
      break;    
  }

  return false;
}


Power * createPower(Player * player, PowerType type) {
  Power * power = (Power *) malloc(sizeof(Power));

  power->type = type;
  power->direction = player->direction;
  power->x = player->x;
  power->y = player->y;
  power->speed = 15;
  power->active = true;

  xpm_load((xpm_map_t) magePower_xpm, XPM_8_8_8_8, &power->img);

  return power;
}

Power * shoot(Player * player) {

  Power * power = createPower(player, NORMAL);

  player->hasMana = false;

  switch (power->direction) {
    case UP:
      power->x += player->img.width - 40;
      power->y -= 25;
      break;
    case DOWN:
      power->x += 5;
      power->y += player->img.height;
      break;
    case LEFT:
      power->x -= 35;
      power->y += 5;
      break;
    case RIGHT:
      power->x += player->img.width;
      power->y += player->img.height - 35;
      break;
  }
  return power;
}

/*
bool monsterPowerAnimation(Power * power, Player * player) {
  clean_power(power);

  switch (power->direction) {
    case UP:
      if (!check_collision_with_player(power, player)) {
        power->y -= power->speed;
        draw_power(power);
        return true;
      }
      break;
    case DOWN:
      if (!check_collision_with_player(power, player)) {
        power->y += power->speed;
        draw_power(power);
        return true;
      }
      break;
    case LEFT:
      if (!check_collision_with_player(power, player)) {
        power->x -= power->speed;
        draw_power(power);
        return true;
      }
      break;
    case RIGHT:
      if (!check_collision_with_player(power, player)) {
        power->x += power->speed;
        draw_power(power);
        return true;
      }
  }

  return false;
}
 */

bool check_collision_with_player(Power * power, Player * player) {
  switch (power->direction) {
    case UP:
      // CHECK COLLISIONS WITH PLAYER
      for (int i = power->x; i <= (int)power->x + (int)power->img.width; i++) {
        for (int j = power->y; j >= (int)power->y - (int)power->speed; j--) {
          if (i > player->x && i < player->x + player->img.width &&
              j > player->y && j < player->y + player->img.height) {
                power->active = false;
                draw_power(power);
                player->numLives--;
                if (player->playerNum != 0)
                  clean_heart(player->playerNum, player->numLives);
                return true;
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = power->x; i <= power->x + power->img.width; i++) {
        for (unsigned int j = power->y; j >= power->y - power->speed; j--) {
          if(*(level->level_walls + i + j * hres) == 0) {
            power->y = j + 1;
            power->active = false;
            return true;
          }
        }
      }
      break;
    case DOWN:
      // CHECK COLLISIONS WITH PLAYER
      for (int i = power->x; i <= (int)power->x + (int)power->img.width; i++) {
        for (int j = power->y + power->img.height; j <= (int)power->y + (int)power->img.height + (int)power->speed; j++) {
          if (i > player->x && i < player->x + player->img.width &&
              j > player->y && j < player->y + player->img.height) {
                power->active = false;
                draw_power(power);
                player->numLives--;
                if (player->playerNum != 0)
                  clean_heart(player->playerNum, player->numLives);
                return true;
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = power->x; i <= power->x + power->img.width; i++) {
        for (unsigned int j = power->y + power->img.height; j <= power->y + power->img.height + (int)power->speed; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            power->y = j - 1;
            power->active = false;
            return true;
          }
        }
      }
      break;
    case LEFT:
      // CHECK COLLISIONS WITH PLAYER
      for (int i = power->x; i >= (int)power->x - (int)power->speed; i--) {
        for (int j = power->y; j <= (int)power->y + (int)power->img.height; j++) {
          if (i > player->x && i < player->x + player->img.width &&
              j > player->y && j < player->y + player->img.height) {
                power->active = false;
                draw_power(power);
                player->numLives--;
                if (player->playerNum != 0)
                  clean_heart(player->playerNum, player->numLives);
                return true;
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = power->x; i >= power->x - power->speed; i--) {
        for (unsigned int j = power->y; j <= power->y + power->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            power->x = i - 1;
            power->active = false;
            return true;
          }
        }
      }
      break;
    case RIGHT:
      // CHECK COLLISIONS WITH PLAYER
      for (int i = power->x + power->img.width; i <= (int)power->x + (int)power->speed + power->img.width; i++) {
        for (int j = power->y; j <= (int)power->y + (int)power->img.height; j++) {
          if (i > player->x && i < player->x + player->img.width &&
              j > player->y && j < player->y + player->img.height) {
                power->active = false;
                draw_power(power);
                player->numLives--;
                if (player->playerNum != 0)
                  clean_heart(player->playerNum, player->numLives);
                return true;
          }
        }
      }

      // CHECK COLLISIONS WITH WALLS
      for (unsigned int i = power->x + power->img.width; i <= power->x + power->speed + power->img.width; i++) {
        for (unsigned int j = power->y; j <= power->y + power->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0) {
            power->x = i + 1;
            power->active = false;
            return true;
          }
        }
      }
      break;    
  }

  return false;
}

