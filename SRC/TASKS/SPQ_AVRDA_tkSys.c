/*
 * File:   tkSystem.c
 * Author: pablo
 *
 * Espera timerPoll y luego muestra los valores de las entradas analogicas.
 * 
 */


#include "SPQ_AVRDA.h"

dataRcd_s dataRcd;

//------------------------------------------------------------------------------
void tkSys(void * pvParameters)
{
    /*
     * Espero timerpoll y poleo todos los canales, dejando los datos en 
     * un dataRcd.
     * Le paso el dataRcd a la tarea de WAN para que lo envie o lo almacene
     * Finalmente muestro los datos en la terminal
     * 
     * TickType_t puede ser uint16 o uint32. Si usamos configUSE_16_BIT_TICKS 0
     * entonces es de 32 bits.
     * 
     */
TickType_t xLastWakeTime;
TickType_t timerpollInTicks;

	while (!starting_flag )
		vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );
    
    xprintf_P(PSTR("Starting tkSys..\r\n"));
        
    // Espero solo 10s para el primer poleo ( no lo almaceno !!)
    vTaskDelay( ( TickType_t)( 10000 / portTICK_PERIOD_MS ) );

    
	for( ;; )
	{
        xLastWakeTime = xTaskGetTickCount();
        
        // Poleo
        u_poll_data(&dataRcd);
        
        // Envio a tkWAN
        //WAN_process_data_rcd(&dataRcd);
        
        // Imprimo
        u_xprint_dr(&dataRcd);
        
        // Espero
        timerpollInTicks = systemConf.ptr_base_conf->timerpoll * 1000;
        timerpollInTicks /= portTICK_PERIOD_MS;
        vTaskDelayUntil( &xLastWakeTime, timerpollInTicks );
        
    }

}
//------------------------------------------------------------------------------
dataRcd_s *get_dataRcd_ptr(void)
{
    return(&dataRcd);
}
//------------------------------------------------------------------------------
