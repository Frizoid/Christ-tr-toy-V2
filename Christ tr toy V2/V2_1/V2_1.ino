#include "TinyMAX7219.h"
TinyMAX7219<3, 0, 2> disp;  // CS, MOSI, SCK

// з новим роком
const uint8_t bitmap_56x7[] PROGMEM = 
{
0xC5, 0x26, 0x4A, 0x26, 0x25, 0x24, 0x40, 
0x25, 0x55, 0x4B, 0x65, 0x55, 0x56, 0xC0, 
0x25, 0x55, 0x5A, 0xA5, 0x56, 0x55, 0x40, 
0xC7, 0x56, 0x5A, 0x26, 0x56, 0x54, 0x40, 
0x25, 0x55, 0x6A, 0x24, 0x55, 0x54, 0x40, 
0x25, 0x55, 0x6A, 0x24, 0x55, 0x54, 0x40, 
0xC5, 0x26, 0x4A, 0x24, 0x25, 0x24, 0x40, 
};


const uint8_t star1[]   PROGMEM = {0x42, 0xA5, 0x42, 0x18, 0x18, 0x42, 0xA5, 0x42};
const uint8_t star2[]   PROGMEM = {0xA5, 0x0, 0xA5, 0x18, 0x18, 0xA5, 0x0, 0xA5};
const uint8_t ring1[]  PROGMEM = {0x99, 0x24, 0x42, 0x99, 0x99, 0x42, 0x24, 0x99};
const uint8_t ring2[]  PROGMEM = {0x66, 0xDB, 0xBD, 0x66, 0x66, 0xBD, 0xDB, 0x66};
const uint8_t pacm1[]  PROGMEM = {0xAA, 0xAA, 0x55, 0x55, 0xAA, 0xAA, 0x55, 0x55};
const uint8_t pacm2[]  PROGMEM = {0x55, 0x55, 0xAA, 0xAA, 0x55, 0x55, 0xAA, 0xAA};
const uint8_t cub1[] PROGMEM = {0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33};
const uint8_t cub2[] PROGMEM = {0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC};
const uint8_t heart1[] PROGMEM = {0x00, 0x66, 0x99, 0x81, 0x81, 0x42, 0x24, 0x18};
const uint8_t heart2[] PROGMEM = {0x00, 0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18};
const uint8_t cube1[]  PROGMEM = {0xff, 0x81, 0xbd, 0xa5, 0xa5, 0xbd, 0x81, 0xff};
const uint8_t cube2[]  PROGMEM = {0x00, 0x7e, 0x42, 0x5a, 0x5a, 0x42, 0x7e, 0x00};
const uint8_t bit1[] PROGMEM = {0x55, 0x80, 0x29, 0x84, 0x21, 0x94, 0x01, 0xAA};
const uint8_t bit2[] PROGMEM = {0xAA, 0x01, 0x94, 0x21, 0x84, 0x29, 0x80, 0x55};


byte offs = 0;
byte mode = 0;
byte mcount = 0;

void setup()
{
  disp.begin();
  disp.setBright(6);
}

void loop()
{
  switch (mode)
  {
    case 0: scrollBitmap(bitmap_56x7, 7, 7, 1);break;
    case 1: switchBitmap(star1, star2); break; 
    case 2: switchBitmap(pacm1, pacm2); break;
    case 3: switchBitmap(cub1, cub2); break;//blinkBitmap(heart2); break;
    case 4: switchBitmap(cube1, cube2); break;
    case 5: switchBitmap(bit1, bit2); break;
    case 6: switchBitmap(ring1, ring2); break;
    case 7: switchBitmap(heart1, heart2); break;
    case 8: scrollBitmap(bitmap_56x7, 7, 7, 1);break;
  }
  offs++;
  if (++mcount > 57)
  {
    offs = mcount = 0;
    mode++;  // mode = rnd();
    if (mode > 8) mode = 0;
    disp.clear();
  }
  delay(300);
}

void blinkBitmap(const uint8_t* bmap)
{
  static bool blink;
  blink = !blink;
  if (blink) drawBitmap(bmap);
  else disp.clear();
}

void switchBitmap(const uint8_t* bmap1, const uint8_t* bmap2)
{
  static bool blink;
  blink = !blink;
  if (blink) drawBitmap(bmap1);
  else drawBitmap(bmap2);
}

void drawBitmap(const uint8_t* bmap)
{
  for (byte i = 0; i < 8; i++)
  {
    disp.buf[i] = pgm_read_byte(&bmap[i]);
  }
  disp.update();
}

void scrollBitmap(const uint8_t* bmap, byte w, byte h, byte p)
{
  if (offs > (w << 3)) offs = 0;
  for (byte i = 0; i < h; i++)
  {
    byte num = (offs >> 3) + w * i;
    byte left = offs & 7;
    byte val1 = pgm_read_byte(&bmap[num - 1]);
    byte val2 = pgm_read_byte(&bmap[num]);
    if (offs < 8) val1 = val2, val1 = 0;
    if (left == 0) disp.buf[i + p] = val1;
    else disp.buf[i + p] = ((val1 << left) | (val2 >> (8 - left)));
  }
  disp.update();
}

void snow()
{
  for (byte i = 0; i < 7; i++)
  {
    disp.buf[7 - i] = disp.buf[7 - i - 1];
    if (rnd() < 2) disp.buf[7 - i] >>= 1;
    if (rnd() > 5) disp.buf[7 - i] <<= 1;
  }
  disp.buf[0] = bit(rnd());
  disp.update();
}

byte rnd()
{
  static uint16_t seed;
  seed = (seed * 2053ul) + 13849;
  return (seed >> 8) & 7;
}
