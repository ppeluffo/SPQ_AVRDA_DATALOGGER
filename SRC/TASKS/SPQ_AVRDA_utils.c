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
void kick_wdt( uint8_t bit_pos)
{
    // Pone el bit correspondiente en 0.
    sys_watchdog &= ~ (1 << bit_pos);
    
}
//------------------------------------------------------------------------------
void u_config_default(void)
{

    // Configuro a default todas las configuraciones locales
    // y luego actualizo el systemConf
    
    memcpy(systemConf.ptr_base_conf->dlgid, "DEFAULT\0", sizeof(systemConf.ptr_base_conf->dlgid));
    
    systemConf.ptr_base_conf->timerdial = 60;
    systemConf.ptr_base_conf->timerdial = 0;
    
    systemConf.ptr_base_conf->pwr_modo = PWR_CONTINUO;
    systemConf.ptr_base_conf->pwr_hhmm_on = 2330;
    systemConf.ptr_base_conf->pwr_hhmm_off = 630;
    
    ainputs_config_defaults();
    counter_config_defaults();

    
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
    // El checksum SIEMPRE debe ser el ultimo byte !!!!!
    uint8_t checksum;
} memConfBuffer;

    // Cargamos el buffer con las configuraciones
    memcpy( &memConfBuffer.base_conf, &base_conf, sizeof(base_conf));
    memcpy( &memConfBuffer.ainputs_conf, &ainputs_conf, sizeof(ainputs_conf));
    memcpy( &memConfBuffer.counter_conf, &counter_conf, sizeof(counter_conf));

    cks = checksum ( (uint8_t *)&memConfBuffer, ( sizeof(memConfBuffer) - 1));
    memConfBuffer.checksum = cks;
    
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
    // El checksum SIEMPRE debe ser el ultimo byte !!!!!
    uint8_t checksum;
} memConfBuffer;

    NVMEE_read( 0x00, (char *)&memConfBuffer, sizeof(memConfBuffer) );
    rd_cks = memConfBuffer.checksum;
    
    calc_cks = checksum ( (uint8_t *)&memConfBuffer, ( sizeof(memConfBuffer) - 1));
    
    if ( calc_cks != rd_cks ) {
		xprintf_P( PSTR("ERROR: Checksum systemVars failed: calc[0x%0x], read[0x%0x]\r\n"), calc_cks, rd_cks );
        
		return(false);
	}
    
    // Desarmo el buffer de memoria
    memcpy( &base_conf, &memConfBuffer.base_conf, sizeof(base_conf));
    memcpy( &ainputs_conf, &memConfBuffer.ainputs_conf, sizeof(ainputs_conf));
    memcpy( &counter_conf, &memConfBuffer.counter_conf, sizeof(counter_conf));
    
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
