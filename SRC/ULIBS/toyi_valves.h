/* 
 * File:   toyi_valves.h
 * Author: pablo
 *
 * Created on 19 de enero de 2024, 08:51 AM
 */

#ifndef TOYI_VALVES_H
#define	TOYI_VALVES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
    
typedef enum { VALVE_OPEN=0, VALVE_CLOSE } t_valve_status;
    
t_valve_status valve_status;

#define VALVE_EN_PORT       PORTC
#define VALVE_EN_PIN_bm     PIN2_bm
#define VALVE_EN_PIN_bp     PIN2_bp
    
#define ENABLE_VALVE()  ( VALVE_EN_PORT.OUT |= VALVE_EN_PIN_bm )
#define DISABLE_VALVE() ( VALVE_EN_PORT.OUT &= ~VALVE_EN_PIN_bm )

#define VALVE_CTRL_PORT       PORTC
#define VALVE_CTRL_PIN_bm     PIN3_bm
#define VALVE_CTRL_PIN_bp     PIN3_bp
    
#define OPEN_VALVE()  ( VALVE_CTRL_PORT.OUT |= VALVE_CTRL_PIN_bm ); valve_status = VALVE_OPEN;
#define CLOSE_VALVE() ( VALVE_CTRL_PORT.OUT &= ~VALVE_CTRL_PIN_bm ); valve_status = VALVE_CLOSE;

void VALVE_EN_init(void);
void VALVE_CTRL_init(void);;
void VALVE_init(void);
t_valve_status *get_valve_status(void);




#ifdef	__cplusplus
}
#endif

#endif	/* TOYI_VALVES_H */

