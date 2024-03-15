
#include "lte.h"

/*
 * Para configurarlo se debe usar el modulo de conversion USB.
 * Se conecta TX a JP23
 *            RX a JP22
 *            GND a JP21
 * 
 * y se configura con el programa de USRIOT
 *       
 */


t_lte_pwr_status lte_pwr_status;

//--------------------------------------------------------------------------
void LTE_init(void)
{
    CONFIG_LTE_EN_DCIN();
    CONFIG_LTE_EN_VCAP();
    CONFIG_LTE_PWR();
    CONFIG_LTE_RESET();
    CONFIG_LTE_RELOAD();
    
    CLEAR_LTE_EN_DCIN();    // No usamos DCIN
    SET_LTE_RESET();        // Reset debe estar H
    SET_LTE_RELOAD();       // Reload debe estar H
    CLEAR_LTE_EN_VCAP();    // No alimento
    CLEAR_LTE_PWR();        // No prendo
    
    lte_pwr_status = LTE_PWR_OFF;
}

//--------------------------------------------------------------------------
void LTE_prender(void)
{
    SET_LTE_EN_VCAP();     // alimento
    SET_LTE_PWR();         // pwr on
    
    lte_pwr_status = LTE_PWR_ON;
}
//--------------------------------------------------------------------------
void LTE_apagar(void)
{
    CLEAR_LTE_PWR();         
    CLEAR_LTE_EN_VCAP();     
    
    lte_pwr_status = LTE_PWR_OFF;
    
}
//--------------------------------------------------------------------------
t_lte_pwr_status LTE_get_pwr_status(void)
{
    return(lte_pwr_status);
}
//--------------------------------------------------------------------------
char *LTE_buffer_ptr(void)
{
    return ( lBchar_get_buffer(&lte_lbuffer));
}
//------------------------------------------------------------------------------
void LTE_flush_buffer(void)
{
    lBchar_Flush(&lte_lbuffer);
}
//------------------------------------------------------------------------------
void lte_test_link(void)
{
    /*
     Envio un mensaje por el enlace del LTE para verificar que llegue a la api
     */
    //xfprintf_P( fdXCOMMS, PSTR("The quick brown fox jumps over the lazy dog\r\n"));
  
uint16_t timeout = 10;   
char *p;

    LTE_flush_buffer();
    xfprintf_P( fdWAN, PSTR("CLASS=TEST&EQUIPO=SPQ_AVRDA"));
    vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
    
    p = LTE_buffer_ptr();
    while ( timeout-- > 0) {
        vTaskDelay( ( TickType_t)( 1000 / portTICK_PERIOD_MS ) );
        if  ( strstr( p, "</html>") != NULL ) {
            xprintf_P( PSTR("rxbuff-> %s\r\n"), p);
            LTE_flush_buffer();
            return;
        }
    }
     xprintf_P( PSTR("RX TIMEOUT\r\n"));
}
//------------------------------------------------------------------------------
