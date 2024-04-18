#include "Level.h"

extern unsigned int hres, vres;

Level * level;

Mana * createMana(int x, int y) {
  Mana * mana = (Mana *) malloc (sizeof(Mana));

  mana->x = x;
  mana->y = y;

  xpm_image_t img;

  xpm_load((xpm_map_t) manaBar_xpm, XPM_8_8_8_8, &img);

  mana->img = img;

  return mana;
}

FinalPos * createFinalPos(int posfinalx, int posfinaly, int lado) {
    
  FinalPos * finalPos = (FinalPos *) malloc (sizeof(FinalPos));

  finalPos->posfinalx = posfinalx;
  finalPos->posfinaly = posfinaly;
  finalPos->lado = lado;

  return finalPos;
}

Level * load_level(unsigned int levelNum) {
  free(level);
  level = (Level *) malloc (sizeof(Level));

  level->levelnum = levelNum;
    xpm_load((xpm_map_t) arena_xpm, XPM_8_8_8_8, &level->level_background);
    xpm_load((xpm_map_t) level0Walls_xpm, XPM_8_8_8_8, &level->level_walls_xpm);
    level->finalPos = createFinalPos(800,360,5);
    level->numMana = 1;
    level->manaList = (Mana **) malloc (level->numMana * sizeof(Mana *));
    level->manaList[0] = createMana(288,704);
    level->numMonsters = 5;
    level->monsterList = (Monster **) malloc (level->numMonsters * sizeof(Monster *));
    level->monsterList[0] = createMonster(912,64,DOWN);
    level->monsterList[1] = createMonster(780,64,DOWN);
    level->monsterList[2] = createMonster(60,40,DOWN);
    level->monsterList[3] = createMonster(368,64,DOWN);
    level->monsterList[4] = createMonster(200,395,RIGHT);

    level->level_back = (uint32_t*) level->level_background.bytes;
    level->level_walls = (uint32_t*) level->level_walls_xpm.bytes;

  if (levelNum != 0) {
      add_final_position_background(&level->finalPos->posfinalx, &level->finalPos->posfinaly, &level->finalPos->lado);
  }

  draw_mana_level();
  draw_level_background();

  if (levelNum != 0) {
    draw_monsters_level();
  }

  return level;
}

void destroy_level() {

  if (level == NULL)
    return;

  if (level->level_background.bytes)
      free(level->level_background.bytes);

  if (level->level_walls_xpm.bytes)
    free(level->level_walls_xpm.bytes);

  free(level);

  level = NULL; 
}

void draw_level_background() {
  for(unsigned int i = 0; i < level->level_background.width; i++) {
    for (unsigned int j = 0; j < level->level_background.height; j++) {
      changePixelColor(i,j,*(level->level_back + i + j*hres));
    }
  }
}

void draw_mana_level() {
  unsigned int x, y;

  for (unsigned int k = 0; k < level->numMana; k++) {
    x = level->manaList[k]->x;
    y = level->manaList[k]->y;
    uint32_t * mana_map = (uint32_t *)level->manaList[k]->img.bytes;
    for(int i = 0; i < level->manaList[k]->img.width; i++) {
      for (int j = 0; j < level->manaList[k]->img.height; j++) {
        if (*(mana_map + i + j*level->manaList[k]->img.width) != xpm_transparency_color(XPM_8_8_8_8))
          *(level->level_back + (x + i) + (y + j) * hres) = *(mana_map + i + j*level->manaList[k]->img.width);
      }
    }
  }
}

void draw_monsters_level() {
  for (unsigned int i = 0; i < level->numMonsters; i++)
    draw_monster(level->monsterList[i]);
}

void clean_picked_mana(unsigned int x, unsigned int y) {
  for(unsigned int i = x; i < x + 32; i++) {
    for (unsigned int j = y; j < y + 16; j++) {
      changePixelColor(i, j,*(level->level_back + i + j * hres));
    }
  }
}

void clean_mana_level() {
  for (unsigned int i = 0; i < level->numMana; i++) {
    for(unsigned int j = level->manaList[i]->x; j < level->manaList[i]->x + 32; j++) {
      for (unsigned int k = level->manaList[i]->y; k < level->manaList[i]->y + 16; k++) {
        changePixelColor(j, k,*(level->level_back + j + k * hres));
      }
    }
  }
}

