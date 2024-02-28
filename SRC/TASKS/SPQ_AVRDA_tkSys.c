/*
 * File:   tkSystem.c
 * Author: pablo
 *
 * Espera timerPoll y luego muestra los valores de las entradas analogicas.
 * 
 */


#include "SPQ_AVRDA.h"

//------------------------------------------------------------------------------
void tkSys(void * pvParameters)
{

	while (! starting_flag )
		vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );
    
    xprintf_P(PSTR("Starting tkSys..\r\n"));
    
    ainputs_init();
    counter_init();
    systemConf.ptr_ainputs_conf = &ainputs_conf;
    systemConf.ptr_counter_conf = &counter_conf;
    systemConf.ptr_base_conf = &base_conf;
    
    // Espero solo 10s para el primer poleo ( no lo almaceno !!)
    vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
     
	for( ;; )
	{
        vTaskDelay( ( TickType_t)( 1000 ) );
    }

}
//------------------------------------------------------------------------------
