// Header guards
#ifndef _TEST_H_
#define _TEST_H_

// Project includes
#include <xc.h>
#include "../graphics/drivers/sh1106.h"
#include "../leds/spi_led.h"
#include "../mcc_generated_files/mcc.h"
#include "../modules/millis.h"
#include "../graphics/drivers/gfx.h"
#include "../userinput/buttons.h"

// Function prototype - test.c
void CLC2_Callback(void);
void CLC3_Callback(void);
void TEST_Function(void);

// Function prototype - test_2.c
void TEST_2_Function(void);

#endif /*_TEST_H_*/