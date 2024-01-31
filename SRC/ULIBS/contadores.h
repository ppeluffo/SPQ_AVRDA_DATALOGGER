/* 
 * File:   contadores.h
 * Author: pablo
 *
 * Created on July 19, 2023, 5:04 PM
 */

#ifndef CONTADORES_H
#define	CONTADORES_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
    
#include "xprintf.h"
#include "ina3221.h"    
#include "pines.h"
#include "utils.h"

#define CNT0_PORT	PORTF
#define CNT0_PIN    4   
#define CNT0_PIN_bm	PIN4_bm
#define CNT0_PIN_bp	PIN4_bp

// Los CNTx son inputs
#define CNT0_CONFIG()    ( CNT0_PORT.DIR &= ~CNT0_PIN_bm )

void COUNTER_init_outofrtos(void);
uint8_t COUNTER_read(void);


#ifdef	__cplusplus
}
#endif

#endif	/* CONTADORES_H */

