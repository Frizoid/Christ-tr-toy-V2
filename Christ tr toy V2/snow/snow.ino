#include "TinyMAX7219.h"
TinyMAX7219<3, 0, 2> disp;  // CS, MOSI, SCK

// merry christmas
const uint8_t bitmap_64x7[] PROGMEM = 
{
0x8B, 0xB3, 0x28, 0x95, 0x92, 0x74, 0x48, 0x80, 
0xDA, 0x2A, 0xA9, 0x55, 0x55, 0x26, 0xD5, 0x40, 
0xAA, 0x2A, 0xA9, 0x15, 0x54, 0x25, 0x55, 0x0, 
0x8B, 0xB3, 0x11, 0x1D, 0x92, 0x24, 0x5C, 0x80, 
0x8A, 0x2A, 0x91, 0x15, 0x51, 0x24, 0x54, 0x40, 
0x8A, 0x2A, 0x91, 0x55, 0x55, 0x24, 0x55, 0x40, 
0x8B, 0xAA, 0x90, 0x95, 0x52, 0x24, 0x54, 0x80,
};

// з новим роком
const uint8_t bitmap_59x5[] PROGMEM = {
	0x65, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 
  0x85, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
  0x47, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 
  0x85, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
  0x65, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
};


const uint8_t star[]   PROGMEM = {0x99, 0x5a, 0x3c, 0xff, 0xff, 0x3c, 0x5a, 0x99};
const uint8_t ring1[]  PROGMEM = {0x3c, 0x66, 0x99, 0xa5, 0xa5, 0x99, 0x66, 0x3c};
const uint8_t ring2[]  PROGMEM = {0x81, 0x3c, 0x24, 0x5a, 0x5a, 0x24, 0x3c, 0x81};
const uint8_t pacm1[]  PROGMEM = {0x3c, 0x7e, 0xdf, 0xff, 0xf0, 0xff, 0x7e, 0x3c};
const uint8_t pacm2[]  PROGMEM = {0x3c, 0x7e, 0xdc, 0xf8, 0xf8, 0xfc, 0x7e, 0x3c};
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
    case 0: snow(); break;
    case 1: snow(); break; 
    case 2: snow(); break;
    case 3: snow(); break;
    case 4: snow(); break;
    case 5: snow(); break;
    case 6: snow(); break;
    case 7: snow(); break;
    case 8: snow(); break;
  }
  offs++;
  if (++mcount > 10000)
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
