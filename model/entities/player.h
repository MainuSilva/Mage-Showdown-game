#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"
#include "../../devices/keyboard_mouse/kbd.h"
#include "../game/Level.h"
#include "power.h"

#include "../../images/mage/mageRight.xpm"
#include "../../images/mage/mageLeft.xpm"
#include "../../images/mage/mageDown.xpm"
#include "../../images/mage/mageUp.xpm"
#include "../../images/mage2/mage2Right.xpm"
#include "../../images/mage2/mage2Left.xpm"
#include "../../images/mage2/mage2Up.xpm"
#include "../../images/mage2/mage2Down.xpm"
#include "../../images/mage/magePower.xpm"

/** @struct Player
 *  @brief Struct relativa ao Mago
 *  @var Player::img 
 *  XPM do Mago
 *  @var Player::x
 *  Posição dos x do Mago
 *  @var Player::y
 *  Posição dos y do Mago
 *  @var Player::xspeed
 *  Velocidade na direção horizontal do Mago
 *  @var Player::yspeed
 *  Velocidade na direção vertical do Mago
 *  @var Player::hasMana
 *  Boleano: 1 se o Mago tiver Mana, 0 caso contrário
 *  @var Player::direction
 *  Direção para a qual o Mago está virado
 *  @var Player::numLives
 *  Número de vidas do Mago
 *  @var Player::playerNum
 *  Número do Mago
 */
typedef struct {
  xpm_image_t down;
  xpm_image_t up;
  xpm_image_t left;
  xpm_image_t right;
  xpm_image_t img;

  int x, y;
  int xspeed, yspeed;
  bool hasMana;

  Direction direction;

  int numLives;
  unsigned int playerNum;

} Player;

/**
 *  @brief Aloca memória e cria o Mago
 *  @param playerNum Número do Mago
 *  @return Mago criado
 */
Player * createPlayer (unsigned int playerNum);

/**
 *  @brief Apresenta o Mago no ecrã
 *  @param player Mago a apresentar no ecrã
 */
void draw_player(Player * player);

/**
 *  @brief Remove o Mago do ecrã
 *  @param player Mago a remover do ecrã
 */
void clean_player(Player * player);

/**
 *  @brief Controlo da Direção do Mago
 *  @param player Mago a controlar a direção
 *  @param scancode Scancode recebido do Teclado
 *  @param up Boleano: 1 se o Mago estiver virado para Cima, 0 caso contrário
 *  @param down Boleano: 1 se o Mago estiver virado para Baixo, 0 caso contrário
 *  @param left Boleano: 1 se o Mago estiver virado para a Esquerda, 0 caso contrário
 *  @param right Boleano: 1 se o Mago estiver virado para a Direita, 0 caso contrário
 */
void update_movement_direction(Player * player, uint8_t scancode, bool * up, bool * down, bool * left, bool * right);

/**
 *  @brief Controlo do Movimento do Mago
 *  @param player Mago a controlar o movimento
 *  @param upPtr Boleano: 1 se o Mago se virar para Cima, 0 caso contrário
 *  @param downPtr Boleano: 1 se o Mago se virar para Baixo, 0 caso contrário
 *  @param leftPtr Boleano: 1 se o Mago se virar para a Esquerda, 0 caso contrário
 *  @param rightPtr Boleano: 1 se o Mago se virar para a Direita, 0 caso contrário
 */
void check_movement(Player * player, bool * upPtr, bool * downPtr, bool * leftPtr, bool * rightPtr);

/**
 *  @brief Verifica se o Mago colide com as Paredes
 *  @param player Mago a verificar colisões
 *  @param direction Direção para a qual o Mago está virado
 *  @return Boleano: 1 se houver colisões, 0 caso contrário
 */
bool check_collision_walls(Player * player, Direction direction);

/**
 *  @brief Verifica se o Mago colide com os Powers
 *  @param player Mago a verificar colisões
 *  @param direction Direção para a qual o Mago está virado
 *  @return Boleano: 1 se houver colisões, 0 caso contrário
 */
bool check_collision_ammo(Player * player, Direction direction);

/**
 *  @brief Verifica se o Mago colide com os Monstros
 *  @param player Mago a verificar colisões
 *  @param direction Direção para a qual o Mago está virado
 *  @param monsters Conjunto de Monstros do Nível
 *  @param numEnemies Número de Monstros do Nível
 *  @return Boleano: 1 se houver colisões, 0 caso contrário
 */
bool check_collision_monster(Player * player, Direction direction, Monster ** monsters, unsigned int numEnemies);

//--------------------------------------------------------------------------------------------------

/**
 *  @brief Aloca memória e cria o Power lançado pelo Mago
 *  @param player Mago que lança o Power
 *  @param type Tipo de Power lançado pelo Mago
 *  @return Power criado
 */
Power * createPower (Player * player, PowerType type);

/**
 *  @brief Controlo de um Power lançado pelo Mago
 *  @param player Mago que lança o Power
 *  @return Power lançado
 */
Power * shoot(Player * player);


/**
 *  @brief Verifica colisões do Power de um Monstro com um Mago
 *  @param power Power lançado pelo Monstro
 *  @param player Mago a testar colisões com o Power
 *  @return Boleano: 1 se houver colisões, 0 caso contrário
 */
bool check_collision_with_player(Power * power, Player * player);

