#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // 1) Configurer les LED en sortie
    DDRE |= (1 << PE6);  // LED1 sur PE6
    DDRF |= (1 << PF1);  // LED2 sur PF1
    
    // 2) Configurer les boutons en entrée avec pull-up
    DDRC &= ~((1 << PC7) | (1 << PC6));  // PC7 et PC6 en entrée
    PORTC |= (1 << PC7) | (1 << PC6);    // Activer les pull-up
    
    // Boucle infinie
    while (1)
    {
        // Bouton B0 (PC7) contrôle LED1 (PE6)
        if ((PINC & (1 << PC7)) == 0)
        {
            // Bouton appuyé, allumer LED1
            PORTE |= (1 << PE6);
        }
        else
        {
            // Bouton relâché, éteindre LED1
            PORTE &= ~(1 << PE6);
        }
        
        // Bouton B1 (PC6) contrôle LED2 (PF1)
        if ((PINC & (1 << PC6)) == 0)
        {
            // Bouton appuyé, allumer LED2
            PORTF |= (1 << PF1);
        }
        else
        {
            // Bouton relâché, éteindre LED2
            PORTF &= ~(1 << PF1);
        }
        
        _delay_ms(20);
    }
    
    return 0;
}