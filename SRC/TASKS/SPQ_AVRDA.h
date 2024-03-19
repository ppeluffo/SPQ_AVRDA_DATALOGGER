/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#ifndef F_CPU
#define F_CPU 24000000
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "croutine.h"
#include "semphr.h"
#include "timers.h"
#include "limits.h"
#include "portable.h"
#include "protected_io.h"
#include "ccp.h"

//#include <avr/io.h>
//#include <avr/builtins.h>
#include <avr/wdt.h> 
#include <avr/pgmspace.h>
//#include <avr/fuse.h>
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//#include "math.h"

#include "frtos-io.h"
#include "xprintf.h"
#include "xgetc.h"
#include "eeprom.h"
#include "nvm.h"
#include "led.h"
#include "pines.h"
#include "linearBuffer.h"
#include "fileSystem.h"
#include "adc.h"
#include "lte.h"
#include "i2c.h"
#include "ina3221.h"
#include "rtc79410.h"
#include "ainputs.h"
#include "contadores.h"
#include "toyi_valves.h"
#include "consignas.h"
#include "modbus.h"
#include "piloto.h"

#define FW_REV "0.0.1"
#define FW_DATE "@ 20240319"
#define HW_MODELO "SPQ_AVRDA FRTOS R001 HW:AVR128DA64"
#define FRTOS_VERSION "FW:FreeRTOS V202111.00"
#define FW_TYPE "SPQ"

#define SYSMAINCLK 24

#define tkCtl_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tkCmd_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkSys_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkWanRX_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkWan_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkRS485RX_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkCtlPresion_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )

#define tkCtl_STACK_SIZE		384
#define tkCmd_STACK_SIZE		512
#define tkSys_STACK_SIZE		512
#define tkWanRX_STACK_SIZE		384
#define tkWan_STACK_SIZE		512
#define tkRS485RX_STACK_SIZE	384
#define tkCtlPresion_STACK_SIZE	384

StaticTask_t tkCtl_Buffer_Ptr;
StackType_t tkCtl_Buffer [tkCtl_STACK_SIZE];

StaticTask_t tkCmd_Buffer_Ptr;
StackType_t tkCmd_Buffer [tkCmd_STACK_SIZE];

StaticTask_t tkSys_Buffer_Ptr;
StackType_t tkSys_Buffer [tkSys_STACK_SIZE];

StaticTask_t tkWanRX_Buffer_Ptr;
StackType_t tkWanRX_Buffer [tkWanRX_STACK_SIZE];

StaticTask_t tkWan_Buffer_Ptr;
StackType_t tkWan_Buffer [tkWan_STACK_SIZE];

StaticTask_t tkRS485RX_Buffer_Ptr;
StackType_t tkRS485RX_Buffer [tkRS485RX_STACK_SIZE];

StaticTask_t tkCtlPresion_Buffer_Ptr;
StackType_t tkCtlPresion_Buffer [tkCtlPresion_STACK_SIZE];

SemaphoreHandle_t sem_SYSVars;
StaticSemaphore_t SYSVARS_xMutexBuffer;

SemaphoreHandle_t sem_XCOMMS;
StaticSemaphore_t XCOMMS_xMutexBuffer;

#define MSTOTAKESYSVARSSEMPH ((  TickType_t ) 10 )

TaskHandle_t xHandle_tkCtl, xHandle_tkCmd, xHandle_tkSys, xHandle_tkWanRX, xHandle_tkWan, xHandle_tkRS485RX, xHandle_tkCtlPresion;

void tkCtl(void * pvParameters);
void tkCmd(void * pvParameters);
void tkSys(void * pvParameters);
void tkWanRX(void * pvParameters);
void tkWan(void * pvParameters);
void tkRS485RX(void * pvParameters);
void tkCtlPresion(void *pvParameters);

typedef enum { PWR_CONTINUO = 0, PWR_DISCRETO, PWR_MIXTO } pwr_modo_t;

#define DLGID_LENGTH		12
#define TDIAL_MIN_DISCRETO  900

#define BAT_SAMPLES 16
#define BAT3V3_FACTOR ( 2.5 * 2 / 4096 )
#define BAT12V_FACTOR ( 2.5 * 6.6 / 4096 )

