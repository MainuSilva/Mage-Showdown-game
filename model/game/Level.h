#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"
#include "../entities/monster.h"

#include "../../images/levels/level0Walls.xpm"
#include "../../images/backgrounds/arena.xpm"
#include "../../images/stats/manaBar.xpm"

/** @struct Mana *  @brief Struct relativa à Mana do Nível
 *  @var Ammo::img 
 *  XPM da Mana
 *  @var Ammo::x
 *  Posição dos x da Mana
 *  @var Ammo::y
 *  Posição dos y da Mana
 */
typedef struct {
  xpm_image_t img;
  unsigned int x;
  unsigned int y;
} Mana;

/** @struct FinalPos
 *  @brief Struct relativa à Zona Final do Nível
 *  @var FinalPos::posfinalx
 *  Posição dos x da Zona Final
 *  @var FinalPos::posfinaly
 *  Posição dos y da Zona Final
 *  @var FinalPos::lado
 *  Comprimento do lado da Zona Final (x16 pixeis)
 */
typedef struct {
  int posfinalx;
  int posfinaly;
  int lado;
} FinalPos;

/** @struct Level
 *  @brief Struct relativa ao Nível
 *  @var Level::levelnum
 *  Número do Nível
 *  @var Level::level_background
 *  XPM do Fundo do Nível
 *  @var Level::level_walls_xpm
 *  XPM das Paredes do Nível (útil na deteção de colisões)
 *  @var Level::level_back
 *  Apontador para os Bytes de Fundo do Nível
 *  @var Level::level_walls
 *  Apontador para os Bytes das Paredes do Nível
 *  @var Level::finalPos
 *  Apontador para o objeto da Posição Final do Nível
 *  @var Level::manaList
 *  Array de todos os objetos de Mana existentes no Nível
 *  @var Level::numMana *  Número de objetos de Mana existentes no Nível
 *  @var Level::monsterList
 *  Array de todos os objetos de Monstros existentes no Nível
 *  @var Level::numMonsters
 *  Número de objetos de Monstros existentes no Nível
 */
typedef struct {
  unsigned int levelnum;

  xpm_image_t level_background;
  xpm_image_t level_walls_xpm;

  uint32_t * level_back;
  uint32_t * level_walls;

  FinalPos *finalPos;

  Mana ** manaList;
  unsigned int numMana;
  
  Monster ** monsterList;
  unsigned int numMonsters;
} Level;

/**
 *  @brief Aloca memória e cria a Zona Final
 *  @param posfinalx Posição dos x da Zona Final
 *  @param posfinaly Posição dos y da Zona Final
 *  @param posfinalx Lado da Zona Final (x16 bits)
 *  @return Zona Final criada
 */
FinalPos * createFinalPos(int posfinalx, int posfinaly, int lado);

/**
 *  @brief Aloca memória e cria a Mana
 *  @param x Posição dos x da Mana
 *  @param y Posição dos y da Mana
 *  @return Mana criada
 */
Mana * createMana(int x, int y);

/**
 *  @brief Carrega o Nível especificado no parâmetro
 *  @param levelNum Número do Nível a ser carregado
 *  @return Nível carregado
 */
Level * load_level(unsigned int levelNum);

/**
 *  @brief Liberta a memória ocupada pelo Nível
 */
void destroy_level();

/**
 *  @brief Apresenta no ecrã o fundo do Nível
 */
void draw_level_background();

/**
 *  @brief Apresenta no ecrã todas as Munições do Nível
 */
void draw_mana_level();

/**
 *  @brief Apresenta no ecrã todos os Monstros do Nível
 */
void draw_monsters_level();

