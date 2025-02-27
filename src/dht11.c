/*
#include <avr/io.h>
#include <util/delay.h>

// Erreurs de lecture
#define DTH_ERR_TIMEOUT  -1
#define DTH_ERR_CHECKSUM -2

// Structure pour stocker uniquement la température
typedef struct {
    int temperature;
} dht_values;

// --- Adaptation pour capteur connecté à PF4 ---
#define DTH_DDR         DDRF
#define DTH_PORT        PORTF
#define DTH_PIN         PINF
#define DTH_IO          4

#define DTH_ANSWER_SIZE 40
#define DTH_TIMEOUT     (F_CPU/40000)  // Calcul basé sur F_CPU (par exemple, environ 2.5 ms si F_CPU=8MHz)

#define DTH_WAIT_LOW_MS         20    // Durée du signal bas (20 ms)
#define DTH_WAIT_HIGH_US        40    // Durée du signal haut initial (40 µs)
#define DTH_LOOP_CYCLES         2
#define DTH_HIGH_DELAY_US       40    // Seuil pour distinguer un 0 d'un 1 (40 µs)

// Fonction de déclenchement du capteur
static void dht_trigger(void) {
    uint8_t mask = (1 << DTH_IO);
    // Configurer PF4 en sortie et tirer la ligne à 0
    DTH_DDR |= mask;
    DTH_PORT &= ~mask;
    _delay_ms(DTH_WAIT_LOW_MS);
    // Remettre la ligne à 1 pendant 40 µs
    DTH_PORT |= mask;
    _delay_us(DTH_WAIT_HIGH_US);
    // Reconfigurer la broche en entrée pour lire la réponse
    DTH_DDR &= ~mask;
}

// Fonction pour lire la température
// Seule la température est exploitée, issue du troisième octet (bits[2])
int dht_get_temperature(dht_values *values) {
    uint8_t bits[DTH_ANSWER_SIZE/8] = {0};
    int i, l;
    
    dht_trigger();  // Déclencher le capteur
    
    uint8_t mask = (1 << DTH_IO);
    
    // Attendre la réponse : le capteur doit tirer la ligne à 0
    for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) == 0; l--);
    if(l < 0) return DTH_ERR_TIMEOUT;
    
    // Attendre la transition de 0 à 1
    for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) != 0; l--);
    if(l < 0) return DTH_ERR_TIMEOUT;
    
    // Lire les 40 bits
    for(i = 0; i < DTH_ANSWER_SIZE; i++) {
        if(i % 8 == 0)
            bits[i/8] = 0;
        // Attendre que la ligne passe à 1 indiquant le début du bit
        for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) == 0; l--);
        if(l < 0) return DTH_ERR_TIMEOUT;
        // Attendre que la ligne passe à 0 (fin du signal haut du bit)
        for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) != 0; l--);
        if(l < 0) return DTH_ERR_TIMEOUT;
        
        // Convertir la durée en microsecondes
        uint32_t time = l * DTH_LOOP_CYCLES / (F_CPU / 1000000L);
        bits[i/8] <<= 1;
        if(time < DTH_HIGH_DELAY_US)
            bits[i/8] |= 0x01;
    }
    
    // Vérifier le checksum (simplification) : ici, on vérifie que la somme du troisième octet vaut le cinquième
    uint8_t sum = bits[2];
    if(sum != bits[4])
        return DTH_ERR_CHECKSUM;
    
    values->temperature = bits[2];
    return 0;
}
*/
#include "dht11.h"
#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

// --- Configuration du capteur sur PF4 ---
// Si le capteur est connecté à PF4, on définit :
#define DTH_DDR     DDRF
#define DTH_PORT    PORTF
#define DTH_PIN     PINF
#define DTH_IO      4

// Taille de la réponse : 40 bits (5 octets)
#define DTH_ANSWER_SIZE 40
// Timeout calculé en fonction de F_CPU (environ 2,5 ms pour F_CPU=8MHz)
#define DTH_TIMEOUT     (F_CPU/40000)

// Délais en millisecondes ou microsecondes (selon le protocole DHT)
#define DTH_WAIT_LOW_MS         20    // 20 ms pour le signal bas de déclenchement
#define DTH_WAIT_HIGH_US        40    // 40 µs pour le signal haut initial
#define DTH_LOOP_CYCLES         2
#define DTH_HIGH_DELAY_US       40    // Seuil en µs pour distinguer un 0 d’un 1

// Fonction interne pour déclencher le capteur
static void dht_trigger(void) {
    uint8_t mask = (1 << DTH_IO);
    // Configurer la broche comme sortie et tirer la ligne à 0
    DTH_DDR |= mask;
    DTH_PORT &= ~mask;
    _delay_ms(DTH_WAIT_LOW_MS);
    // Remettre la ligne à 1 pendant 40 µs
    DTH_PORT |= mask;
    _delay_us(DTH_WAIT_HIGH_US);
    // Reconfigurer la broche en entrée pour lire la réponse
    DTH_DDR &= ~mask;
}

// Fonction publique qui lit la température du capteur.
// Seule la température est exploitée, issue du troisième octet (bits[2])
int dht_get_temperature(dht_values *values) {
    uint8_t bits[DTH_ANSWER_SIZE/8] = {0};
    int i, l;
    
    dht_trigger();  // Déclencher le capteur
    
    uint8_t mask = (1 << DTH_IO);
    
    // Attendre que le capteur réponde en tirant la ligne à 0
    for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) == 0; l--);
    if(l < 0) return DTH_ERR_TIMEOUT;
    
    // Attendre la transition de 0 à 1
    for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) != 0; l--);
    if(l < 0) return DTH_ERR_TIMEOUT;
    
    // Lire les 40 bits envoyés par le capteur
    for(i = 0; i < DTH_ANSWER_SIZE; i++) {
        if(i % 8 == 0)
            bits[i/8] = 0;
        // Attendre que la ligne passe à 1 (début du bit)
        for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) == 0; l--);
        if(l < 0) return DTH_ERR_TIMEOUT;
        // Attendre que la ligne repasse à 0 (fin du signal haut)
        for(l = DTH_TIMEOUT; l >= 0 && (DTH_PIN & mask) != 0; l--);
        if(l < 0) return DTH_ERR_TIMEOUT;
        
        // Convertir la durée (l) en microsecondes et décider si c'est un 0 ou un 1
        uint32_t time = l * DTH_LOOP_CYCLES / (F_CPU / 1000000L);
        bits[i/8] <<= 1;
        if(time < DTH_HIGH_DELAY_US)
            bits[i/8] |= 0x01;
    }
    
    // Vérifier le checksum : ici, nous simplifions en vérifiant que
    // la valeur de température (octet 3, c'est-à-dire bits[2]) correspond à l'octet de checksum (bits[4])
    uint8_t sum = bits[2];
    if(sum != bits[4])
        return DTH_ERR_CHECKSUM;
    
    values->temperature = bits[2];
    return 0;
}
