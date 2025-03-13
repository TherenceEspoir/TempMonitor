/** Definitions for DHT11 temperature and humidity sensor **/

#define DTH_ERR_TIMEOUT		-1
#define DTH_ERR_CHECKSUM	-2

typedef struct{
  int temperature;
  int humidity ;
  } dht_values;

int dht_get(dht_values *values);