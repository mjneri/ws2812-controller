//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// rotaryencoder.h -- Header file for rotaryencoder.c
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: mjneri
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: rotaryencoder.h
// Description: 
//
// Revisions:
// Revision 0.01 - File Created (2022-12-10)
// Additional Comments:
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Header guard
#ifndef _ROTARYENCODER_H_
#define _ROTARYENCODER_H_

// *****************************************************************************
// *****************************************************************************
// Section: Header Files
// *****************************************************************************
// *****************************************************************************

#include "../mcc_generated_files/mcc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Constant Defines
// *****************************************************************************
// *****************************************************************************

#define ROTENC_RINGBUF_MAXSIZE     8
#define ROTENC_RINGBUF_DATA_MAX    (ROTENC_RINGBUF_MAXSIZE*8)
#define ROTENC_VLCTY_CALLRATEMS    100

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data type definitions
// *****************************************************************************
// *****************************************************************************

// Rotation direction
typedef enum
{
    ROTENC_CCW = 0,
    ROTENC_CW,
            
    ROTENC_ERR = 0xFF
} ROT_DIR;

// Ring Buffer
typedef struct
{
    volatile uint8_t buffer[ROTENC_RINGBUF_MAXSIZE];
    uint8_t rdIndex;            // Index pointing to data to be read
    uint8_t rdBitIndex;         // Bit index within the byte...

    uint8_t wrIndex;            // Index pointing to data to be written
    uint8_t wrBitIndex;         // Bit index within the byte...

    bool isEmpty;               // Flag; set when the ring buffer is empty
    bool isFull;                // Flag; set when the ring buffer is full
    uint8_t dataAmt;            // Determines how many ring buffer elements are stored
} rot_ring_buf_t;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

void CLC2_Callback(void);
void CLC3_Callback(void);

// Note: millis_Initialize() must be called before this function
void ROTENC_Initialize(void);

ROT_DIR ROTENC_ReadRingBuf(void);

void ROTENC_GetRotationCount(uint16_t *cw_count, uint16_t *ccw_count);

/**
 * Example usage:
 *      // Read buffered inputs
 *      rotVelocity = ROTENC_Velocity();
 *      if(rotVelocity > 0)
 *      {
 *          // Read inputs only when encoder was turned
 *          // Because velocity updates every x ms, there should be enough time
 *          // to read all the buffered inputs.
 *          rotDirection = ROTENC_ReadRingBuf();
 *          if(rotDirection != ROTENC_ERR)
 *          {
 *              // Increment the buffered input counter.
 *              rotEnc_InputCounter++;

 *              if(rotDirection == ROTENC_CW)
 *              {
 *                  buf_cw_count++;
 *                  testcounter = testcounter + rotVelocity;
 *              }
 *              else
 *              {
 *                  buf_ccw_count++;
 *                  testcounter = testcounter - rotVelocity;
 *              }
 *          }
 *      }
 *  
 */
uint16_t ROTENC_Velocity(void);

#endif /*_ROTARYENCODER_H_*/

/*******************************************************************************
 End of File
*/

