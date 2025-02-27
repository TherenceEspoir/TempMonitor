/* -----------------------------------------------------
     2 LED + 2 Boutons
   - LED1 reliée à PD2 (Sortie)
   - LED2 reliée à PD3 (Sortie)
   - Bouton1 relié à PF0 (Entrée, pull-up)
   - Bouton2 relié à PF1 (Entrée, pull-up)

   Objectif :
   - Quand Bouton1 est appuyé, LED1 s'allume
   - Quand Bouton2 est appuyé, LED2 s'allume
   - Sinon, les LED sont éteintes.


   ----------------------------------------------------- */

#define F_CPU 8000000UL   // Fréquence du microcontrôleur
#include <avr/io.h>       // Définitions des registres d'E/S
#include <util/delay.h>   // Pour la fonction _delay_ms()

int main(void)
{
    // 1) Configurer les LED en sortie
    //    - LED1 sur PD2
    //    - LED2 sur PD3
    DDRD |= (1 << PD2) | (1 << PD3);  
    // => On met à 1 le bit 2 et le bit 3 de DDRD pour que PD2 et PD3 soient des sorties.

    // 2) Configurer les boutons en entrée avec pull-up
    //    - Bouton1 sur PF0
    //    - Bouton2 sur PF1
    DDRF &= ~((1 << PF0) | (1 << PF1));
    // => On met à 0 le bit 0 et bit 1 de DDRF (PF0, PF1) => entrées.

    PORTF |= (1 << PF0) | (1 << PF1);
    // => Mettre à 1 le bit 0 et 1 de PORTF active les résistances de pull-up internes
    //    sur PF0 et PF1. Ainsi, PF0 et PF1 sont "tirées" au niveau logique haut
    //    quand le bouton n'est pas enfoncé.

    // Boucle infinie
    while (1)
    {
        // --- Bouton1 / LED1 ---
        // Lire l'état de PF0 via PINF.
        // Si (PINF & (1 << PF0)) == 0 => PF0 est à 0 => bouton appuyé
        // Sinon => bouton relâché
        if ( (PINF & (1 << PF0)) == 0 )
        {
            // Appui => Allumer LED1 (PD2 à 1)
            PORTD |= (1 << PD2);
        }
        else
        {
            // Relâché => Éteindre LED1 (PD2 à 0)
            PORTD &= ~(1 << PD2);
        }

        // --- Bouton2 / LED2 ---
        // Même logique sur PF1/PD3
        if ( (PINF & (1 << PF1)) == 0 )
        {
            // Appui => Allumer LED2
            PORTD |= (1 << PD3);
        }
        else
        {
            // Relâché => Éteindre LED2
            PORTD &= ~(1 << PD3);
        }

        // Petit délai pour stabiliser la lecture et éviter un bouclage trop rapide.
        _delay_ms(20);
    }

    // Le programme ne quitte jamais la boucle while(1).
    // Pas de "return" nécessaire car on ne revient pas de main().
    return 0;
}
