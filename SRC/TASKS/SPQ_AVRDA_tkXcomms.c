#include "SPQ_AVRDA.h"
#include "frtos_cmd.h"

/*
 * El comport esta conectado al UART0 que es el que atiende el modem
 * El buffer debe ser suficientemente grande para recibir los frames de
 * configuracion del servidor
 */



//------------------------------------------------------------------------------
void tkLte(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;
uint8_t c = 0;

    while ( ! starting_flag )
        vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );

    lBchar_CreateStatic ( &lte_lbuffer, lte_buffer, LTE_BUFFER_SIZE );
    
    xprintf_P(PSTR("Starting tkLte..\r\n" ));
    
	// loop
	for( ;; )
	{
        //kick_wdt(XCMB_WDG_bp);
         
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 50ms. lo que genera la espera.
        while ( xfgetc( fdXCOMMS, (char *)&c ) == 1 ) {
            // Si hay datos recividos, los encolo
            if ( ! lBchar_Put( &lte_lbuffer, c) ) {
                // Si la cola esta lleno, la borro !!!!
                lBchar_Flush(&lte_lbuffer);
            }
        }
        
        vTaskDelay( ( TickType_t)( 10 / portTICK_PERIOD_MS ) );
	}    
}
//------------------------------------------------------------------------------
