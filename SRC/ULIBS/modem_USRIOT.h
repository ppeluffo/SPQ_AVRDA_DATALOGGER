/* 
 * File:   modem_USRIOT.h
 * Author: pablo
 *
 * Created on 24 de abril de 2024, 11:30 AM
 */

#ifndef MODEM_USRIOT_H
#define	MODEM_USRIOT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
    
#include "xprintf.h"
#include "modem_lte.h"
    

bool MODEM_enter_mode_at(void);
void MODEM_exit_mode_at(void);
void MODEM_query_parameters(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MODEM_USRIOT_H */

