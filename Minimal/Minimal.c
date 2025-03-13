/*
             LUFA Library
     Copyright (C) Dean Camera, 2021.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2021  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the Minimal demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */

#define  INCLUDE_FROM_MINIMAL_C
#include "Minimal.h"

#define CMD_ERASE    0x01  // Commande pour effacer les données stockées
#define CMD_SENDDATA 0x02  // Commande pour envoyer les données au PC
#define BLOCK_SIZE 512

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
	SetupHardware();
  SD_info sd; 
	GlobalInterruptEnable();

	for (;;)
	{
		USB_USBTask();
    HandleVendorINEndpoint();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	USB_Init();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs.
 */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
  Endpoint_ConfigureEndpoint(<VENDOR_IN_EPADDR>, <EP_TYPE_BULK>, <VENDOR_IN_EPSIZE>, 1);
  Endpoint_ConfigureEndpoint(<VENDOR_OUT_EPADDR>, <EP_TYPE_INTERRUPT>, <VENDOR_OUT_EPSIZE>, 1);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
}

void EraseData(void)
{

}


void HandleVendorINEndpoint(void)
{
    uint8_t blockData[BLOCK_SIZE];
    uint16_t i;
    uint32_t currentBlock = 0; // Numéro du bloc à lire

    // Lire le bloc 'currentBlock' depuis la carte SD dans le buffer blockData
    if (readBlock(&sd, currentBlock, blockData) != 0)
    {
        // En cas d'erreur de lecture, on peut ne rien envoyer ou gérer l'erreur
        return;
    }
    
    // Sélectionner l'endpoint IN
    Endpoint_SelectEndpoint(VENDOR_IN_EPADDR);
    
    if (Endpoint_IsReadWriteAllowed())
    {
        // Optionnel : envoyer le numéro de bloc (1 octet)
        Endpoint_Write_Byte((uint8_t)(currentBlock & 0xFF));
        Endpoint_ClearIN();
        
        // Envoyer le bloc complet octet par octet, et après chaque paquet de 64 octets, finaliser le transfert
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            Endpoint_Write_Byte(blockData[i]);
            
            // Dès que 64 octets ont été envoyés, finaliser le paquet
            if ((i + 1) % VENDOR_IN_EPSIZE == 0)
            {
                Endpoint_ClearIN();
                // Attendre que l'endpoint soit de nouveau prêt
                while (!Endpoint_IsReadWriteAllowed());
            }
        }
    }
}

 /**
 * Gère le point d'accès OUT : 
 * Reçoit des commandes depuis l'hôte.
 */
void HandleVendorOUTEndpoint(void)
{
    Endpoint_SelectEndpoint(VENDOR_OUT_EPADDR);
    if (Endpoint_IsOUTReceived())
    {
        if (Endpoint_IsReadWriteAllowed())
        {
            uint8_t command = Endpoint_Read_Byte();
            // si la commande vaut 0x01, on efface les données stockées
            if (command == CMD_ERASE)
            {
                // Si la commande vaut CMD_ERASE (0x01), effacer les données
                EraseData();
            }
        }
        Endpoint_ClearOUT();
    }
}
