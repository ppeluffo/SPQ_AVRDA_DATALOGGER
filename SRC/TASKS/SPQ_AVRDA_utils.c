/* 
 * File:   frtos20_utils.c
 * Author: pablo
 *
 * Created on 22 de diciembre de 2021, 07:34 AM
 */

#include "SPQ_AVRDA.h"
#include "pines.h"

//------------------------------------------------------------------------------
int8_t WDT_init(void);
int8_t CLKCTRL_init(void);
uint8_t checksum( uint8_t *s, uint16_t size );

//-----------------------------------------------------------------------------
void system_init()
{

    // Init OUT OF RTOS !!!
    
	CLKCTRL_init();
    //WDT_init();
    LED_init();
    XPRINTF_init();
    VALVE_init(); 
    ADC_init();
    LTE_init();
    I2C_init();
    
    CONFIG_EN_PWR_CPRES();
    CONFIG_EN_PWR_SENSEXT();
    CONFIG_EN_PWR_QMBUS();
    
    CONFIG_RTS_485();
    
    CONFIG_EN_SENS3V3();
    CONFIG_EN_SENS12V();
    CONFIG_PWR_SENSORS();
    
    
     
}
//-----------------------------------------------------------------------------
int8_t WDT_init(void)
{
	/* 8K cycles (8.2s) */
	/* Off */
	ccp_write_io((void *)&(WDT.CTRLA), WDT_PERIOD_8KCLK_gc | WDT_WINDOW_OFF_gc );  
	return 0;
}
//-----------------------------------------------------------------------------
int8_t CLKCTRL_init(void)
{
	// Configuro el clock para 24Mhz
	
	ccp_write_io((void *)&(CLKCTRL.OSCHFCTRLA), CLKCTRL_FREQSEL_24M_gc         /* 24 */
	| 0 << CLKCTRL_AUTOTUNE_bp /* Auto-Tune enable: disabled */
	| 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),CLKCTRL_CLKSEL_OSCHF_gc /* Internal high-frequency oscillator */
	//		 | 0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */);

	// ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0 << CLKCTRL_LOCKEN_bp /* lock enable: disabled */);

	return 0;
}
//-----------------------------------------------------------------------------
void reset(void)
{
    xprintf_P(PSTR("ALERT !!!. Going to reset...\r\n"));
    vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );
	/* Issue a Software Reset to initilize the CPU */
	ccp_write_io( (void *)&(RSTCTRL.SWRR), RSTCTRL_SWRST_bm ); 
                                           
}
//------------------------------------------------------------------------------
void u_kick_wdt( uint8_t wdg_gc)
{
    // Pone el bit correspondiente en 0.
    sys_watchdog &= ~wdg_gc;
    
}
//------------------------------------------------------------------------------
void u_config_default(void)
{

    // Configuro a default todas las configuraciones locales
    // y luego actualizo el systemConf
    
    memcpy(systemConf.ptr_base_conf->dlgid, "DEFAULT\0", sizeof(systemConf.ptr_base_conf->dlgid));
    
    systemConf.ptr_base_conf->timerdial = 0;
    systemConf.ptr_base_conf->timerpoll = 60;
    
    systemConf.ptr_base_conf->pwr_modo = PWR_CONTINUO;
    systemConf.ptr_base_conf->pwr_hhmm_on = 2330;
    systemConf.ptr_base_conf->pwr_hhmm_off = 630;
    
    ainputs_config_defaults();
    counter_config_defaults();
    consigna_config_defaults();
    modbus_config_defaults();
    piloto_config_defaults();
    
}
//------------------------------------------------------------------------------
bool config_debug( char *tipo, char *valor)
{
    return(true);
}
//------------------------------------------------------------------------------
bool u_save_config_in_NVM(void)
{
   
int8_t retVal;
uint8_t cks;

struct {
    base_conf_t base_conf;
	ainputs_conf_t ainputs_conf;
    counter_conf_t counter_conf;
    consigna_conf_t consigna_conf;
    modbus_conf_t modbus_conf;
    piloto_conf_t piloto_conf;
    // El checksum SIEMPRE debe ser el ultimo byte !!!!!
    uint8_t checksum;
} memConfBuffer;

    // Cargamos el buffer con las configuraciones
    memcpy( &memConfBuffer.base_conf, systemConf.ptr_base_conf, sizeof(base_conf));
    memcpy( &memConfBuffer.ainputs_conf, systemConf.ptr_ainputs_conf, sizeof(ainputs_conf));
    memcpy( &memConfBuffer.counter_conf, systemConf.ptr_counter_conf, sizeof(counter_conf));
    memcpy( &memConfBuffer.consigna_conf, systemConf.ptr_consigna_conf, sizeof(consigna_conf));
    memcpy( &memConfBuffer.modbus_conf, systemConf.ptr_modbus_conf, sizeof(modbus_conf));
    memcpy( &memConfBuffer.piloto_conf, systemConf.ptr_piloto_conf, sizeof(piloto_conf));

    cks = checksum ( (uint8_t *)&memConfBuffer, ( sizeof(memConfBuffer) - 1));
    memConfBuffer.checksum = cks;

    xprintf_P(PSTR("SAVE DEBUG: memblock size = %d\r\n"), sizeof(memConfBuffer));
    
    retVal = NVMEE_write( 0x00, (char *)&memConfBuffer, sizeof(memConfBuffer) );
    
    //xprintf_P(PSTR("DEBUG: Save in NVM OK\r\n"));
    
    if (retVal == -1 )
        return(false);
    
    return(true);
   
}
//------------------------------------------------------------------------------
bool u_load_config_from_NVM(void)
{

uint8_t rd_cks, calc_cks;
struct {
    base_conf_t base_conf;
	ainputs_conf_t ainputs_conf;
    counter_conf_t counter_conf;
    consigna_conf_t consigna_conf;
    modbus_conf_t modbus_conf;
    piloto_conf_t piloto_conf;
    // El checksum SIEMPRE debe ser el ultimo byte !!!!!
    uint8_t checksum;
} memConfBuffer;

    NVMEE_read( 0x00, (char *)&memConfBuffer, sizeof(memConfBuffer) );
    rd_cks = memConfBuffer.checksum;
    
    calc_cks = checksum ( (uint8_t *)&memConfBuffer, ( sizeof(memConfBuffer) - 1));
    
    if ( calc_cks != rd_cks ) {
		xprintf_P( PSTR("ERROR: Checksum systemConf failed: calc[0x%0x], read[0x%0x]\r\n"), calc_cks, rd_cks );
        
		return(false);
	}
    
    // Desarmo el buffer de memoria
    memcpy( systemConf.ptr_base_conf, &memConfBuffer.base_conf, sizeof(base_conf));
    memcpy( systemConf.ptr_ainputs_conf, &memConfBuffer.ainputs_conf, sizeof(ainputs_conf));
    memcpy( systemConf.ptr_counter_conf, &memConfBuffer.counter_conf, sizeof(counter_conf));
    memcpy( systemConf.ptr_consigna_conf, &memConfBuffer.consigna_conf, sizeof(consigna_conf));
    memcpy( systemConf.ptr_modbus_conf, &memConfBuffer.modbus_conf, sizeof(modbus_conf));
    memcpy( systemConf.ptr_piloto_conf, &memConfBuffer.piloto_conf, sizeof(piloto_conf));
    return(true);
}
//------------------------------------------------------------------------------
uint8_t checksum( uint8_t *s, uint16_t size )
{
	/*
	 * Recibe un puntero a una estructura y un tamaño.
	 * Recorre la estructura en forma lineal y calcula el checksum
	 */

uint8_t *p = NULL;
uint8_t cks = 0;
uint16_t i = 0;

	cks = 0;
	p = s;
	for ( i = 0; i < size ; i++) {
		 cks = (cks + (int)(p[i])) % 256;
	}

	return(cks);
}
//------------------------------------------------------------------------------
bool u_config_timerdial ( char *s_timerdial )
{
	// El timer dial puede ser 0 si vamos a trabajar en modo continuo o mayor a
	// 15 minutos.
	// Es una variable de 32 bits para almacenar los segundos de 24hs.

uint16_t l_timerdial;
    
    l_timerdial = atoi(s_timerdial);
    if ( (l_timerdial > 0) && (l_timerdial < TDIAL_MIN_DISCRETO ) ) {
        xprintf_P( PSTR("TDIAL warn: continuo TDIAL=0, discreto TDIAL >= 900)\r\n"));
        l_timerdial = TDIAL_MIN_DISCRETO;
    }
    
	systemConf.ptr_base_conf->timerdial = atoi(s_timerdial);
	return(true);
}
//------------------------------------------------------------------------------
bool u_config_timerpoll ( char *s_timerpoll )
{
	// Configura el tiempo de poleo.
	// Se utiliza desde el modo comando como desde el modo online
	// El tiempo de poleo debe estar entre 15s y 3600s


	systemConf.ptr_base_conf->timerpoll = atoi(s_timerpoll);

	if ( systemConf.ptr_base_conf->timerpoll < 15 )
		systemConf.ptr_base_conf->timerpoll = 15;

	if ( systemConf.ptr_base_conf->timerpoll > 3600 )
		systemConf.ptr_base_conf->timerpoll = 3600;

	return(true);
}
//------------------------------------------------------------------------------
bool u_config_dlgid ( char *s_dlgid )
{
	if ( s_dlgid == NULL ) {
		return(false);
	} else;
   
    memset(systemConf.ptr_base_conf->dlgid,'\0', sizeof(systemConf.ptr_base_conf->dlgid) );
	memcpy(systemConf.ptr_base_conf->dlgid, s_dlgid, sizeof(systemConf.ptr_base_conf->dlgid));
	systemConf.ptr_base_conf->dlgid[DLGID_LENGTH - 1] = '\0';
	return(true);
    
}
//------------------------------------------------------------------------------
bool u_config_pwrmodo ( char *s_pwrmodo )
{
    if ((strcmp_P( strupr(s_pwrmodo), PSTR("CONTINUO")) == 0) ) {
        systemConf.ptr_base_conf->pwr_modo = PWR_CONTINUO;
        return(true);
    }
    
    if ((strcmp_P( strupr(s_pwrmodo), PSTR("DISCRETO")) == 0) ) {
        systemConf.ptr_base_conf->pwr_modo = PWR_DISCRETO;
        return(true);
    }
    
    if ((strcmp_P( strupr(s_pwrmodo), PSTR("MIXTO")) == 0) ) {
        systemConf.ptr_base_conf->pwr_modo = PWR_MIXTO;
        return(true);
    }
    
    return(false);
}
//------------------------------------------------------------------------------
bool u_config_pwron ( char *s_pwron )
{
    systemConf.ptr_base_conf->pwr_hhmm_on = atoi(s_pwron);
    return(true);
}
//------------------------------------------------------------------------------
bool u_config_pwroff ( char *s_pwroff )
{
    systemConf.ptr_base_conf->pwr_hhmm_off = atoi(s_pwroff);
    return(true);
}
//------------------------------------------------------------------------------
void u_print_pwr_configuration(void)
{
    /*
     * Muestra en pantalla el modo de energia configurado
     */
    
uint16_t hh, mm;
    
    switch( systemConf.ptr_base_conf->pwr_modo ) {
        case PWR_CONTINUO:
            xprintf_P(PSTR(" pwr_modo: continuo\r\n"));
            break;
        case PWR_DISCRETO:
            xprintf_P(PSTR(" pwr_modo: discreto (%d s)\r\n"), systemConf.ptr_base_conf->timerdial);
            break;
        case PWR_MIXTO:
            xprintf_P(PSTR(" pwr_modo: mixto\r\n"));
            hh = (uint8_t)(systemConf.ptr_base_conf->pwr_hhmm_on / 100);
            mm = (uint8_t)(systemConf.ptr_base_conf->pwr_hhmm_on % 100);
            xprintf_P(PSTR("    inicio continuo -> %02d:%02d\r\n"), hh,mm);
            
            hh = (uint8_t)(systemConf.ptr_base_conf->pwr_hhmm_off / 100);
            mm = (uint8_t)(systemConf.ptr_base_conf->pwr_hhmm_off % 100);
            xprintf_P(PSTR("    inicio discreto -> %02d:%02d\r\n"), hh,mm);
            break;
    }
    xprintf_P(PSTR(" pwr_on:%d, pwr_off:%d\r\n"),systemConf.ptr_base_conf->pwr_hhmm_on, systemConf.ptr_base_conf->pwr_hhmm_off );
}
//------------------------------------------------------------------------------
float u_read_bat3v3(bool debug)
{

uint16_t adc = 0;
float bat3v3 = 0.0;
uint8_t i;

    /*
     Como acumulo en el ADC 8 samples, el resultado debo dividirlo /8
     */

    SET_EN_SENS3V3();
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    SYSTEM_ENTER_CRITICAL();
    
    for (i=0; i < BAT_SAMPLES; i++) {
        adc = ADC_read_sens3v3();
        bat3v3 += 1.0 * adc;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
    bat3v3 /= 8;
    bat3v3 /= BAT_SAMPLES;
    
    SYSTEM_EXIT_CRITICAL();
    bat3v3 *= BAT3V3_FACTOR;
    adc /= 8;
            
    CLEAR_EN_SENS3V3();
    if(debug) {
        xprintf_P(PSTR("BAT3v3: adc=%d, bat3v3=%0.2f\r\n"), adc, bat3v3);
    }
    return (bat3v3);
}
//------------------------------------------------------------------------------
float u_read_bat12v(bool debug)
{
uint16_t adc = 0;
float bat12v = 0.0;
uint8_t i;

    /*
     Como acumulo en el ADC 8 samples, el resultado debo dividirlo /8
     */

    SET_EN_SENS12V();
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    SYSTEM_ENTER_CRITICAL();
    
    for (i=0; i < BAT_SAMPLES; i++) {
        adc = ADC_read_sens12v();
        bat12v += 1.0 * adc;
        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
    bat12v /= 8;
    bat12v /= BAT_SAMPLES;
    
    SYSTEM_EXIT_CRITICAL();
    // Convierto a voltaje
    bat12v *= BAT12V_FACTOR;
    adc /= 8;
    
    CLEAR_EN_SENS12V();
    if(debug) {
        xprintf_P(PSTR("BAT12v: adc=%d, bat12v=%0.2f\r\n"), adc, bat12v);
    }
    return (bat12v);
}
//------------------------------------------------------------------------------
bool u_poll_data(dataRcd_s *dataRcd)
{
    /*
     * Se encarga de leer los datos.
     * Lo hacemos aqui asi es una funcion que se puede invocar desde Cmd.
     */
bool f_status;
uint8_t channel;
float mag;
uint16_t raw;
bool retS = false;
counter_value_t cnt;

    // Canales analogicos:
    ainputs_prender_sensores();
    
    for ( channel = 0; channel < NRO_ANALOG_CHANNELS; channel++) {
        mag = 0.0;
        raw = 0;
        // Solo leo los canales configurados.
        if ( systemConf.ptr_ainputs_conf->channel[channel].enabled ) {
            ainputs_read_channel ( channel, &mag, &raw );
        }
        dataRcd->ainputs[channel] = mag;
    }
    
    ainputs_apagar_sensores();
    
    // Contador
    cnt = counter_read();
    counter_clear();
    if ( systemConf.ptr_counter_conf->enabled ) {
        if ( systemConf.ptr_counter_conf->modo_medida == PULSOS ) {
            dataRcd->contador = (float) cnt.pulsos;
        } else {
            dataRcd->contador = cnt.caudal;
        }      
    }
    
    // Modbus 
    modbus_read ( dataRcd->modbus );
    
    // Bateria
    dataRcd->bt3v3 = u_read_bat3v3(false);
    dataRcd->bt12v = u_read_bat12v(false);
    
    // Agrego el timestamp.
    f_status = RTC_read_dtime( &dataRcd->rtc );
    if ( ! f_status ) {
        xprintf_P(PSTR("u_poll_data: ERROR I2C RTC:data_read_inputs\r\n"));
        retS = false;
        goto quit;
    }
    
    // Control de errores
    // 1- Clock:
    if ( dataRcd->rtc.year == 0) {
        xprintf_P(PSTR("u_poll_data: DATA ERROR: byClock\r\n"));
        retS = false;
        goto quit;
    }
        
    retS = true;
    
quit:
            
    return(retS);
     
}
//------------------------------------------------------------------------------
void u_xprint_dr(dataRcd_s *dr)
{
    /*
     * Imprime en pantalla el dataRcd pasado
     */
    
uint8_t i;

    xprintf_P( PSTR("ID=%s;TYPE=%s;VER=%s;"), systemConf.ptr_base_conf->dlgid, FW_TYPE, FW_REV);
 
    // Clock
    xprintf_P( PSTR("DATE=%02d%02d%02d;"), dr->rtc.year, dr->rtc.month, dr->rtc.day );
    xprintf_P( PSTR("TIME=%02d%02d%02d;"), dr->rtc.hour, dr->rtc.min, dr->rtc.sec);
    
    // Canales Analogicos:
    for ( i=0; i < NRO_ANALOG_CHANNELS; i++) {
        if ( systemConf.ptr_ainputs_conf->channel[i].enabled ) {
            xprintf_P( PSTR("%s=%0.2f;"), systemConf.ptr_ainputs_conf->channel[i].name, dr->ainputs[i]);
        }
    }
        
    // Contador
    if ( systemConf.ptr_counter_conf->enabled) {
        if ( systemConf.ptr_counter_conf->modo_medida == PULSOS ) {
            xprintf_P( PSTR("%s=%d;"), systemConf.ptr_counter_conf->name, (uint16_t)(dr->contador) );
        } else {
            xprintf_P( PSTR("%s=%0.3f;"), systemConf.ptr_counter_conf->name, dr->contador);
        }
    }
      
    // Canales Modbus:
    for ( i=0; i < NRO_MODBUS_CHANNELS; i++) {
        if ( systemConf.ptr_modbus_conf->mbch[i].enabled ) {
            xprintf_P( PSTR("%s=%0.2f;"), systemConf.ptr_modbus_conf->mbch[i].name, dr->modbus[i]);
        }
    }
    
    // Bateria
    xprintf_P( PSTR("bt3v3=%0.2f;bt12v=%0.2f"), dr->bt3v3, dr->bt12v);
    
    xprintf_P( PSTR("\r\n"));
}
//------------------------------------------------------------------------------
void SYSTEM_ENTER_CRITICAL(void)
{
    while ( xSemaphoreTake( sem_SYSVars, ( TickType_t ) 5 ) != pdTRUE )
  		vTaskDelay( ( TickType_t)( 10 ) );   
}
//------------------------------------------------------------------------------
void SYSTEM_EXIT_CRITICAL(void)
{
    xSemaphoreGive( sem_SYSVars );
}
//------------------------------------------------------------------------------
void u_data_resync_clock( char *str_time, bool force_adjust)
{
	/*
	 * Ajusta el clock interno de acuerdo al valor de rtc_s
     * Si force_adjust es TRUE siempre lo ajusta.
     * Si es FALSE, solo lo ajusta si la diferencia con la hora actual son mas
     * de 90 segundos
     * 
	 * Bug 01: 2021-12-14:
	 * El ajuste no considera los segundos entonces si el timerpoll es c/15s, cada 15s
	 * se reajusta y cambia la hora del datalogger.
	 * Modifico para que el reajuste se haga si hay una diferencia de mas de 90s entre
	 * el reloj local y el del server
	 */


float diff_seconds;
RtcTimeType_t rtc_l, rtc_wan;
int8_t xBytes = 0;
   
    // Convierto el string YYMMDDHHMM a RTC.
    //xprintf_P(PSTR("DATA: DEBUG CLOCK2\r\n") );
    memset( &rtc_wan, '\0', sizeof(rtc_wan) );        
    RTC_str2rtc( str_time, &rtc_wan);
    //xprintf_P(PSTR("DATA: DEBUG CLOCK3\r\n") );
            
            
	if ( force_adjust ) {
		// Fuerzo el ajuste.( al comienzo )
		xBytes = RTC_write_dtime(&rtc_wan);		// Grabo el RTC
		if ( xBytes == -1 ) {
			xprintf_P(PSTR("ERROR: CLOCK: I2C:RTC:pv_process_server_clock\r\n"));
		} else {
			xprintf_P( PSTR("CLOCK: Update rtc.\r\n") );
		}
		return;
	}

	// Solo ajusto si la diferencia es mayor de 90s
	// Veo la diferencia de segundos entre ambos.
	// Asumo yy,mm,dd iguales
	// Leo la hora actual del datalogger
	RTC_read_dtime( &rtc_l);
	diff_seconds = abs( rtc_l.hour * 3600 + rtc_l.min * 60 + rtc_l.sec - ( rtc_wan.hour * 3600 + rtc_wan.min * 60 + rtc_wan.sec));
	//xprintf_P( PSTR("COMMS: rtc diff=%.01f\r\n"), diff_seconds );

	if ( diff_seconds > 90 ) {
		// Ajusto
		xBytes = RTC_write_dtime(&rtc_wan);		// Grabo el RTC
		if ( xBytes == -1 ) {
			xprintf_P(PSTR("ERROR: CLOCK: I2C:RTC:pv_process_server_clock\r\n"));
		} else {
			xprintf_P( PSTR("CLOCK: Update rtc\r\n") );
		}
		return;
	}
}
//------------------------------------------------------------------------------
void u_reset_memory_remote(void)
{
    /*
     * Desde el servidor podemos mandar resetear la memoria cuando detectamos
     * problemas como fecha/hora en 0 o valores incorrectos.
     * Se debe mandar 'RESMEM'
     * 
     * Debemos primero suspender las tareas que pueden llegar a usar la memoria
     * para no interferir en ella.
     */
          
    vTaskSuspend( xHandle_tkSys );
    vTaskSuspend( xHandle_tkWanRX );
    vTaskSuspend( xHandle_tkWan );
        
    //FS_format(true);
    
    xprintf("Reset..\r\n");
    reset();
    
}
//------------------------------------------------------------------------------
uint8_t u_confbase_hash(void)
{
   
uint8_t hash_buffer[32];
uint8_t hash = 0;
char *p;

    // Calculo el hash de la configuracion base
    memset(hash_buffer, '\0', sizeof(hash_buffer));
    sprintf_P( (char *)&hash_buffer, PSTR("[TIMERPOLL:%03d]"), systemConf.ptr_base_conf->timerpoll );
    p = (char *)hash_buffer;
    while (*p != '\0') {
		hash = u_hash(hash, *p++);
	}
    //xprintf_P(PSTR("HASH_BASE:<%s>, hash=%d\r\n"),hash_buffer, hash );
    //
    memset(hash_buffer, '\0', sizeof(hash_buffer));
    sprintf_P( (char *)&hash_buffer, PSTR("[TIMERDIAL:%03d]"), systemConf.ptr_base_conf->timerdial );
    p = (char *)hash_buffer;
    while (*p != '\0') {
		hash = u_hash(hash, *p++);
	}
    //xprintf_P(PSTR("HASH_BASE:<%s>, hash=%d\r\n"),hash_buffer, hash );    
    //
    memset(hash_buffer, '\0', sizeof(hash_buffer));
    sprintf_P( (char *)&hash_buffer, PSTR("[PWRMODO:%d]"), systemConf.ptr_base_conf->pwr_modo );
    p = (char *)hash_buffer;
    while (*p != '\0') {
		hash = u_hash(hash, *p++);
	}
    //xprintf_P(PSTR("HASH_BASE:<%s>, hash=%d\r\n"),hash_buffer, hash );
    //
    memset(hash_buffer, '\0', sizeof(hash_buffer));
    sprintf_P( (char *)&hash_buffer, PSTR("[PWRON:%04d]"), systemConf.ptr_base_conf->pwr_hhmm_on );
    p = (char *)hash_buffer;
    while (*p != '\0') {
		hash = u_hash(hash, *p++);
	}
    //xprintf_P(PSTR("HASH_BASE:<%s>, hash=%d\r\n"),hash_buffer, hash );
    //
    memset(hash_buffer, '\0', sizeof(hash_buffer));
    sprintf_P( (char *)&hash_buffer, PSTR("[PWROFF:%04d]"), systemConf.ptr_base_conf->pwr_hhmm_off );
    p = (char *)hash_buffer;
    while (*p != '\0') {
		hash = u_hash(hash, *p++);
	}
    //xprintf_P(PSTR("HASH_BASE:<%s>, hash=%d\r\n"),hash_buffer, hash );
    //  
    return(hash);
}
//------------------------------------------------------------------------------
bool u_config_debug( char *tipo, char *valor)
{
    /*
     * Configura las flags de debug para ayudar a visualizar los problemas
     * ainput,counter,modbus,piloto,wan
     */
    
    if (!strcmp_P( strupr(tipo), PSTR("NONE")) ) {
        ainputs_config_debug(false);
        counter_config_debug(false);
        return(true); 
    }
   
    if (!strcmp_P( strupr(tipo), PSTR("AINPUT")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            ainputs_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            ainputs_config_debug(false);
            return(true);
        }
    }

    if (!strcmp_P( strupr(tipo), PSTR("COUNTER")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            counter_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            counter_config_debug(false);
            return(true);
        }
    }
    
    if (!strcmp_P( strupr(tipo), PSTR("MODBUS")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            modbus_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            modbus_config_debug(false);
            return(true);
        }
    }
    
    if (!strcmp_P( strupr(tipo), PSTR("PILOTO")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            piloto_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            piloto_config_debug(false);
            return(true);
        }
    }
    
    if (!strcmp_P( strupr(tipo), PSTR("WAN")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            WAN_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            WAN_config_debug(false);
            return(true);
        }
    }
       
    return(false);
    
}
//------------------------------------------------------------------------------
void u_print_tasks_running(void)
{
    xprintf_P(PSTR(" watchdogs: (0x%02x)\r\n"), sys_watchdog);
    xprintf_P(PSTR(" task running: (0x%02x)"), task_running);
    
    if ( (task_running & CMD_WDG_gc ) != 0 ) {
        xprintf_P(PSTR(" cmd"));
    }
    
    if ( (task_running & SYS_WDG_gc ) != 0 ) {
        xprintf_P(PSTR(" sys"));
    }
    
    if ( (task_running & WAN_WDG_gc ) != 0 ) {
        xprintf_P(PSTR(" wan"));
    }
    
    if ( (task_running & WANRX_WDG_gc ) != 0 ) {
        xprintf_P(PSTR(" wanrx"));
    }
    
    xprintf_P(PSTR("\r\n"));
    
}
//------------------------------------------------------------------------------
uint16_t u_hhmm_to_mins(uint16_t hhmm)
{
    /*
     * Convierte una hora (hhmm) en minutos (0..1440)
     */
    
uint16_t hh,mm, mins;

    hh = (uint16_t) (hhmm / 100);
    mm = hhmm - hh * 100;
    mins = hh*60 + mm;
    return (mins);
    
}
//------------------------------------------------------------------------------
void u_check_stacks_usage(void)
{
    /*
     * Mide el stack de todas las tareas y lo informa
     */
    
uint16_t uxHighWaterMark;

    uxHighWaterMark = SPYuxTaskGetStackHighWaterMark( xHandle_tkCmd );
    xprintf_P(PSTR("tkCMD stack = %d\r\n"), uxHighWaterMark );

    uxHighWaterMark = SPYuxTaskGetStackHighWaterMark( xHandle_tkSys );
    xprintf_P(PSTR("tkSYS stack = %d\r\n"), uxHighWaterMark );
    
    uxHighWaterMark = SPYuxTaskGetStackHighWaterMark( xHandle_tkWanRX );
    xprintf_P(PSTR("tkWANrx stack = %d\r\n"), uxHighWaterMark );
    
    uxHighWaterMark = SPYuxTaskGetStackHighWaterMark( xHandle_tkWan );
    xprintf_P(PSTR("tkWAN stack = %d\r\n"), uxHighWaterMark );
    
    uxHighWaterMark = SPYuxTaskGetStackHighWaterMark( xHandle_tkRS485RX );
    xprintf_P(PSTR("tkRS485RX stack = %d\r\n"), uxHighWaterMark );
    
}
//------------------------------------------------------------------------------