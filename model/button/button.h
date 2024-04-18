#pragma once

#include <lcom/lcf.h>
#include "../../devices/video_card/graphics_card.h"

#include "../../images/button/quitButton.xpm"
#include "../../images/highlightedButtons/quitButtonHighlighted.xpm"
#include "../../images/button/continueButton.xpm"
#include "../../images/highlightedButtons/continueButtonHighlighted.xpm"
#include "../../images/button/playButton.xpm"
#include "../../images/highlightedButtons/playButtonHighlighted.xpm"
#include "../../images/button/multiplayerButton.xpm"
#include "../../images/highlightedButtons/multiplayerButtonHighlighted.xpm"
#include "../../images/button/instructionsButton.xpm"
#include "../../images/highlightedButtons/instructionsButtonHighlighted.xpm"

typedef enum {BUTTON_EXIT, BUTTON_CONTINUE, BUTTON_SINGLEPLAYER, BUTTON_MULTIPLAYER, BUTTON_HELP} ButtonType;

/** @struct Button
 *  @brief Struct relativa a um Botão
 *  @var Button::x
 *  Posição dos x do Botão
 *  @var Button::y
 *  Posição dos y do Botão
 *  @var Button::buttonImg
 *  XPM do Botão Normal (Cursor Fora dele)
 *  @var Button::highlightedImg
 *  XPM do Botão Destacado (Cursor Sobre ele)
 *  @var Button::buttonType
 *  Tipo de Botão
 *  @var Button::mouseOver
 *  Boleano: 1 se o Cursor estiver sobre o Botão e 0 caso contrário
 */
typedef struct {
  unsigned int x, y;

  xpm_image_t buttonImg;
  xpm_image_t highlightedImg;

  ButtonType buttonType;

  bool mouseOver;

} Button;

/**
 *  @brief Aloca memória e cria o Botão
 *  @param buttonType Tipo do Botão a criar
 *  @param x Posição dos x do Botão
 *  @param y Posição dos y do Botão
 *  @return Botão criado
 */
Button * createButton(ButtonType buttonType, unsigned int x, unsigned int y);

/**
 *  @brief Apresenta o Botão no ecrã
 *  @param button Botão a apresentar no ecrã
 */
void draw_button(Button * button);

/**
 *  @brief Adiciona o Botão ao Fundo do Nível
 *  @param button Botão a adicionar ao Fundo do Nível
 */
void add_button_to_background(Button * button);
