#pragma once

#include <lcom/lcf.h>

#include "../menu/menu.h"

#include "../../images/backgrounds/menu_backgroud.xpm"
#include "../../images/backgrounds/instMenu.xpm"

void InstMenuInterruptHandler(Device device);

void draw_instructions_background();

void clean_instructions_background();
