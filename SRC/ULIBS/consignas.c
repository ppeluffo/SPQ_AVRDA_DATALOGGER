
#include "consignas.h"

//------------------------------------------------------------------------------
void consigna_set_nocturna(void)
{
	// ( VA open / VB close ) -> ( VA close / VB open )
	// Open VB con lo que el punto común de las válvulas queda a pAtm y la VA puede operar correctamente.
	// Close VA.
    // Mando un comando modbus a la placa CONTROL_PRESION


}
//------------------------------------------------------------------------------
void consigna_set_diurna(void)
{
    // Mando un comando modbus a la placa CONTROL_PRESION
}
//------------------------------------------------------------------------------
void consigna_config_defaults(void)
{
    consigna_conf.enabled = false;
    consigna_conf.consigna_diurna = 700;
    consigna_conf.consigna_nocturna = 2300;
}
//------------------------------------------------------------------------------
bool consigna_config( char *s_enable, char *s_cdiurna, char *s_cnocturna )
{
    
    //xprintf_P(PSTR("CONSIGNA DEBUG: %s,%s,%s\r\n"),s_enable,s_cdiurna,s_cnocturna);
    
    if (!strcmp_P( strupr(s_enable), PSTR("TRUE"))  ) {
        consigna_conf.enabled = true;
    }
    
    if (!strcmp_P( strupr(s_enable), PSTR("FALSE"))  ) {
        consigna_conf.enabled = false;
    }
    
    consigna_conf.consigna_diurna = atoi(s_cdiurna);
    consigna_conf.consigna_nocturna = atoi(s_cnocturna);
    
    return (true);
    
}
//------------------------------------------------------------------------------
void consigna_print_configuration(void)
{
    xprintf_P( PSTR("Consigna:\r\n"));
    if (  ! consigna_conf.enabled ) {
        xprintf_P( PSTR(" status=disabled\r\n"));
        return;
    }
    
    xprintf_P( PSTR(" status=enabled\r\n"));
	xprintf_P( PSTR(" cDiurna=%04d, cNocturna=%04d\r\n"), consigna_conf.consigna_diurna, consigna_conf.consigna_nocturna  );

}
//------------------------------------------------------------------------------
uint8_t consigna_hash(void)
{
    
uint8_t hash = 0;
char *p;
uint8_t hash_buffer[64];

   // Calculo el hash de la configuracion modbus

    memset(hash_buffer, '\0', sizeof(hash_buffer) );
    if ( consigna_conf.enabled ) {
        sprintf_P( (char *)&hash_buffer, PSTR("[TRUE,%04d,%04d]"),consigna_conf.consigna_diurna, consigna_conf.consigna_nocturna);
    } else {
        sprintf_P( (char *)&hash_buffer, PSTR("[FALSE,%04d,%04d]"),consigna_conf.consigna_diurna, consigna_conf.consigna_nocturna);
    }
    p = (char *)hash_buffer;
    while (*p != '\0') {
        hash = u_hash(hash, *p++);
    }
    //xprintf_P(PSTR("HASH_PILOTO:%s, hash=%d\r\n"), hash_buffer, hash ); 
    return(hash);   
}
//------------------------------------------------------------------------------
void consigna_service(void)
{
 
RtcTimeType_t rtcDateTime;
uint16_t now;

	// Chequeo y aplico.
	// Las consignas se chequean y/o setean en cualquier modo de trabajo, continuo o discreto
	memset( &rtcDateTime, '\0', sizeof(RtcTimeType_t));
	if ( ! RTC_read_dtime(&rtcDateTime) ) {
		xprintf_P(PSTR("CONSIGNA ERROR: I2C:RTC chequear_consignas\r\n\0"));
		return;
	}

    now = rtcDateTime.hour * 100 + rtcDateTime.min;
    
	// Consigna diurna ?
	if ( now == consigna_conf.consigna_diurna  ) {
		consigna_set_diurna();
		xprintf_P(PSTR("Set CONSIGNA diurna %04d\r\n"), now );
		return;
	}

	// Consigna nocturna ?
	if ( now == consigna_conf.consigna_nocturna  ) {
		consigna_set_nocturna();
		xprintf_P(PSTR("Set CONSIGNA nocturna %04d\r\n"),now);
		return;
	}
    
}
//------------------------------------------------------------------------------