bool starting_flag;

/* Estructura que tiene el valor de las medidas en el intervalo de poleo
struct {   
    bool debug;
    float ainputs[NRO_ANALOG_CHANNELS];
    float counter;
    float modbus[NRO_MODBUS_CHANNELS];
    float bt3v3;
    float bt12v;
} systemVars;
*/

typedef struct {
    char dlgid[DLGID_LENGTH];
    uint16_t timerpoll;
    uint16_t timerdial;
    pwr_modo_t pwr_modo;
    uint16_t pwr_hhmm_on;
    uint16_t pwr_hhmm_off;    
} base_conf_t;

base_conf_t base_conf;

// Estructura que tiene la configuracion del sistema
struct {
    base_conf_t *ptr_base_conf;
	ainputs_conf_t *ptr_ainputs_conf;
    counter_conf_t *ptr_counter_conf;
    consigna_conf_t *ptr_consigna_conf;
    modbus_conf_t *ptr_modbus_conf;
    piloto_conf_t *ptr_piloto_conf;
} systemConf;

// Tipo que define la estrucutra de las medidas tomadas.
typedef struct {
    float ainputs[NRO_ANALOG_CHANNELS];
    float contador;
    float modbus[NRO_MODBUS_CHANNELS];
    float bt3v3;
    float bt12v;
    RtcTimeType_t  rtc;	
} dataRcd_s;

void system_init();
void reset(void);
void u_print_pwr_configuration(void);
bool u_config_timerdial ( char *s_timerdial );
bool u_config_timerpoll ( char *s_timerpoll );
bool u_config_dlgid ( char *s_dlgid );
bool u_config_pwrmodo ( char *s_pwrmodo );
bool u_config_pwron ( char *s_pwron );
bool u_config_pwroff ( char *s_pwroff );
void u_config_default(void);
bool u_save_config_in_NVM(void);
bool u_load_config_from_NVM(void);
bool u_poll_data(dataRcd_s *dataRcd);
void u_xprint_dr(dataRcd_s *dr);
float u_read_bat3v3(bool debug);
float u_read_bat12v(bool debug);
dataRcd_s *get_dataRcd_ptr(void);
void SYSTEM_ENTER_CRITICAL(void);
void SYSTEM_EXIT_CRITICAL(void);
void u_data_resync_clock( char *str_time, bool force_adjust);
void u_reset_memory_remote(void);
uint8_t u_confbase_hash( void );
bool u_config_debug( char *tipo, char *valor);
void u_print_tasks_running(void);
void u_kick_wdt( uint8_t wdg_gc);
uint8_t u_hash(uint8_t seed, char ch );
uint16_t u_hhmm_to_mins(uint16_t hhmm);
void u_check_stacks_usage(void);
void XCOMMS_ENTER_CRITICAL(void);
void XCOMMS_EXIT_CRITICAL(void);


bool WAN_process_data_rcd( dataRcd_s *dataRcd);
void WAN_print_configuration(void);
void WAN_config_debug(bool debug );
bool WAN_read_debug(void);

// Mensajes entre tareas
#define SIGNAL_FRAME_READY		0x01

uint8_t sys_watchdog;
uint8_t task_running;

#define CMD_WDG_bp     0x01
#define SYS_WDG_bp     0x02
#define WAN_WDG_bp     0x04
#define WANRX_WDG_bp   0x08
#define RS485RX_WDG_bp  0x10
#define CTLPRES_WDG_bp  0x20

#define CMD_WDG_gc          (0x01 << 0)
#define SYS_WDG_gc          (0x01 << 1)
#define WAN_WDG_gc          (0x01 << 2)
#define WANRX_WDG_gc        (0x01 << 3)
#define RS485RX_WDG_gc      (0x01 << 4)
#define CTLPRES_WDG_gc      (0x01 << 5)

// No habilitado PLT_WDG !!!
#define WDG_bm 0x3F     // Pone todos los bits habilitados en 1
#define WDG_INIT() ( sys_watchdog = WDG_bm )

#endif	/* XC_HEADER_TEMPLATE_H */

