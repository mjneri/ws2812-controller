/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define STEPS 256       // No. of brightness steps for each color value

const uint8_t logBrightness[STEPS] = {
    0, 31, 50, 63, 74, 82, 89,
    95,101,106,110,114,118,121,
    124,127,130,133,135,137,140,
    142,144,146,148,149,151,153,
    155,156,158,159,160,162,163,
    164,166,167,168,169,170,172,
    173,174,175,176,177,178,179,
    180,181,181,182,183,184,185,
    186,186,187,188,189,189,190,
    191,192,192,193,194,194,195,
    196,196,197,198,198,199,199,
    200,201,201,202,202,203,203,
    204,205,205,206,206,207,207,
    208,208,209,209,210,210,211,
    211,212,212,212,213,213,214,
    214,215,215,215,216,216,217,
    217,218,218,218,219,219,220,
    220,220,221,221,221,222,222,
    223,223,223,224,224,224,225,
    225,225,226,226,226,227,227,
    227,228,228,228,229,229,229,
    230,230,230,230,231,231,231,
    232,232,232,233,233,233,233,
    234,234,234,235,235,235,235,
    236,236,236,236,237,237,237,
    238,238,238,238,239,239,239,
    239,240,240,240,240,241,241,
    241,241,242,242,242,242,242,
    243,243,243,243,244,244,244,
    244,245,245,245,245,245,246,
    246,246,246,247,247,247,247,
    247,248,248,248,248,248,249,
    249,249,249,249,250,250,250,
    250,250,251,251,251,251,251,
    252,252,252,252,252,253,253,
    253,253,253,253,254,254,254,
    254,254,255,255
};

const uint8_t xponentialBrightness[STEPS] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,
    4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,
    6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,10,10,10,
    10,10,11,11,11,11,12,12,12,12,13,13,13,14,14,
    14,15,15,15,16,16,16,17,17,17,18,18,19,19,20,
    20,20,21,21,22,22,23,23,24,24,25,26,26,27,27,
    28,29,29,30,30,31,32,33,33,34,35,36,36,37,38,
    39,40,41,41,42,43,44,45,46,47,48,49,51,52,53,
    54,55,56,58,59,60,62,63,64,66,67,69,70,72,73,
    75,77,78,80,82,84,86,87,89,91,93,95,98,100,102,
    104,106,109,111,114,116,119,121,124,127,130,132,
    135,138,141,144,148,151,154,158,161,165,168,172,
    176,180,184,188,192,196,200,205,209,214,219,223,
    228,233,238,244,249,254
};

extern uint8_t pixelProfile[570] = {
    // Shifting Red LED
    0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,25,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,25,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,0,0, 0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
    0,25,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
};

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

