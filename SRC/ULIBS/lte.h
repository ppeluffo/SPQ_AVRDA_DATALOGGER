/* 
 * File:   lte.h
 * Author: pablo
 *
 * Created on 25 de enero de 2024, 10:23 AM
 */

#ifndef LTE_H
#define	LTE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include "stdbool.h"
#include "linearBuffer.h"
#include "frtos-io.h"
#include "xprintf.h"
#include "string.h"
    
//--------------------------------------------------------------------------
// PUSR WH-LTE-7S1
// LTE_EN_DCIN
#define LTE_EN_DCIN_PORT         PORTB
#define LTE_EN_DCIN              5
#define LTE_EN_DCIN_PIN_bm       PIN5_bm
#define LTE_EN_DCIN_PIN_bp       PIN5_bp
#define SET_LTE_EN_DCIN()        ( LTE_EN_DCIN_PORT.OUT |= LTE_EN_DCIN_PIN_bm )
#define CLEAR_LTE_EN_DCIN()      ( LTE_EN_DCIN_PORT.OUT &= ~LTE_EN_DCIN_PIN_bm )
#define CONFIG_LTE_EN_DCIN()     LTE_EN_DCIN_PORT.DIR |= LTE_EN_DCIN_PIN_bm;

// LTE_EN_VCAP
#define LTE_EN_VCAP_PORT         PORTB
#define LTE_EN_VCAP              6
#define LTE_EN_VCAP_PIN_bm       PIN6_bm
#define LTE_EN_VCAP_PIN_bp       PIN6_bp
#define SET_LTE_EN_VCAP()        ( LTE_EN_VCAP_PORT.OUT |= LTE_EN_VCAP_PIN_bm )
#define CLEAR_LTE_EN_VCAP()      ( LTE_EN_VCAP_PORT.OUT &= ~LTE_EN_VCAP_PIN_bm )
#define CONFIG_LTE_EN_VCAP()     LTE_EN_VCAP_PORT.DIR |= LTE_EN_VCAP_PIN_bm;

// LTE_PWR
#define LTE_PWR_PORT         PORTB
#define LTE_PWR              4
#define LTE_PWR_PIN_bm       PIN4_bm
#define LTE_PWR_PIN_bp       PIN4_bp
#define CLEAR_LTE_PWR()        ( LTE_PWR_PORT.OUT |= LTE_PWR_PIN_bm )
#define SET_LTE_PWR()      ( LTE_PWR_PORT.OUT &= ~LTE_PWR_PIN_bm )
#define CONFIG_LTE_PWR()     LTE_PWR_PORT.DIR |= LTE_PWR_PIN_bm;

// LTE_RESET
#define LTE_RESET_PORT         PORTC
#define LTE_RESET              0
#define LTE_RESET_PIN_bm       PIN0_bm
#define LTE_RESET_PIN_bp       PIN0_bp
#define CLEAR_LTE_RESET()        ( LTE_RESET_PORT.OUT |= LTE_RESET_PIN_bm )
#define SET_LTE_RESET()      ( LTE_RESET_PORT.OUT &= ~LTE_RESET_PIN_bm )
#define CONFIG_LTE_RESET()     LTE_RESET_PORT.DIR |= LTE_RESET_PIN_bm;

// LTE_RELOAD
#define LTE_RELOAD_PORT         PORTB
#define LTE_RELOAD              7
#define LTE_RELOAD_PIN_bm       PIN7_bm
#define LTE_RELOAD_PIN_bp       PIN7_bp
#define CLEAR_LTE_RELOAD()        ( LTE_RELOAD_PORT.OUT |= LTE_RELOAD_PIN_bm )
#define SET_LTE_RELOAD()      ( LTE_RELOAD_PORT.OUT &= ~LTE_RELOAD_PIN_bm )
#define CONFIG_LTE_RELOAD()     LTE_RELOAD_PORT.DIR |= LTE_RELOAD_PIN_bm;

    
#define LTE_BUFFER_SIZE 255

char lte_buffer[LTE_BUFFER_SIZE];
lBuffer_s lte_lbuffer;


void LTE_process_buffer( char c);


typedef enum { LTE_PWR_OFF=0, LTE_PWR_ON} t_lte_pwr_status;
    
void LTE_init(void);
void LTE_prender(void);
void LTE_apagar(void);
t_lte_pwr_status LTE_get_pwr_status(void);
char *LTE_buffer_ptr(void);
void LTE_flush_buffer(void);
void lte_test_link(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LTE_H */

