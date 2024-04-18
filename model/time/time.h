#pragma once

#include <lcom/lcf.h>

#include "../../devices/rtc/rtc.h"

#include "../menu/menu.h"


#include "../../images/numbers/slash.xpm"
#include "../../images/numbers/0.xpm"
#include "../../images/numbers/1.xpm"
#include "../../images/numbers/2.xpm"
#include "../../images/numbers/3.xpm"
#include "../../images/numbers/4.xpm"
#include "../../images/numbers/5.xpm"
#include "../../images/numbers/6.xpm"
#include "../../images/numbers/7.xpm"
#include "../../images/numbers/8.xpm"
#include "../../images/numbers/9.xpm"
#include "../../images/numbers/colon.xpm"

/** @struct DateTime
 *  @brief Struct relativa à Data e Hora do RTC
 *  @var DateTime::img
 *  Array dos XPMs dos Dígitos e da Pontuação usada no Relógio
 *  @var DateTime::second
 *  Segundo atual
 *  @var DateTime::minute
 *  Minuto atual
 *  @var DateTime::hour
 *  Hora atual
 *  @var DateTime::day
 *  Dia atual
 *  @var DateTime::month
 *  Mês atual
 *  @var DateTime::year
 *  Ano atual
 */
typedef struct {
  xpm_image_t img[12]; //     : -> img[10]     / -> img[11]

  uint8_t second, minute, hour, day, month, year;

} DateTime;

/**
 *  @brief Aloca memória e cria a DateTime
 *  @return DateTime criada
 */
DateTime * LoadDateTime();

/**
 *  @brief Devolve o XPM do dígito a utilizar
 *  @return XPM do dígito a utilizar
 */
xpm_image_t * decide_digit_xpm();

/**
 *  @brief Apresente no ecrã um dígito
 *  @param img XPM do dígito a apresentar no ecrã
 *  @param x Posição dos x do Dígito
 *  @param y Posição dos y do Dígito
 */
void draw_digit(xpm_image_t * img, int x, int y);

/**
 *  @brief Remove do ecrã um dígito
 *  @param img XPM do dígito a remover do ecrã
 *  @param x Posição dos x do Dígito
 *  @param y Posição dos y do Dígito
 */
void clean_digit(xpm_image_t * img, int x, int y);

/**
 *  @brief Adicionar a Data ao Fundo do Nível
 */
void add_date_to_background();

/**
 *  @brief Adicionar o Relógio ao Fundo do Nível
 */
void add_time_to_background();

/**
 *  @brief Apresenta a Data no ecrã
 */
void draw_date();

/**
 *  @brief Apresenta o Relógio no ecrã
 */
void draw_time();

/**
 *  @brief Apresenta a Data e o Relógio no ecrã
 */
void draw_clock();

/**
 *  @brief Remove o Relógio do ecrã
 */
void clean_time();

/**
 *  @brief Remove o Relógio do ecrã
 */
void clean_clock();

/**
 *  @brief Apresenta o Relógio no ecrã (em Modo de Texto)
 */
void print_clock();

/**
 *  @brief Atualiza o Relógio com os dados do RTC
 */
void updateDateTime();
