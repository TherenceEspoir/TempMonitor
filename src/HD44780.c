#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "HD44780_private.h"
#include "HD44780.h"

static void HD44780_WriteNibble(const uint8_t nib)
{
  LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
  if(nib&0x08) LCD_D7_PORT |= (1<<LCD_D7_PIN);
  else LCD_D7_PORT &= ~(1<<LCD_D7_PIN);
  if(nib&0x04) LCD_D6_PORT |= (1<<LCD_D6_PIN);
  else LCD_D6_PORT &= ~(1<<LCD_D6_PIN);
  if(nib&0x02) LCD_D5_PORT |= (1<<LCD_D5_PIN);
  else LCD_D5_PORT &= ~(1<<LCD_D5_PIN);
  if(nib&0x01) LCD_D4_PORT |= (1<<LCD_D4_PIN);
  else LCD_D4_PORT &= ~(1<<LCD_D4_PIN);
  _delay_us(1);
  LCD_EN_PORT |= (1<<LCD_EN_PIN);
  _delay_us(1);
  LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
  _delay_us(100);
}

static void HD44780_WriteByte(const uint8_t c)
{
  HD44780_WriteNibble(c>>4);
  HD44780_WriteNibble(c&0x0F);
}

static void HD44780_PowerUp4Bit(void)
{
  /* Wait for more than 40 ms after VCC rises to 2.7 V */
  _delay_ms(40);
  HD44780_WriteNibble(0x03);        // FN_SET 8-bit

  /* Wait for more than 4.1 ms */
  _delay_ms(5);
  HD44780_WriteNibble(0x03);        // FN_SET 8-bit

  /* Wait for more than 100 µs */
  _delay_us(100);
  HD44780_WriteNibble(0x03);        // FN_SET 8-bit

  /* From now on we must allow 40us for each command */
  _delay_us(50);
  HD44780_WriteNibble(0x02);        // FN_SET 4-bit

  /* The LCD is now in 4-bit mode so we can continue
     using the 4-bit API */
  _delay_us(50);
}

void HD44780_Initialize(void)
{
  LCD_D4_DDR |= (1<<LCD_D4_PIN);
  LCD_D5_DDR |= (1<<LCD_D5_PIN);
  LCD_D6_DDR |= (1<<LCD_D6_PIN);
  LCD_D7_DDR |= (1<<LCD_D7_PIN);
  LCD_RS_DDR |= (1<<LCD_RS_PIN);
  LCD_RW_DDR |= (1<<LCD_RW_PIN);
  LCD_EN_DDR |= (1<<LCD_EN_PIN);
  LCD_D4_PORT &= ~(1<<LCD_D4_PIN);
  LCD_D5_PORT &= ~(1<<LCD_D5_PIN);
  LCD_D6_PORT &= ~(1<<LCD_D6_PIN);
  LCD_D7_PORT &= ~(1<<LCD_D7_PIN);
  LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
  LCD_RW_PORT &= ~(1<<LCD_RW_PIN);
  LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
  HD44780_PowerUp4Bit();
  _delay_ms(50);
}

void HD44780_WriteCommand(const uint8_t c)
{
  LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
  HD44780_WriteByte(c);
  _delay_us(50);
}

void HD44780_WriteData(const uint8_t c)
{
  LCD_RS_PORT |= (1<<LCD_RS_PIN);
  HD44780_WriteByte(c);
  LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
  _delay_us(50);
}

int HD44780_XY2Adrr(int nbrows,int nbcols,int row,int col)
{
  int row_offsets[]={0x00,0x40,0x14,0x54};
  if(row>=nbrows) row=nbrows-1;
  if(col>=nbcols) col=nbcols-1;
  return row_offsets[row]+col;
}
