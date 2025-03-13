#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include "dht11.h"
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


    /* Variables pour le capteur
    dht_values sensor_values;
    int result;
    char temp_str[16] = "Temp: -- C";
    int counter = 0;
  */
    
    // Initialiser l'écran LCD
    HD44780_Initialize();
    
    // Configurer l'écran (2 lignes, police 5x8)
    HD44780_WriteCommand(0x28);
    
    HD44780_WriteCommand(0x0C); // Affichage ON, curseur OFF
    HD44780_WriteCommand(0x01); // Effacer l'écran

    _delay_ms(10);

   // _delay_ms(2); // Cette commande nécessite plus de temps
    
    /*
    while(1) {
        // Tous les 10 cycles, lire la température
        if (counter % 10 == 0) {
        result = dht_get(&sensor_values);
        
        if (result == 0) {
            sprintf(temp_str, "Temp: %d C", sensor_values.temperature);
        } else if (result == DTH_ERR_TIMEOUT) {
            sprintf(temp_str, "Erreur timeout");
        } else if (result == DTH_ERR_CHECKSUM) {
            sprintf(temp_str, "Erreur checksum");
        }
        }
        
        //Effacer l'écran
        HD44780_WriteCommand(0x01);  // Commande CLEAR
        _delay_ms(2);

        // Positionner le curseur en début de première ligne (adresse 0x80)
        HD44780_WriteCommand(0x80);

        // Afficher la température (ou le message d'erreur)
        HD44780_WriteString(temp_str);
        
        // Attendre 2 secondes avant la prochaine lecture
        _delay_ms(2000); 
        counter++;
    }*/

    /*
    while(1)
    {
        // Lire la température du capteur DHT
        result = dht_get_temperature(&sensor_values);
        if(result == 0) {
            // Lecture réussie : formater la chaîne "Temp: XX C"
            sprintf(temp_str, "Temp: %d C", sensor_values.temperature);
        } else if(result == DTH_ERR_TIMEOUT) {
            sprintf(temp_str, "Timeout Err");
        } else if(result == DTH_ERR_CHECKSUM) {
            sprintf(temp_str, "Checksum Err");
        }
        
        // Effacer l'écran
        HD44780_WriteCommand(0x01);  // Commande CLEAR
        _delay_ms(2);

        // Positionner le curseur en début de la première ligne (adresse 0x80)
        HD44780_WriteCommand(0x80);
        
        // Afficher la température ou le message d'erreur
        HD44780_WriteString(temp_str);
        
        // Attendre 2 secondes avant la prochaine lecture
        _delay_ms(2000);
    }
    */

    //Test capteur température
    dht_values values;
    int result=dht_get(&values);
    int address;

    if(result<0){
        HD44780_WriteString("Erreur : ");
        HD44780_WriteInteger(result,10);
        }
      else{
        HD44780_WriteString("Temper. : ");
        HD44780_WriteInteger(values.temperature,10);
        HD44780_WriteString("C");
        address=HD44780_XY2Adrr(NB_ROWS,NB_COLS,1,0);
        HD44780_WriteCommand(LCD_ADDRSET|address);
        HD44780_WriteString("Humidité : ");
        HD44780_WriteInteger(values.humidity,10);
        HD44780_WriteString("%");
        }
    return 0;
}