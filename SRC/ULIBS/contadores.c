
#include "contadores.h"
#include <avr/interrupt.h>

static bool f_debug_counters;

//------------------------------------------------------------------------------
void counter_init_outofrtos( void )
{
    CNT0_CONFIG();
    
    // Configuro los pines para interrumpir
    // Habilito a interrumpir, pullup, flanco de bajada.
    cli();
    PORTF.PIN4CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    sei();
}
// ----------------------------------------------------------------------------- 
void counter_init( void )
{
    counter_clear();
}
void counter_config_defaults( void )
{
    /*
     * Realiza la configuracion por defecto de los canales digitales.
     */

    strncpy( counter_conf.name, "X", CNT_PARAMNAME_LENGTH );
    counter_conf.enabled = false;
    counter_conf.magpp = 1;
    counter_conf.modo_medida = CAUDAL;

}
//------------------------------------------------------------------------------
void counter_print_configuration( void )
{
    /*
     * Muestra la configuracion de todos los canales de contadores en la terminal
     * La usa el comando tkCmd::status.
     */
    

    xprintf_P(PSTR("Counter:\r\n"));
    xprintf_P(PSTR(" debug: "));
    f_debug_counters ? xprintf_P(PSTR("true\r\n")) : xprintf_P(PSTR("false\r\n"));
    
        
    if ( counter_conf.enabled ) {
        xprintf_P( PSTR(" c0: +"));
    } else {
        xprintf_P( PSTR(" c0: -"));
    }
                
    xprintf_P( PSTR("[%s,magpp=%.03f,"), counter_conf.name, counter_conf.magpp );
    if ( counter_conf.modo_medida == CAUDAL ) {
        xprintf_P(PSTR("CAUDAL]\r\n"));
    } else {
        xprintf_P(PSTR("PULSO]\r\n"));
    }
      
}
//------------------------------------------------------------------------------
bool counter_config_channel( char *s_enable, char *s_name, char *s_magpp, char *s_modo )
{
	// Configuro un canal contador.
	// channel: id del canal
	// s_param0: string del nombre del canal
	// s_param1: string con el valor del factor magpp.
	//
	// {0..1} dname magPP

bool retS = false;

    //xprintf_P(PSTR("DEBUG COUNTERS: en=%s,name=%s,magpp=%s,modo=%s,rbsize=%s\r\n"), s_enable,s_name,s_magpp,s_modo,s_rb_size  );

	if ( s_name == NULL ) {
		return(retS);
	}

    // Enable ?
    if (!strcmp_P( strupr(s_enable), PSTR("TRUE"))  ) {
        counter_conf.enabled = true;
    } else if (!strcmp_P( strupr(s_enable), PSTR("FALSE"))  ) {
        counter_conf.enabled = false;
    }
        
    // NOMBRE
	//snprintf_P( cnt->channel[ch].name, CNT_PARAMNAME_LENGTH, PSTR("%s"), s_name );
    strncpy( counter_conf.name, s_name, CNT_PARAMNAME_LENGTH );

	// MAGPP
	if ( s_magpp != NULL ) { counter_conf.magpp = atof(s_magpp); }

    // MODO ( PULSO/CAUDAL )
    if ( s_modo != NULL ) {
		if ( strcmp_P( strupr(s_modo), PSTR("PULSO")) == 0 ) {
			counter_conf.modo_medida = PULSOS;

		} else if ( strcmp_P( strupr(s_modo) , PSTR("CAUDAL")) == 0 ) {
			counter_conf.modo_medida = CAUDAL;

		} else {
			//xprintf_P(PSTR("ERROR: counters modo: PULSO/CAUDAL only!!\r\n"));
            return (false);
		}
    }
        
    retS = true;
	return(retS);

}
//------------------------------------------------------------------------------
void counter_config_debug(bool debug )
{
    if ( debug ) {
        f_debug_counters = true;
    } else {
        f_debug_counters = false;
    }
}
//------------------------------------------------------------------------------
bool counter_read_debug(void)
{
    return (f_debug_counters);
}
//------------------------------------------------------------------------------
uint8_t counter_read_pin(void)
{
    return ( ( CNT0_PORT.IN & CNT0_PIN_bm ) >> CNT0_PIN) ;
}
// -----------------------------------------------------------------------------
uint16_t counter_read(void)
{
    return(contador.pulsos);
}
// -----------------------------------------------------------------------------
void counter_clear(void)
{
   /*
    * Una vez por periodo ( timerpoll ) borro los contadores.
    * Si en el periodo NO llegaron pulsos, aqui debo entonces en los
    * caudales agregar un 0.0 al ring buffer para que luego de un tiempo
    * converja a 0.
    * 
    */
    contador.caudal = 0.0;
    contador.pulsos = 0;
}
//------------------------------------------------------------------------------
uint8_t counter_hash( void )
{
    
uint8_t hash_buffer[32];
uint8_t j;
uint8_t hash = 0;
char *p;

    // Calculo el hash de la configuracion de los contadores


    memset(hash_buffer, '\0', sizeof(hash_buffer));
    j = 0;
    if ( counter_conf.enabled ) {
        j += sprintf_P( (char *)&hash_buffer[j], PSTR("[C%0:TRUE,") );
    } else {
       j += sprintf_P( (char *)&hash_buffer[j], PSTR("[C%0:FALSE,") );
    }
        
    j += sprintf_P( (char *)&hash_buffer[j], PSTR("%s,"), counter_conf.name );
    j += sprintf_P( (char *)&hash_buffer[j], PSTR("%.03f,"), counter_conf.magpp );
        
    if ( counter_conf.modo_medida == 0 ) {
        j += sprintf_P( (char *)&hash_buffer[j], PSTR("CAUDAL"));
    } else {
        j += sprintf_P( (char *)&hash_buffer[j], PSTR("PULSOS"));
    }

    p = (char *)hash_buffer;
    while (*p != '\0') {
        hash = u_hash(hash, *p++);
    }
        
    //xprintf_P(PSTR("HASH_CNT:<%s>, hash=%d\r\n"),hash_buffer, hash );
 
    return(hash);
    
}
//------------------------------------------------------------------------------
 ISR(PORTF_PORT_vect)
{

    // Borro las flags.
    if (PF4_INTERRUPT ) {
        contador.pulsos++;
        PF4_CLEAR_INTERRUPT_FLAG;
    }

}
//------------------------------------------------------------------------------
 

       