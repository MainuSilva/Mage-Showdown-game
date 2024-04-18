#pragma once

#include <lcom/lcf.h>

#include "../menu/menu.h"
#include "../../devices/serialPort/serial_port.h"
#include "../../devices/keyboard_mouse/i8042.h"

/**
 *  @brief Controlo das Interrupções geradas pela Serial Port recebidas do Menu
 */
void ReceivedSPInterruptFromMenu();

/**
 *  @brief Controlo das Interrupções geradas por um dado dispositivo recebidas no Waiting Menu
 *  @param device Dispositivo que gerou as Interrupções
 */
void WaitingInterruptHandler(Device device);

/**
 *  @brief Controlo das Interrupções geradas por um dado dispositivo recebidas durante o Multi-Player
 *  @param device Dispositivo que gerou as Interrupções
 */
void MultiPlayerInterrupthandler(Device device);

/**
 *  @brief Carrega os XPMs do Multi-Player
 */
void LoadMultiPlayerXPM();

/**
 *  @brief Apresenta no ecrã o XPM do Waiting Menu
 */
void LoadWaitingMenu();

/**
 *  @brief Preparação para o Início do Multi-Player
 */
void LoadMultiPlayer();

/**
 *  @brief Apresenta no ecrã as vidas de cada Jogador
 */
void draw_hearts();

/**
 *  @brief Remove do ecrã as vidas de cada Jogador
 */
void clean_heart(int playerNum, int numLives);

/**
 *  @brief Controlo da data recebida
 *  @param auxcode Data recebida
 */
void handle_data(uint8_t auxcode);

