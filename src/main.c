#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include <string.h>

#define	NB_ROWS	2
#define	NB_COLS	16


/**
 * Permet défiler le message de gauche à droite
 * @param message
 */
void defiler_GD(char *message) {
    char temp = message[0];
    int length = strlen(message);
    for(int i = 0; i < length - 1; i++) {
        message[i] = message[i+1];
    }
    message[length-1] = temp;
}

/**
 * Permet de défiler le message de bas en haut
 * @param message
 */
void defiler_BH(char *message) {
    char temp[NB_COLS + 1];

    strncpy(temp, message, NB_COLS);
    temp[NB_COLS] = '\0';

    memmove(message, message + NB_COLS, strlen(message) - NB_COLS + 1);

    strcat(message, temp);
}

int main(void)
{
    // Chaîne à afficher et faire défiler
    char message[] = "Hello World";
    
    // Initialiser l'écran LCD
    HD44780_Initialize();
    
    // Configurer l'écran (2 lignes, police 5x8)
    HD44780_WriteCommand(0x28);
    
    // Allumer l'écran, curseur éteint, pas de clignotement
    HD44780_WriteCommand(0x0C);
    
    // Effacer l'écran
    HD44780_WriteCommand(0x01);
    _delay_ms(2); // Cette commande nécessite plus de temps
    
    while(1) {
        // Effacer la première ligne
        HD44780_WriteCommand(0x80); // Position au début de la première ligne
        for(int i = 0; i < 16; i++) {
            HD44780_WriteData(' '); // Écrire des espaces pour effacer
        }
        
        // Positionner le curseur au début de la première ligne
        HD44780_WriteCommand(0x80);
        
        // Afficher le message
        HD44780_WriteString(message);
        
        // Faire défiler le message d'un caractère
        defiler_GD(message);
        
        // Attendre avant le prochain défilement
        _delay_ms(500);
    }
    
    return 0;
}