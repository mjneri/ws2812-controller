/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F47K42
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "leds/led_profiles.h"
#include "graphics/drivers/sh1106.h"

#define LEDPROFILES 4

volatile uint8_t currentProfile = 0;
volatile bool longTmrFlag = false;

void Heartbeat_Callback(void)
{
    LED0_Toggle();
}

void LongTimer_Callback(void)
{
    currentProfile = (currentProfile+1) % LEDPROFILES;
    longTmrFlag = true;
}

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    // Register Callbacks
    TMR1_SetInterruptHandler(Heartbeat_Callback);
    TMR3_SetInterruptHandler(LongTimer_Callback);

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    // Open SPI port
    SPI1_Open(SPI1_DEFAULT);
    
    RGB_Clear(LEDSTRIPSIZE);
    OLED_Initialize();
    OLED_ClearDisplay();
    
    OLED_DrawString(0, 0, "Current Profile:", font5x7, 0);

    while (1)
    {
        // Add your application code
        switch(currentProfile)
        {
            case 0:
                OLED_DrawString(8, 0, "Comet's Tail   ", font5x7, 0);
                cometsTail(6);
                break;
            case 1:
                OLED_DrawString(8, 0, "Theater Chase  ", font5x7, 0);
                theaterChase(3, 6);
                break;
            case 2:
                OLED_DrawString(8, 0, "One Color Pulse", font5x7, 0);
                oneColorPulse();
                break;
            default:
                OLED_DrawString(8, 0, "Solid Color    ", font5x7, 0);
                solidColor(0x000000);
                break;
        }
        
        if(longTmrFlag)
        {
            longTmrFlag = false;
        }
    }
}
/**
 End of File
*/