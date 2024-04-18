#include "monster.h"

extern unsigned int hres, vres;
extern Level * level;
extern Player * player;

Monster * createMonster (int x, int y, Direction direction) {
  Monster * monster = (Monster *) malloc (sizeof(Monster));

  monster->x = x;
  monster->y = y;

  monster->xspeed = 3;
  monster->yspeed = 3;

  monster->isMoving = true;

  monster->dead = false;

  monster->direction = direction;

  monster->power = (Power *) malloc(sizeof(Power));

  monster->power->type = NORMAL;
  monster->power->direction = monster->direction;
  monster->power->x = monster->x;
  monster->power->y = monster->y;
  monster->power->speed = 17;

  xpm_load((xpm_map_t) magePower_xpm, XPM_8_8_8_8, &monster->power->img);
  monster->power->active = false;

  xpm_image_t img;

  xpm_load((xpm_map_t) monsterDown_xpm, XPM_8_8_8_8, &img);
  monster->img = img;

  return monster;
}

void draw_monster(Monster * monster) {
  if (monster->dead)
    return;

  uint32_t* map = (uint32_t*) monster->img.bytes;

  for(int i = 0; i < monster->img.width; i++) {
      for (int j = 0; j < monster->img.height; j++) {
          if (*(map + i + j*monster->img.width) != xpm_transparency_color(XPM_8_8_8_8))
            changePixelColor(monster->x+i,monster->y+j,*(map + i + j*monster->img.width));
      }
  }
}

void clean_monster(Monster * monster) {
  if (monster->dead)
    return;

  for (int i = monster->x; i <= monster->x + monster->img.width; i++) {
    for (int j = monster->y; j <= monster->y + monster->img.height; j++) {
      if (i < (int)hres - 1 && j < (int)vres - 1)
        changePixelColor(i,j,*(level->level_back + i + j * hres));
    }
  }
}

void monster_death(Monster * monster) {
  clean_monster(monster);
  monster->dead = true;
}

void monster_movement(Monster * monster) {
  if (monster->dead)
    return;

  bool up = false, down = false, right = false, left = false;

  switch(monster->direction) {
    case UP:
      up = true;
      break;
    case DOWN:
      down = true;
      break;
    case LEFT:
      left = true;
      break;
    case RIGHT:
      right = true;
      break;
  }

  if (up) {
    if(!check_monster_collision_walls(monster)) {
      clean_monster(monster);

      if (monster->y - monster->yspeed < 0) {
        monster->direction = DOWN;
        monster->y = 0;
      }
      else  
        monster->y -= monster->yspeed;
  
      draw_monster(monster);
    }
    else {
      monster->direction = DOWN;
    }
  }
  else if (down) {
    if (!check_monster_collision_walls(monster)) {
      clean_monster(monster);

      if (monster->y + monster->yspeed > (int)vres - monster->img.height) {
        monster->direction = UP;
        monster->y = (int)vres - monster->img.height - 1;
      }
      else  
        monster->y += monster->yspeed;
      
      draw_monster(monster);
    }
    else {
      monster->direction = UP;
    }
  }

  else if (right) {
    if(!check_monster_collision_walls(monster)) {
      clean_monster(monster);

      if (monster->x + monster->xspeed + monster->img.width > (int)hres) {
        monster->direction = LEFT;
        monster->x = (int)hres - monster->img.width - 1;
      }
      else
        monster->x += monster->xspeed;
      
      draw_monster(monster);
    }
    else {
      monster->direction = LEFT;
    }
  }

  else if (left) {
    if(!check_monster_collision_walls(monster)) {
      clean_monster(monster);

      if (monster->x - monster->xspeed < 0) {
        monster->direction = RIGHT;
        monster->x = 0;
      }
      else  
        monster->x -= monster->xspeed;

      draw_monster(monster);
    }
    else {
      monster->direction = RIGHT;
    }
  }
  
}

bool check_monster_collision_walls(Monster * monster) {
  switch (monster->direction) {
    case UP:
      for (int i = monster->x; i <= monster->x + monster->img.width; i++) {
        for (int j = monster->y; j >= monster->y - monster->yspeed; j--) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;
    case DOWN:
      for (int i = monster->x; i <= monster->x + monster->img.width; i++) {
        for (int j = monster->y + monster->img.height; j <= monster->y + monster->img.height + monster->yspeed; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;
    case LEFT:
      for (int i = monster->x; i >= monster->x - monster->xspeed; i--) {
        for (int j = monster->y; j <= monster->y + monster->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;
    case RIGHT:
      for (int i = monster->x + monster->img.width; i <= monster->x + monster->xspeed + monster->img.width; i++) {
        for (int j = monster->y; j <= monster->y + monster->img.height; j++) {
          if(*(level->level_walls + i + j * hres) == 0)
            return true;
        }
      }
      break;    
  }

  return false;
}

