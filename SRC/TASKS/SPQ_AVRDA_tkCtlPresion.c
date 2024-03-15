/*
 * File:   tkPiloto.c
 * Author: pablo
 *
 */


#include "SPQ_AVRDA.h"

//------------------------------------------------------------------------------
void tkCtlPresion(void * pvParameters)
{

	/*
     * Tarea que implementa el sistema de piloto para controlar una
     * valvula reguladora.
     * Impleentamos un modelo productor - consumidor.
     * 
     */
    
( void ) pvParameters;


	while (! starting_flag )
		vTaskDelay( ( TickType_t)( 200 / portTICK_PERIOD_MS ) );

    SYSTEM_ENTER_CRITICAL();
    task_running |= CTLPRES_WDG_gc;
    SYSTEM_EXIT_CRITICAL();
    
	vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );
    xprintf_P(PSTR("Starting tkCtlPresion..\r\n"));
    
   
	for( ;; )
	{
        /*
         * Corre cada 1 minuto porque el timeslot se mide como hhmm y no queremos
         * que se realmacene la orden de un mismo tslot
         * 
         */
        u_kick_wdt(CTLPRES_WDG_gc);
		vTaskDelay( ( TickType_t)( 30000 / portTICK_PERIOD_MS ) );
        
        if ( systemConf.ptr_consigna_conf->enabled ) {
            consigna_service();
        }
        
	}
}
//------------------------------------------------------------------------------

