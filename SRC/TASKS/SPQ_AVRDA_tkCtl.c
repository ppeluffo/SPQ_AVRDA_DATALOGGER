/*
 * File:   tkCtl.c
 * Author: pablo
 *
 * Created on 25 de octubre de 2021, 12:50 PM
 */


#include "SPQ_AVRDA.h"

#define TKCTL_DELAY_S	1

void sys_watchdog_check(void);
void sys_daily_reset(void);

//------------------------------------------------------------------------------
void tkCtl(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

//uint16_t a;

	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );
    xprintf_P(PSTR("Starting tkCtl..\r\n"));
    
    ainputs_init();
    counter_init();
    
    // Inicializo los punteros
    systemConf.ptr_ainputs_conf = &ainputs_conf;
    systemConf.ptr_counter_conf = &counter_conf;
    systemConf.ptr_base_conf = &base_conf;
    
    // Leo la configuracion de EE en systemConf
    xprintf_P(PSTR("Loading config...\r\n"));
    if ( ! u_load_config_from_NVM())  {
       xprintf_P(PSTR("Loading config default..\r\n"));
       u_config_default();
    }
    
    WDG_INIT();
     
    vTaskDelay( ( TickType_t)( 2000 / portTICK_PERIOD_MS ) );
    
    // Por ultimo habilito a todas las otras tareas a arrancar
    starting_flag = true;
    
	for( ;; )
	{
        //vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
		vTaskDelay( ( TickType_t)( 1000 * TKCTL_DELAY_S / portTICK_PERIOD_MS ) );
        led_flash();
        //sys_watchdog_check();
        //sys_daily_reset();
        // xfprintf_P( fdXCOMMS, PSTR("The quick brown fox jumps over the lazy dog = %d\r\n"),a++);
        
	}
}
//------------------------------------------------------------------------------
void sys_watchdog_check(void)
{
    // El watchdog se inicializa en 2F.
    // Cada tarea debe poner su bit en 0. Si alguna no puede, se resetea
    // Esta funcion se corre cada 5s (TKCTL_DELAY_S)
    
static uint16_t wdg_count = 0;

    //xprintf_P(PSTR("wdg reset\r\n"));
    wdt_reset();
    return;
        
    // EL wdg lo leo cada 120secs ( 5 x 60 )
    if ( wdg_count++ <  (240 / TKCTL_DELAY_S ) ) {
        wdt_reset();
        return;
    }
    
    //xprintf_P(PSTR("DEBUG: wdg check\r\n"));
    wdg_count = 0;
    
    // Analizo los watchdows individuales
    //xprintf_P(PSTR("tkCtl: check wdg [0x%02X]\r\n"), sys_watchdog );

    if ( sys_watchdog != 0 ) {  
        xprintf_P(PSTR("tkCtl: reset by wdg [0x%02X]\r\n"), sys_watchdog );
        vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
        reset();
        
    } else {
        wdt_reset();
        WDG_INIT();
    }
}
//------------------------------------------------------------------------------
void sys_daily_reset(void)
{
	// Todos los dias debo resetearme para restaturar automaticamente posibles
	// problemas.
	// Se invoca 1 vez por minuto ( 60s ).

static uint32_t ticks_to_reset = 86400 / TKCTL_DELAY_S ; // ticks en 1 dia.

	//xprintf_P( PSTR("DEBUG dailyReset..\r\n\0"));
	while ( --ticks_to_reset > 0 ) {
		return;
	}

	xprintf_P( PSTR("Daily Reset !!\r\n\0") );
    vTaskDelay( ( TickType_t)( 2000 / portTICK_PERIOD_MS ) );
    reset();
    
}
//------------------------------------------------------------------------------
