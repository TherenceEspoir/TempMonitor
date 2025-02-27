#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include <string.h>

int main(void)
{

    CLKSEL0 = 0b00010101;   // sélection de l'horloge externe
    CLKSEL1 = 0b00001111;   // minimum de 8Mhz
    CLKPR = (1<<CLKPCE);
    CLKPR = 0;
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);
    // Initialisation de l'écran LCD
    HD44780_Initialize();

    // Effacer l'écran (optionnel si l'initialisation le fait déjà)
    HD44780_WriteCommand(LCD_ON | CURSOR_NONE);
    HD44780_WriteCommand(LCD_CLEAR);
    HD44780_WriteCommand(LCD_HOME);
    HD44780_WriteCommand(LCD_INCR_RIGHT);

    _delay_ms(50);

    // Afficher "Hello World" sur l'écran LCD
    HD44780_WriteData('H');
    // Boucle infinie pour maintenir l'affichage
    while (1)
    {
        // Rien à faire ici, le message reste affiché
    }
    
    return 0;
}