#ifndef DHT11_H
#define DHT11_H
#define DHT11_PIN  PF4
#define DHT11_PIN_REG PINF
#include <stdint.h>
#include <stdbool.h>

//Structure de donnée pour stocker la temperature
typedef struct {
    uint8_t temperature;
    bool valid;
} dht11_temp_t;

// Initialisation du capteur DHT11
void dht11_init(void);

// Lecture de la température du capteur
// Retourne true si la lecture est réussie
bool dht11_read_temperature(dht11_temp_t *data);

#endif