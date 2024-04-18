#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"

#include "../../images/mage/magePower.xpm"
#include "monster.h"
#include "../menu/menu.h"

/**
 *  @brief Inicia o disparo por um Monstro
 *  @param monster Monstro que dispara
 */
void monster_shoot(Monster * monster);

/**
 *  @brief Controlo do Movimento do Power
 *  @param power Power que se movimenta
 *  @param enemies Conjunto de Monstros do Nível
 *  @param numEnemies Número de Monstros do Nível
 *  @return Boleano: false se houver colisões, true caso contrário
 */
bool powerAnimation(Power * power, Monster ** enemies, unsigned int numEnemies);

/**
 *  @brief Apresenta no ecrã o Power
 *  @param power Power a apresentar no ecrã
 */
void draw_power(Power * power);

/**
 *  @brief Remove do ecrã o Power
 *  @param power Power a remover do ecrã
 */
void clean_power(Power * power);

/**
 *  @brief Verifica se o Power colide com um Monstro ou com as Paredes
 *  @param power Power a verificar colisões
 *  @param enemies Conjunto de Monstros do Nível
 *  @param numEnemies Número de Monstros do Nível
 *  @return Boleano: true se houver colisões, false caso contrário
 */
bool check_collisions_power(Power * power, Monster ** enemies, unsigned int numEnemies);
