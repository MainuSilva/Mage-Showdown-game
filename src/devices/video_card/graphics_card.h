#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;


/**
 *  @brief Mete a Placa Gráfica no Modo especificado
 *  @param mode Modo a utilizar na Placa Gráfica
 *  @return 0 em caso de sucesso, 1 caso contrário
 */
int (set_graphics_card_mode)(uint16_t mode);

/**
 *  @brief Muda a cor de um pixel para a cor especificada
 *  @param x Posição dos x do pixel a alterar
 *  @param y Posição dos y do pixel a alterar
 *  @param color Nova Cor do pixel
 *  @return 0 em caso de sucesso, 1 caso contrário
 */
int (changePixelColor)(uint16_t x, uint16_t y, uint32_t color);

/**
 *  @brief Implementação da técnica Double-Buffering
 */
void (double_buffer)();

/**
 *  @brief Libertação da memória utilizada pela placa gráfica
 */
void free_memory();

