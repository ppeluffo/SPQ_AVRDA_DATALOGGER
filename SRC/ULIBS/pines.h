    /* 
 * File:   pines.h
 * Author: pablo
 *
 * Created on 11 de febrero de 2022, 06:02 PM
 */

#ifndef PINES_H
#define	PINES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include "stdbool.h"
    
//--------------------------------------------------------------------------

// VSENSORS (sensors de 4-20, output)
#define VSENSORS_PORT         PORTC
#define VSENSORS              1
#define VSENSORS_PIN_bm       PIN1_bm
#define VSENSORS_PIN_bp       PIN1_bp
#define SET_VSENSORS()        ( VSENSORS_PORT.OUT |= VSENSORS_PIN_bm )
#define CLEAR_VSENSORS()      ( VSENSORS_PORT.OUT &= ~VSENSORS_PIN_bm )
#define CONFIG_VSENSORS()     VSENSORS_PORT.DIR |= VSENSORS_PIN_bm;
    
// EN_PWR_CPRES (output)
#define EN_PWR_CPRES_PORT         PORTD
#define EN_PWR_CPRES              6
#define EN_PWR_CPRES_PIN_bm       PIN6_bm
#define EN_PWR_CPRES_PIN_bp       PIN6_bp
#define SET_EN_PWR_CPRES()        ( EN_PWR_CPRES_PORT.OUT |= EN_PWR_CPRES_PIN_bm )
#define CLEAR_EN_PWR_CPRES()      ( EN_PWR_CPRES_PORT.OUT &= ~EN_PWR_CPRES_PIN_bm )
#define CONFIG_EN_PWR_CPRES()     EN_PWR_CPRES_PORT.DIR |= EN_PWR_CPRES_PIN_bm;
    
// EN_SENS3V3 (output)
#define EN_SENS3V3_PORT         PORTD
#define EN_SENS3V3              5
#define EN_SENS3V3_PIN_bm       PIN5_bm
#define EN_SENS3V3_PIN_bp       PIN5_bp
#define SET_EN_SENS3V3()        ( EN_SENS3V3_PORT.OUT |= EN_SENS3V3_PIN_bm )
#define CLEAR_EN_SENS3V3()      ( EN_SENS3V3_PORT.OUT &= ~EN_SENS3V3_PIN_bm )
#define CONFIG_EN_SENS3V3()     EN_PWR_CPRES_PORT.DIR |= EN_SENS3V3_PIN_bm;
    
// EN_SENS12V (output)
#define EN_SENS12V_PORT         PORTD
#define EN_SENS12V               4
#define EN_SENS12V_PIN_bm       PIN4_bm
#define EN_SENS12V_PIN_bp       PIN4_bp
#define SET_EN_SENS12V()        ( EN_SENS12V_PORT.OUT |= EN_SENS12V_PIN_bm )
#define CLEAR_EN_SENS12V()      ( EN_SENS12V_PORT.OUT &= ~EN_SENS12V_PIN_bm )
#define CONFIG_EN_SENS12V()     EN_PWR_CPRES_PORT.DIR |= EN_SENS12V_PIN_bm;
    
    
#define RTS_RS485A_PORT         PORTC
#define RTS_RS485A              2
#define RTS_RS485A_PIN_bm       PIN2_bm
#define RTS_RS485A_PIN_bp       PIN2_bp
#define SET_RTS_RS485A()        ( RTS_RS485A_PORT.OUT |= RTS_RS485A_PIN_bm )
#define CLEAR_RTS_RS485A()      ( RTS_RS485A_PORT.OUT &= ~RTS_RS485A_PIN_bm )

#define CONFIG_RTS_485A()       RTS_RS485A_PORT.DIR |= RTS_RS485A_PIN_bm;


#define RTS_RS485B_PORT         PORTG
#define RTS_RS485B              7
#define RTS_RS485B_PIN_bm       PIN7_bm
#define RTS_RS485B_PIN_bp       PIN7_bp
#define SET_RTS_RS485B()        ( RTS_RS485B_PORT.OUT |= RTS_RS485B_PIN_bm )
#define CLEAR_RTS_RS485B()      ( RTS_RS485B_PORT.OUT &= ~RTS_RS485B_PIN_bm )

#define CONFIG_RTS_485B()       RTS_RS485B_PORT.DIR |= RTS_RS485B_PIN_bm

// Los pines de FinCarrera son entradas
#define FC1_PORT      PORTE    
#define FC1           6
#define FC1_PIN_bm    PIN6_bm
#define FC1_PIN_bp    PIN6_bp
#define CONFIG_FC1()    ( FC1_PORT.DIR &= ~FC1_PIN_bm )

#define FC2_PORT      PORTA     
#define FC2           7
#define FC2_PIN_bm    PIN7_bm
#define FC2_PIN_bp    PIN7_bp
#define CONFIG_FC2()    ( FC2_PORT.DIR &= ~FC2_PIN_bm )

uint8_t FC1_read(void);
uint8_t FC2_read(void);

#define FC_alta_read() FC1_read()
#define FC_baja_read() FC2_read()

void FCx_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PINES_H */

