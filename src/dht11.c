#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dht11.h"

#define DTH_DDR		DDRF
#define DTH_PORT	PORTF
#define DTH_PIN		PINF
#define DTH_IO		4

#define DTH_ANSWER_SIZE	40
#define DTH_TIMEOUT	(F_CPU/40000)

#define DTH_WAIT_LOW_MS		20
#define DTH_WAIT_HIGH_US	40
#define DTH_LOOP_CYCLES		2
#define DTH_HIGH_DELAY_US	40	

static void dht_trigger(void){
  uint8_t mask=(1<<DTH_IO);
  DTH_DDR |= mask;
  DTH_PORT &= ~mask;
  _delay_ms(DTH_WAIT_LOW_MS);
  DTH_PORT |= mask;
  _delay_us(DTH_WAIT_HIGH_US);
  DTH_DDR &= ~mask;
}

int dht_get(dht_values *values){
  uint8_t bits[DTH_ANSWER_SIZE/8];
  int i,l;
  // Trigger answer
  dht_trigger();
  // Check acknowledgment
  uint8_t mask=(1<<DTH_IO);
  for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)==0;l--); if(l<0) return DTH_ERR_TIMEOUT;
  for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)!=0;l--); if(l<0) return DTH_ERR_TIMEOUT;
  // Read values
  for(i=0;i<DTH_ANSWER_SIZE;i++){
    if(i%8==0) bits[i/8]=0;
    for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)==0;l--); if(l<0) return DTH_ERR_TIMEOUT;
    for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)!=0;l--); if(l<0) return DTH_ERR_TIMEOUT;
    uint32_t time=l*DTH_LOOP_CYCLES/(F_CPU/1000000L);
    bits[i/8] <<= 1;
    if(time<DTH_HIGH_DELAY_US) bits[i/8] |= 0x01;
    }
  // Verify checksum
  uint8_t sum=bits[0]+bits[2];
  if(sum!=bits[4]) return DTH_ERR_CHECKSUM;
  // Convert values
  values->humidity=bits[0];
  values->temperature=bits[2];
  return 0;
}

/*
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dht11.h"

#define DTH_DDR		DDRF
#define DTH_PORT	PORTF
#define DTH_PIN		PINF
#define DTH_IO		4

#define DTH_ANSWER_SIZE	40
#define DTH_TIMEOUT	(F_CPU/40000)

#define DTH_WAIT_LOW_MS		20
#define DTH_WAIT_HIGH_US	40
#define DTH_LOOP_CYCLES		2
#define DTH_HIGH_DELAY_US	40	

static void dht_trigger(void){
uint8_t mask=(1<<DTH_IO);
DTH_DDR |= mask;
DTH_PORT &= ~mask;
_delay_ms(DTH_WAIT_LOW_MS);
DTH_PORT |= mask;
_delay_us(DTH_WAIT_HIGH_US);
DTH_DDR &= ~mask;
}

int dht_get(dht_values *values){
uint8_t bits[DTH_ANSWER_SIZE/8];
int i,l;
// Trigger answer
dht_trigger();
// Check acknowledgment
uint8_t mask=(1<<DTH_IO);
for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)==0;l--); if(l<0) return DTH_ERR_TIMEOUT;
for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)!=0;l--); if(l<0) return DTH_ERR_TIMEOUT;
// Read values
for(i=0;i<DTH_ANSWER_SIZE;i++){
  if(i%8==0) bits[i/8]=0;
  for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)==0;l--); if(l<0) return DTH_ERR_TIMEOUT;
  for(l=DTH_TIMEOUT;l>=0 && (DTH_PIN&mask)!=0;l--); if(l<0) return DTH_ERR_TIMEOUT;
  uint32_t time=l*DTH_LOOP_CYCLES/(F_CPU/1000000L);
  bits[i/8] <<= 1;
  if(time<DTH_HIGH_DELAY_US) bits[i/8] |= 0x01;
  }
// Verify checksum
uint8_t sum=bits[0]+bits[2];
if(sum!=bits[4]) return DTH_ERR_CHECKSUM;
// Convert values
values->humidity=bits[0];
values->temperature=bits[2];
return 0;
}
*/