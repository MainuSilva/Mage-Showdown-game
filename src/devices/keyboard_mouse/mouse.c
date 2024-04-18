#include "mouse.h"

int hook_id_mouse;
uint8_t scancode;
uint8_t stat;

extern unsigned int hres, vres;
extern xpm_image_t background_menu;

uint16_t convert_2_complement(uint8_t number, uint8_t msb, struct packet * pp){

  if (pp->bytes[0] & msb)
    return number - 256;
  return number;

}

bool (check_input_buffer)() {
  uint8_t stat;

  if (utils_sys_inb(KBC_CMD_REG, &stat) != 0) {return false;}

  if (stat & IBF) 
    return false;
  else
    return true;
}

bool (check_output_buffer)() {
  uint8_t stat;

  if (utils_sys_inb(KBC_CMD_REG, &stat) != 0) {return false;}

  if (stat & OBF) {
    if ((stat & (PAR_ERR | TO_ERR)) != 0)
      return false;
    else
      return true;
  }
  else
    return false;
  
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id_mouse = MOUSE_IRQ;
  *bit_no = hook_id_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_mouse) != 0) {return 3;}

  return 0;
}

int (mouse_unsubscribe_int)() {
  uint8_t temp;

  if (sys_irqrmpolicy(&hook_id_mouse) != 0) {return 3;}

  if (utils_sys_inb(KBC_CMD_REG, &stat) != 0) return 4;

  if (stat & OBF)
    if (utils_sys_inb(OUT_BUF, &temp) != 0) return 4;

  return 0;
}

void (mouse_ih)(){
  while (1) {
    if (check_output_buffer()) {
      if (utils_sys_inb(WRITE_CMD_BYTE, &scancode) != 0) {scancode = 0x00;}
      break;
    }
    else {
      scancode = 0x00;
      break;
    }
  }
  
}

void (buildPacket)(struct packet * pp) {
  pp->lb = pp->bytes[0] & LB;
  pp->rb = pp->bytes[0] & RB;
  pp->mb = pp->bytes[0] & MB;
  pp->x_ov = pp->bytes[0] & X_OV;
  pp->y_ov = pp->bytes[0] & Y_OV;
  pp->delta_x = convert_2_complement(pp->bytes[1], DELTA_X, pp);
  pp->delta_y = convert_2_complement(pp->bytes[2], DELTA_Y, pp);

}

int (mouse_enable_data_report)() {
  uint8_t resp;
  uint8_t secondError = 0;

  do {
    if (sys_outb(KBC_CMD_REG, WRITE_B_MOUSE) != 0) {return 1;}

    if (mouse_write_cmd(ENA_DATA_REP, &resp) != 0) {return 1;}

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {secondError++;}
    else if (resp == ERROR){return 1;}
    
  } while (secondError < 2);

  return 1;
}

int (mouse_disable_data_report)() {
  uint8_t resp;
  uint8_t secondError = 0;

  do {
    if (sys_outb(IN_BUF, WRITE_MOUSE) != 0) {return 1;}

    if (mouse_write_cmd(DISABLE_DATA_REPORT, &resp) != 0) {return 1;}

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {secondError++;}
    else if (resp == ERROR){return 1;}
    
  } while (secondError < 2);

  return 1;
}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  int count = 0;

  while (count < 3) {

    if (check_input_buffer()) {

      if (sys_outb(WRITE_CMD_BYTE, cmd) != 0) {return 1;}

      if (utils_sys_inb(OUT_BUF, resp) != 0) {return 1;}

      return 0;
    }
    
    count++;
  }

  return 1;
}

int (mouse_stream)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  do {
    if (sys_outb(IN_BUF, WRITE_CMD_BYTE) != 0) {return 1;}

    if (mouse_write_cmd(STREAM_MODE, &resp) != 0) {return 1;}

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
    
  } while (seconderror < 2);

  return 1;
}

int (mouse_remote)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  do {
    if (sys_outb(IN_BUF, WRITE_CMD_BYTE) != 0) {return 1;}

    if (mouse_write_cmd(REMOTE_MODE, &resp) != 0) {return 1;}

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
    
  } while (seconderror < 2);

  return 1;
}

int (mouse_read_data)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  do {
    if (sys_outb(IN_BUF, WRITE_CMD_BYTE) != 0) {return 1;}

    if (mouse_write_cmd(READ_DATA, &resp) != 0) {return 1;}

    if (resp == ACK) {mouse_ih(); return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
    
  } while (seconderror < 2);

  return 1;
}


Mouse *mouse;

Mouse * load_mouse() {
  mouse = (Mouse *) malloc(sizeof(Mouse));

  xpm_load((xpm_map_t) mouse_xpm, XPM_8_8_8_8, &mouse->img);

  mouse->x = 30;
  mouse->y = 30;

  return mouse;
}

void mouse_update(struct packet * pp) {

  clean_mouse();

  if (pp->delta_x > 0) {
    if (mouse->x + pp->delta_x > (int)hres - mouse->img.width)
      mouse->x = (int)hres - mouse->img.width;
    else
      mouse->x += pp->delta_x;
  }
  else if (pp->delta_x < 0) {
    if (mouse->x + pp->delta_x < 0)
      mouse->x = 0;
    else
      mouse->x += pp->delta_x;
  }
  if (pp->delta_y < 0) {
    if (mouse->y + mouse->img.height - pp->delta_y > (int)vres)
      mouse->y = (int)vres - mouse->img.height;
    else
      mouse->y -= pp->delta_y;
  }
  else if (pp->delta_y > 0) {
    if (mouse->y - pp->delta_y < 0)
      mouse->y = 0;
    else
      mouse->y -= pp->delta_y;
  }

  draw_mouse_mouse();

}

void draw_mouse_mouse() {
  uint32_t* map = (uint32_t*) mouse->img.bytes;

  for(int i = 0; i < mouse->img.width; i++) {
        for (int j = 0; j < mouse->img.height; j++) {
          if (*(map + i + j*mouse->img.width) != xpm_transparency_color(XPM_8_8_8_8))
            changePixelColor(mouse->x+i,mouse->y+j,*(map + i + j*mouse->img.width));
    }
  }
}

void clean_mouse() {
  uint32_t* map = (uint32_t*) background_menu.bytes;

  for (int i = mouse->x; i <= mouse->x + mouse->img.width; i++) {
    for (int j = mouse->y; j <= mouse->y + mouse->img.height; j++) {
      if (i < (int)hres - 1 && j < (int)vres - 1)
        changePixelColor(i,j,*(map + i + j * hres));
    }
  }
}

unsigned int check_collision_main_menu () {
  if (mouse->x > 400 && mouse->x < 574 && mouse->y > 300 && mouse->y < 371)
    return 3;
  else if (mouse->x > 360 && mouse->x < 608 && mouse->y > 375 && mouse->y < 430)
    return 4;  
  else if (mouse->x > 350 && mouse->x < 623 && mouse->y > 435 && mouse->y < 506)
    return 5;
  else if (mouse->x > 405 && mouse->x < 579 && mouse->y > 510 && mouse->y < 581)
    return 1;
  else return 0;
}

unsigned int check_collision_pause_menu () {
  if (mouse->x > 320 && mouse->x < 640 && mouse->y > 400 && mouse->y < 464)
    return 2;
  else if (mouse->x > 320 && mouse->x < 640 && mouse->y > 592 && mouse->y < 656)
    return 1;
  else return 0;
}

unsigned int check_collision_won_menu() {
  if (mouse->x > 320 && mouse->x < 640 && mouse->y > 500 && mouse->y < 564)
    return 1;
  else return 0;
}
