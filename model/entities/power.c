#include "power.h"

extern Player * player;
extern Level * level;

extern GameState gameState;

extern unsigned int hres, vres;

void monster_shoot(Monster * monster) {
  monster->power->active = true;

  monster->power->direction = monster->direction;

  switch (monster->power->direction) {
    case UP:
      monster->power->x = monster->x + monster->img.width - 40;
      monster->power->y = monster->y - 25;
      break;
    case DOWN:
      monster->power->x = monster->x;
      monster->power->y = monster->y + monster->img.height + 25;
      break;
    case LEFT:
      monster->power->x = monster->x - 35;
      monster->power->y = monster->y;
      break;
    case RIGHT:
      monster->power->x = monster->x + monster->img.width;
      monster->power->y = monster->y + monster->img.height - 25;
      break;     
  }
}

bool powerAnimation(Power * power, Monster ** enemies, unsigned int numEnemies) {
  clean_power(power);

  switch (power->direction) {
    case UP:
      if (!check_collisions_power(power, enemies, numEnemies)) {
        power->y -= power->speed;
        draw_power(power);
        return true;
      }
      break;
    case DOWN:
      if (!check_collisions_power(power, enemies, numEnemies)) {
        power->y += power->speed;
        draw_power(power);
        return true;
      }
      break;
    case LEFT:
      if (!check_collisions_power(power, enemies, numEnemies)) {
        power->x -= power->speed;
        draw_power(power);
        return true;
      }
      break;
    case RIGHT:
      if (!check_collisions_power(power, enemies, numEnemies)) {
        power->x += power->speed;
        draw_power(power);
        return true;
      }
  }

  draw_power(power);
  return false;
}

void draw_power(Power * power) {
  uint32_t* map = (uint32_t*) power->img.bytes;

  for(unsigned int i = 0; i < power->img.width; i++) {
    for (unsigned int j = 0; j < power->img.height; j++) {
      if (*(map + i + j*power->img.width) != 16777215)
        changePixelColor(power->x+i,power->y+j,*(map + i + j*power->img.width));
    }
  }
}

void clean_power(Power * power) {

  for (unsigned int i = power->x; i <= power->x + power->img.width; i++) {
    for (unsigned int j = power->y; j <= power->y + power->img.height; j++) {
      if (i < hres - 1 && j < vres - 1)
        changePixelColor(i,j,*(level->level_back + i + j * hres));
    }
  }
}

bool check_collisions_power(Power * power, Monster ** enemies, unsigned int numEnemies) {
  switch (power->direction) {
    case UP:
      // CHECK COLLISIONS WITH MONSTERS
      for (int i = power->x; i <= (int)power->x + (int)power->img.width; i++) {
        for (int j = power->y; j >= (int)power->y - (int)power->speed; j--) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  monster_death(enemies[k]);
                  power->active = false;
                  return true;
            }
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
      // CHECK COLLISIONS WITH MONSTERS
      for (int i = power->x; i <= (int)power->x + (int)power->img.width; i++) {
        for (int j = power->y + power->img.height; j <= (int)power->y + (int)power->img.height + (int)power->speed; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  monster_death(enemies[k]);
                  power->active = false;
                  return true;
            }
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
      // CHECK COLLISIONS WITH MONSTERS
      for (int i = power->x; i >= (int)power->x - (int)power->speed; i--) {
        for (int j = power->y; j <= (int)power->y + (int)power->img.height; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  monster_death(enemies[k]);
                  power->active = false;
                  return true;
            }
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
      // CHECK COLLISIONS WITH MONSTERS
      for (int i = power->x + power->img.width; i <= (int)power->x + (int)power->speed + power->img.width; i++) {
        for (int j = power->y; j <= (int)power->y + (int)power->img.height; j++) {
          for (unsigned int k = 0; k < numEnemies; k++) {
            if (enemies[k]->dead)
              continue;
            if (i > enemies[k]->x && i < enemies[k]->x + enemies[k]->img.width &&
                j > enemies[k]->y && j < enemies[k]->y + enemies[k]->img.height) {
                  monster_death(enemies[k]);
                  power->active = false;
                  return true;
            }
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

