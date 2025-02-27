#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

// Constantes d'erreur
#define DTH_ERR_TIMEOUT  -1
#define DTH_ERR_CHECKSUM -2

// Structure pour stocker uniquement la température
typedef struct {
    int temperature;
} dht_values;

// Prototype de la fonction qui lit la température du capteur.
// Retourne 0 en cas de succès, ou un code d'erreur (DTH_ERR_TIMEOUT ou DTH_ERR_CHECKSUM)
int dht_get_temperature(dht_values *values);

#endif // DHT11_H
