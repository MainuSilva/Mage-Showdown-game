#include "graphics_card.h"

static void *video_mem;  /* frame-buffer VM address(static global variable)*/ 
static void *doubleBuffer;
static mmap_t mem_map;

struct minix_mem_range mr, m2;

unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */

unsigned int hres;
unsigned int vres;
unsigned int numBytesPerPixel;

vbe_mode_info_t mode_info;

int r;				    

int (set_graphics_card_mode)(uint16_t mode) {

  mem_map.phys = 0;
  mem_map.size = 1024 * 1024;
  lm_alloc(mem_map.size, &mem_map);

  if (vbe_get_mode_info(mode, &mode_info) != OK) {return 1;}


  vram_base = mode_info.PhysBasePtr;
  vram_size = mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel + 7) / 8;
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  numBytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  doubleBuffer = malloc(vram_size);

  reg86_t r86;

  memset(&r86, 0, sizeof(reg86_t));	/* zero the structure */

  r86.ax = 0x4F02;
  r86.bx = 1<<14|mode; // set bit 14: linear framebuffer
  r86.intno = 0x10;

  if (sys_int86(&r86) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  lm_free(&mem_map);

  return 0;
}

int (changePixelColor)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t * ptr;

  ptr = (uint8_t*)doubleBuffer + (x * numBytesPerPixel) + (y * hres * numBytesPerPixel);

  uint8_t temp;

  for (unsigned int i = 0; i < numBytesPerPixel; i++) {
    temp = color & 0xFF;
    *(ptr + i) = temp;
    color = color >> 8;
  }

  return 0;
}

void (double_buffer)() {
  memcpy(video_mem, doubleBuffer, vres * hres * numBytesPerPixel);
}

void free_memory() {
  lm_free(&mem_map);
  free(doubleBuffer);
}

