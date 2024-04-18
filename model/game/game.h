#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"
#include "../../devices/timer/i8254.h"
#include "../../devices/keyboard_mouse//i8042.h"
#include "../../devices/keyboard_mouse/kbd.h"

typedef enum {
    TIMER,
    KEYBOARD,
    MOUSE,
    RTC,
    SERIALPORT
} Device;

typedef enum {
    MENU,
    PLAY,
    LOST,
    WON,
    PAUSEMENU,
    MULTIPLAYER,
    WAITING,
    INSTRUCTIONS,
    EXIT
} GameState;

typedef enum {
    L1,
    PAUSE,
    END
} SinglePlayerGameState;

#include "Level.h"
#include "../entities/player.h"
#include "../entities/power.h"
#include "../time/time.h"
#include "../menu/menu.h"
#include "../gameOver/gameOver.h"
#include "../gameWon/gameWon.h"
#include "../instructions/instructions.h"
#include "../multiPlayer/multiPlayer.h"

/**
 *  @brief Controlo geral de Interrupções para um qualquer dispositivo
 *  @param device Dispositivo que gerou interrupções
 */
void GeneralInterrupt(Device device);

/**
 *  @brief Controlo de Interrupções relativas ao Single-Player para um qualquer dispositivo
 *  @param device Dispositivo que gerou interrupções
 */
void SinglePlayerInterruptHandler(Device device);

/**
 *  @brief Carregar estado de um Nível
 *  @param levelNum Número do Nível
 *  @param begin Boleano: 1 se o Nível estiver no estado inicial, 0 caso contrário
 */
void LoadSinglePLayer(unsigned int levelNum, bool begin);

/**
 *  @brief Verifica se o Jogador está perto da Zona Final
 *  @return Boleano: 1 se o Jogador estiver perto da Zona Final, 0 caso contrário
 */
bool check_final_position();

/**
 *  @brief Adiciona a Zona Final ao Fundo do Nível
 *  @param posfinalx Posição dos x da Zona Final
 *  @param posfinaly Posição dos y da Zona Final
 *  @param lado Comprimento do lado da Zona Final (x16 pixeis)
 */
void add_final_position_background(int *posfinalx, int *posfinaly, int *lado);

/**
 *  @brief Controlo do Rato conforme o Mouse Packet Recebido
 *  @param pack Mouse Packet com as informações do Rato
 *  @return Tipo de evento causado pelo Mouse Packet recebido
 */
struct mouse_ev * mouse_events(struct packet * pack);

