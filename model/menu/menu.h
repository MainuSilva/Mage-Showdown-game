#pragma once

#include <lcom/lcf.h>

#include "../../devices/timer/i8254.h"
#include "../../devices/keyboard_mouse/mouse.h"
#include "../../devices/rtc/rtc.h"
#include "../game/game.h"
#include "../time/time.h"
#include "../button/button.h"

#include "../../images/backgrounds/menu_backgroud.xpm"
#include "../../images/gameLogo.xpm"

/**
 *  @brief Carrega o Fundo do Nível
 */
void load_background();

/**
 *  @brief Apresenta no ecrã o Fundo do Nível
 */
void draw_menu_background();

/**
 *  @brief Adiciona ao Fundo do Nível o XPM especificado, na posição especificada
 *  @param img XPM a adicionar ao Fundo do Nível
 *  @param x Posição dos x onde se vai adicionar o XPM
 *  @param y Posição dos y onde se vai adicionar o XPM
 */
void add_to_background(xpm_image_t * img, int x, int y);

/**
 *  @brief Carrega o Fundo do Main Menu
 *  @param firstLoad Boleano: 1 se for a primeira vez que o Main Menu é carregado, 0 caso contrário
 */
void LoadMainMenu(bool firstLoad);

/**
 *  @brief Carrega o Fundo do Pause Menu
 */
void LoadPauseMenu();

/**
 *  @brief Controlo das Interrupções de um dado dispositivo relativas ao Main Menu
 *  @param device Dispositivo que gerou interrupções
 */
void MainMenuInterruptHandler(Device device);

/**
 *  @brief Controlo das Interrupções de um dado dispositivo relativas ao Pause Menu
 *  @param device Dispositivo que gerou interrupções
 */
void PauseMenuInterruptHandler(Device device);
