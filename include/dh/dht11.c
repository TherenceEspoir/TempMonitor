#include "dht11.h"
#include <avr/io.h>
#include <util/delay.h>

void dht11_init(void) {
    // Configurer la broche comme sortie initialement (pour le signal de démarrage)
    DHT11_DDR |= (1 << DHT11_PIN);
    // Mettre la broche à l'état haut
    DHT11_PORT |= (1 << DHT11_PIN);
}

// Lecture de la température du DHT11
bool dht11_read_temperature(dht11_temp_t *data) {
    uint8_t bits[5] = {0}; // Buffer pour stocker les 40 bits (5 octets)
    uint8_t i, j;
    
    // Signal de démarrage
    DHT11_DDR |= (1 << DHT11_PIN);     // Configurer comme sortie
    DHT11_PORT &= ~(1 << DHT11_PIN);   // Mettre à 0
    _delay_ms(20);                     // Attendre au moins 18ms
    DHT11_PORT |= (1 << DHT11_PIN);    // Mettre à 1
    _delay_us(30);                     // Attendre 20-40µs
    
    // Configurer comme entrée pour lire la réponse
    DHT11_DDR &= ~(1 << DHT11_PIN);
    
    // Attendre la réponse du DHT11
    _delay_us(40);
    
    // Vérifier le bit de début de la réponse
    if ((DHT11_PIN_REG & (1 << DHT11_PIN)) != 0) {
        return false; // Pas de réponse
    }
    
    _delay_us(80); // Attendre que le signal bas se termine
    
    // Vérifier le bit haut de début
    if ((DHT11_PIN_REG & (1 << DHT11_PIN)) == 0) {
        return false; // Pas de réponse correcte
    }
    
    _delay_us(80); // Attendre que le signal haut se termine
    
    // Lire les 40 bits (5 octets)
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            // Attendre le front descendant
            while ((DHT11_PIN_REG & (1 << DHT11_PIN)) != 0);
            _delay_us(30); // Attendre 50µs
            
            // Vérifier la durée du signal haut
            if ((DHT11_PIN_REG & (1 << DHT11_PIN)) != 0) {
                // C'est un bit 1
                bits[i] |= (1 << (7 - j));
            }
            
            // Attendre la fin du bit(Attendre que le signal se termine, retour  à 0)
            while ((DHT11_PIN_REG & (1 << DHT11_PIN)) == 0);
        }
    }
    
    // Vérifier la somme de contrôle
    if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
        data->temperature = bits[2];  // Seule la température nous intéresse
        data->valid = true;
        return true;
    } else {
        data->valid = false;
        return false;
    }
}
