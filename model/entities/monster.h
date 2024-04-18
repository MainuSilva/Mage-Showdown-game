#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"
#include "../../devices/keyboard_mouse/kbd.h"

#include "../../images/monster/monsterDown.xpm"
#include "../../images/monster/monsterUp.xpm"
#include "../../images/monster/monsterLeft.xpm"
#include "../../images/monster/monsterRight.xpm"

typedef enum {NORMAL} PowerType;

/** @struct Power
 *  @brief Struct relativo ao Power
 *  @var Power::x
 *  Posição dos x do Power
 *  @var Power::y
 *  Posição dos y do Power
 *  @var Power::speed
 *  Velocidade do Power (frame-rate)
 *  @var Power::img
 *  XPM do Power
 *  @var Power::direction
 *  Direção do movimento do Power
 *  @var Power::active
 *  Boleano: 1 se o Power estiver a ser disparado, 0 caso contrário
 */
typedef struct {
    unsigned int x;
    unsigned int y;

    unsigned int speed;

    xpm_image_t img;
    PowerType type;
    Direction direction;

    bool active;
} Power;

/** @struct Monster
 *  @brief Struct relativa aos Monstros
 *  @var Monster::img 
 *  XPM do Monstro
 *  @var Monster::x
 *  Posição dos x do Monstro
 *  @var Monster::y
 *  Posição dos y do Monstro
 *  @var Monster::xspeed
 *  Velocidade na direção horizontal do Monstro
 *  @var Monster::yspeed
 *  Velocidade na direção vertical do Monstro
 *  @var Monster::isMoving
 *  Boleano: 1 se o Monstro estiver em Movimento, 0 caso contrário
 *  @var Monster::direction
 *  Direção para a qual o Monstro está virado
 *  @var Monster::dead
 *  Boleano: 1 se o Monstro estiver morto, 0 caso contrário
 *  @var Monster::power
 *  Objeto do Power disparado pelo Monstro
 */
typedef struct {
  xpm_image_t img;

  int x, y;
  int xspeed, yspeed;

  bool isMoving;
  Direction direction;

  bool dead;

  Power * power;

} Monster;

#include "../game/Level.h"
#include "power.h"

/**
 *  @brief Aloca memória e cria o Monstro
 *  @param x Posição dos x do Monstro
 *  @param y Posição dos y do Monstro
 *  @param direction Direção para a qual o Monstro está virado
 *  @param isStatic Boleano: 1 se o Monstro for estático (Atirador), 0 caso contrário (Guarda)
 *  @return Monstro criado
 */
Monster * createMonster(int x, int y, Direction direction);

/**
 *  @brief Apresenta o Monstro no ecrã
 *  @param Monster Monstro a apresentar no ecrã
 */
void draw_monster(Monster * monster);

/**
 *  @brief Remove o Monstro no ecrã
 *  @param Monster Monstro a remover do ecrã
 */
void clean_monster(Monster * monster);

/**
 *  @brief Torna o Monstro morto
 *  @param Monster Monstro a tornar morto
 */
void monster_death(Monster * monster);

/**
 *  @brief Controlo do Movimento do Monstro
 *  @param Monster Monstro que se movimenta
 */
void monster_movement(Monster * monster);

/**
 *  @brief Verifica se o Monstro colide com as Paredes
 *  @param Monster Monstro a verificar colisões
 *  @return Boleano: 1 se houver colisões, 0 caso contrário
 */
bool check_monster_collision_walls(Monster * monster);


