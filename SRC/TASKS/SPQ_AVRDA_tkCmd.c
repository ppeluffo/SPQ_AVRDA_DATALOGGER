
#include "SPQ_AVRDA.h"
#include "frtos_cmd.h"

static void cmdClsFunction(void);
static void cmdHelpFunction(void);
static void cmdResetFunction(void);
static void cmdStatusFunction(void);
static void cmdWriteFunction(void);
static void cmdReadFunction(void);
static void cmdConfigFunction(void);
static void cmdTestFunction(void);

static void pv_snprintfP_OK(void );
static void pv_snprintfP_ERR(void );

static bool test_valve( char *action);

//------------------------------------------------------------------------------
void tkCmd(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

    while ( ! starting_flag )
        vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );

	//vTaskDelay( ( TickType_t)( 500 / portTICK_PERIOD_MS ) );

uint8_t c = 0;
//uint16_t sleep_timeout;

    FRTOS_CMD_init();

    FRTOS_CMD_register( "cls", cmdClsFunction );
	FRTOS_CMD_register( "help", cmdHelpFunction );
    FRTOS_CMD_register( "reset", cmdResetFunction );
    FRTOS_CMD_register( "status", cmdStatusFunction );
    FRTOS_CMD_register( "write", cmdWriteFunction );
    FRTOS_CMD_register( "read", cmdReadFunction );
    FRTOS_CMD_register( "config", cmdConfigFunction );
    FRTOS_CMD_register( "test", cmdTestFunction );
    
    xprintf_P(PSTR("Starting tkCmd..\r\n" ));
    xprintf_P(PSTR("Spymovil %s %s %s %s \r\n") , HW_MODELO, FRTOS_VERSION, FW_REV, FW_DATE);
      
	// loop
	for( ;; )
	{
        kick_wdt(CMD_WDG_bp);
         
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 10ms. lo que genera la espera.
		//while ( frtos_read( fdTERM, (char *)&c, 1 ) == 1 ) {
        while ( xgetc( (char *)&c ) == 1 ) {
            FRTOS_CMD_process(c);
        }
        
        // Espero 10ms si no hay caracteres en el buffer
        vTaskDelay( ( TickType_t)( 10 / portTICK_PERIOD_MS ) );
               
	}    
}
//------------------------------------------------------------------------------
static void cmdTestFunction(void)
{

    FRTOS_CMD_makeArgv();

    // test lte (dcin,vcap,pwr,reset,reload} {on|off}
    if (!strcmp_P( strupr(argv[1]), PSTR("LTE"))  ) {
      
        // Link
        if (!strcmp_P( strupr(argv[2]), PSTR("LINK"))  ) {
            lte_test_link();
            pv_snprintfP_OK();
            return;
        }
        // Prender | apagar
        if (!strcmp_P( strupr(argv[2]), PSTR("ON"))  ) {
            LTE_prender();
            pv_snprintfP_OK();
            return;
        }    
        
        if (!strcmp_P( strupr(argv[2]), PSTR("OFF"))  ) {
            LTE_apagar();
            pv_snprintfP_OK();
            return;
        } 
                    
        if (!strcmp_P( strupr(argv[2]), PSTR("RELOAD"))  ) {
               
            if (!strcmp_P( strupr(argv[3]), PSTR("ON"))  ) {
                SET_LTE_RELOAD();
                pv_snprintfP_OK();
                return;
            }        
            if (!strcmp_P( strupr(argv[3]), PSTR("OFF"))  ) {
                CLEAR_LTE_RELOAD();
                pv_snprintfP_OK();
                return;
            } 
            pv_snprintfP_ERR();
            return;
        }
                
        if (!strcmp_P( strupr(argv[2]), PSTR("RESET"))  ) {
               
            if (!strcmp_P( strupr(argv[3]), PSTR("ON"))  ) {
                SET_LTE_RESET();
                pv_snprintfP_OK();
                return;
            }        
            if (!strcmp_P( strupr(argv[3]), PSTR("OFF"))  ) {
                CLEAR_LTE_RESET();
                pv_snprintfP_OK();
                return;
            } 
            pv_snprintfP_ERR();
            return;
        }
                
        if (!strcmp_P( strupr(argv[2]), PSTR("PWR"))  ) {
               
            if (!strcmp_P( strupr(argv[3]), PSTR("ON"))  ) {
                SET_LTE_PWR();
                pv_snprintfP_OK();
                return;
            }        
            if (!strcmp_P( strupr(argv[3]), PSTR("OFF"))  ) {
                CLEAR_LTE_PWR();
                pv_snprintfP_OK();
                return;
            } 
            pv_snprintfP_ERR();
            return;
        }
                
        if (!strcmp_P( strupr(argv[2]), PSTR("VCAP"))  ) {
               
            if (!strcmp_P( strupr(argv[3]), PSTR("ON"))  ) {
                SET_LTE_EN_VCAP();
                pv_snprintfP_OK();
                return;
            }        
            if (!strcmp_P( strupr(argv[3]), PSTR("OFF"))  ) {
                CLEAR_LTE_EN_VCAP();
                pv_snprintfP_OK();
                return;
            } 
            pv_snprintfP_ERR();
            return;
        }
                
        if (!strcmp_P( strupr(argv[2]), PSTR("DCIN"))  ) {
               
            if (!strcmp_P( strupr(argv[3]), PSTR("ON"))  ) {
                SET_LTE_EN_DCIN();
                pv_snprintfP_OK();
                return;
            }        
            if (!strcmp_P( strupr(argv[3]), PSTR("OFF"))  ) {
                CLEAR_LTE_EN_DCIN();
                pv_snprintfP_OK();
                return;
            } 
            pv_snprintfP_ERR();
            return;
        }
        
        pv_snprintfP_ERR();
        return;
    }
    
    // test cpctl,sens3v3, sens12V, pwr_sensors{enable|disable}
    if (!strcmp_P( strupr(argv[1]), PSTR("PWR_SENSORS"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_PWR_SENSORS();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_PWR_SENSORS();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    if (!strcmp_P( strupr(argv[1]), PSTR("CPCTL"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_PWR_CPRES();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_PWR_CPRES();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }

    if (!strcmp_P( strupr(argv[1]), PSTR("SENS3V3"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_SENS3V3();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_SENS3V3();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    if (!strcmp_P( strupr(argv[1]), PSTR("SENS12V"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_SENS12V();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_SENS12V();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    // pwr_cpres,pwr_sensext,pwr_qmbus {enable|disable}
    if (!strcmp_P( strupr(argv[1]), PSTR("PWR_CPRES"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_PWR_CPRES();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_PWR_CPRES();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    if (!strcmp_P( strupr(argv[1]), PSTR("PWR_SENSEXT"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_PWR_SENSEXT();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_PWR_SENSEXT();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }

    if (!strcmp_P( strupr(argv[1]), PSTR("PWR_QMBUS"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ENABLE"))  ) {
            SET_EN_PWR_QMBUS();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("DISABLE"))  ) {
            CLEAR_EN_PWR_QMBUS();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    // rtx {on|off}
    if (!strcmp_P( strupr(argv[1]), PSTR("RTS"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("ON"))  ) {
            SET_RTS_RS485();
            pv_snprintfP_OK();
            return;
        }        

        if (!strcmp_P( strupr(argv[2]), PSTR("OFF"))  ) {
            CLEAR_RTS_RS485();
            pv_snprintfP_OK();
            return;
        } 
        
        pv_snprintfP_ERR();
        return;
    }
    
    //--------------------------------------------------------------------------
    if (!strcmp_P( strupr(argv[1]), PSTR("VALVE"))  ) {
        if ( test_valve(argv[2]) ) {
            pv_snprintfP_OK();
        } else {
            pv_snprintfP_ERR();
        }
        return;
    }
        
    // test kill {sys}
    if (!strcmp_P( strupr(argv[1]), PSTR("KILL"))  ) {
               
        if (!strcmp_P( strupr(argv[2]), PSTR("SYS"))  ) {
            if ( xHandle_tkSys != NULL ) {
                vTaskSuspend( xHandle_tkSys );
                xHandle_tkSys = NULL;
            }
            pv_snprintfP_OK();
            return;
        }        

        pv_snprintfP_ERR();
        return;
    }
    
    pv_snprintfP_ERR();
    return;
       
}
//------------------------------------------------------------------------------
static void cmdHelpFunction(void)
{

    FRTOS_CMD_makeArgv();
        
    if ( !strcmp_P( strupr(argv[1]), PSTR("WRITE"))) {
		xprintf_P( PSTR("-write:\r\n"));
        xprintf_P( PSTR("  (ee,nvmee,rtcram) {pos string} {debug}\r\n"));
        xprintf_P( PSTR("  rtc YYMMDDhhmm\r\n"));
        xprintf_P( PSTR("  ina {confValue}\r\n"));
        xprintf_P( PSTR("  ain_sensors_pwr {on|off}\r\n"));
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("READ"))) {
		xprintf_P( PSTR("-read:\r\n"));
        xprintf_P( PSTR("  (ee,nvmee,rtcram) {pos} {lenght} {debug}\r\n"));
        xprintf_P( PSTR("  avrid,rtc {long,short}\r\n"));
        xprintf_P( PSTR("  sens3v3,sens12v\r\n"));
        xprintf_P( PSTR("  ina {conf|chXshv|chXbusv|mfid|dieid}\r\n"));
        xprintf_P( PSTR("  ainput {n}\r\n"));
        xprintf_P( PSTR("  cnt, cnt_pin\r\n"));
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("CONFIG"))) {
		xprintf_P( PSTR("-config:\r\n"));
        xprintf_P( PSTR("  dlgid\r\n"));
        xprintf_P( PSTR("  default, save\r\n"));
        xprintf_P( PSTR("  timerpoll, timerdial\r\n"));
        xprintf_P( PSTR("  pwrmodo {continuo,discreto,mixto}, pwron {hhmm}, pwroff {hhmm}\r\n"));
        xprintf_P( PSTR("  ainput {0..%d} enable{true/false} aname imin imax mmin mmax offset\r\n"),( NRO_ANALOG_CHANNELS - 1 ) );
        
    	// HELP RESET
	} else if (!strcmp_P( strupr(argv[1]), PSTR("RESET"))) {
		xprintf_P( PSTR("-reset\r\n"));
        xprintf_P( PSTR("  memory {soft|hard}\r\n"));
		return;
        
    } else if (!strcmp_P( strupr(argv[1]), PSTR("TEST"))) {
		xprintf_P( PSTR("-test\r\n"));
        xprintf_P( PSTR("  kill {wan,sys}\r\n"));
        xprintf_P( PSTR("  valve {open|close}\r\n"));
        xprintf_P( PSTR("        {enable|disable}\r\n"));
        xprintf_P( PSTR("  sens3v3, sens12V, pwr_sensors {enable|disable}\r\n"));
        xprintf_P( PSTR("  pwr_cpres,pwr_sensext,pwr_qmbus {enable|disable}\r\n"));
        xprintf_P( PSTR("  rts {on|off}\r\n"));
        xprintf_P( PSTR("  lte (dcin,vcap,pwr,reset,reload} {on|off}\r\n"));
        xprintf_P( PSTR("      {on|off}\r\n"));
        xprintf_P( PSTR("      link\r\n"));
        return;
        
    }  else {
        // HELP GENERAL
        xprintf("Available commands are:\r\n");
        xprintf("-cls\r\n");
        xprintf("-help\r\n");
        xprintf("-status\r\n");
        xprintf("-reset\r\n");
        xprintf("-write...\r\n");
        xprintf("-config...\r\n");
        xprintf("-read...\r\n");

    }
   
	xprintf("Exit help \r\n");

}
//------------------------------------------------------------------------------
static void cmdReadFunction(void)
{
    
    FRTOS_CMD_makeArgv();       
    
    // CONTADOR
    // read cnt
	if (!strcmp_P( strupr(argv[1]), PSTR("CNT")) ) {
		xprintf_P(PSTR("CNT=%d\r\n"), counter_read());
        pv_snprintfP_OK();
		return;
	} 

    // read cnt_pin
	if (!strcmp_P( strupr(argv[1]), PSTR("CNT_PIN")) ) {
		xprintf_P(PSTR("CNT=%d\r\n"), counter_read_pin());
        pv_snprintfP_OK();
		return;
	} 
    
    // AINPUT
    // read ainput {n}
    if (!strcmp_P( strupr(argv[1]), PSTR("AINPUT"))  ) {
        ainputs_test_read_channel( atoi(argv[2]) ) ? pv_snprintfP_OK(): pv_snprintfP_ERR();
		return;
	}

    // EE
	// read ee address length
	if (!strcmp_P( strupr(argv[1]), PSTR("EE")) ) {
		EE_test_read ( argv[2], argv[3], argv[4] );
		return;
	}
    
    // RTC
	// read rtc { long | short } 
    if (!strcmp_P( strupr(argv[1]), PSTR("RTC")) ) {
        if (!strcmp_P( strupr(argv[2]), PSTR("LONG")) ) {
            RTC_read_time(FORMAT_LONG);
            pv_snprintfP_OK();
            return;
        }
        if (!strcmp_P( strupr(argv[2]), PSTR("SHORT")) ) {
            RTC_read_time(FORMAT_SHORT);
            pv_snprintfP_OK();
            return;
        }
        pv_snprintfP_ERR();
        return;
    }
 
    // INA
	// read ina regName
	if (!strcmp_P( strupr(argv[1]), PSTR("INA"))  ) {
		INA_test_read ( argv[2] );
		return;
	}
    
    // SENS3V3
    // read sens3v3
	if (!strcmp_P( strupr(argv[1]), PSTR("SENS3V3")) ) {
		xprintf_P(PSTR("SENS3V3=%d\r\n"), ADC_read_sens3v3() );
        pv_snprintfP_OK();
		return;
	} 

    // SENS12V
    // read sens12v
	if (!strcmp_P( strupr(argv[1]), PSTR("SENS12V")) ) {
		xprintf_P(PSTR("SENS12V=%d\r\n"), ADC_read_sens12v() );
        pv_snprintfP_OK();
		return;
	} 
        
    // NVMEE
	// read nvmee address length
	if (!strcmp_P( strupr(argv[1]), PSTR("NVMEE")) ) {
		NVMEE_test_read ( argv[2], argv[3] );
		return;
	}

	// AVRID
	// read avrid
	if (!strcmp_P( strupr(argv[1]), PSTR("AVRID"))) {
		//nvm_read_print_id();
        xprintf_P(PSTR("ID: %s\r\n"), NVM_id2str() );
        xprintf_P(PSTR("SIGNATURE: %s\r\n"), NVM_signature2str() );
		return;
	}
    
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdClsFunction(void)
{
	// ESC [ 2 J
	xprintf("\x1B[2J\0");
}
//------------------------------------------------------------------------------
static void cmdResetFunction(void)
{
    
    FRTOS_CMD_makeArgv();
    
    xprintf("Reset..\r\n");
    reset();
}
//------------------------------------------------------------------------------
static void cmdStatusFunction(void)
{

    // https://stackoverflow.com/questions/12844117/printing-defined-constants


    xprintf("Spymovil %s %s TYPE=%s, VER=%s %s \r\n" , HW_MODELO, FRTOS_VERSION, FW_TYPE, FW_REV, FW_DATE);
      
    xprintf_P(PSTR("Config:\r\n"));
    xprintf_P(PSTR(" date: %s\r\n"), RTC_logprint(FORMAT_LONG));
    xprintf_P(PSTR(" dlgid: %s\r\n"), systemConf.ptr_base_conf->dlgid );
    xprintf_P(PSTR(" timerdial=%d\r\n"), systemConf.ptr_base_conf->timerdial);
    xprintf_P(PSTR(" timerpoll=%d\r\n"), systemConf.ptr_base_conf->timerpoll);
    u_print_pwr_configuration();
    
    ainputs_print_configuration();
    counter_print_configuration();
}
//------------------------------------------------------------------------------
static void cmdWriteFunction(void)
{

    FRTOS_CMD_makeArgv();
        
    // ANALOG SENSORS PWR
    // write ain_sensors_pwr {on|off}
    if ((strcmp_P( strupr(argv[1]), PSTR("AIN_SENSORS_PWR")) == 0) ) {
        if ((strcmp_P( strupr(argv[2]), PSTR("ON")) == 0) ) {
            ainputs_prender_sensores();
            pv_snprintfP_OK();
            return;
        }
        
        if ((strcmp_P( strupr(argv[2]), PSTR("OFF")) == 0) ) {
            ainputs_apagar_sensores();
            pv_snprintfP_OK();
            return;
        }        
        pv_snprintfP_ERR();
        return;
    }

    // EE
	// write ee pos string
	if ((strcmp_P( strupr(argv[1]), PSTR("EE")) == 0) ) {
		( EE_test_write ( argv[2], argv[3], argv[4] ) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}

    // RTC
	// write rtc YYMMDDhhmm
	if ( strcmp_P( strupr(argv[1]), PSTR("RTC")) == 0 ) {
		( RTC_write_time( argv[2]) > 0)?  pv_snprintfP_OK() : 	pv_snprintfP_ERR();
		return;
	}

    // NVMEE
	// write nvmee pos string
	if ( (strcmp_P( strupr(argv[1]), PSTR("NVMEE")) == 0)) {
		NVMEE_test_write ( argv[2], argv[3] );
		pv_snprintfP_OK();
		return;
	}
    
    // INA
	// write ina rconfValue
	// Solo escribimos el registro 0 de configuracion.
	if ((strcmp_P( strupr(argv[1]), PSTR("INA")) == 0) ) {
        INA_awake();
		( INA_test_write ( argv[2] ) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
        INA_sleep();
		return;
	}
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdConfigFunction(void)
{
    
    FRTOS_CMD_makeArgv();

	// SAVE
	// config save
	if (!strcmp_P( strupr(argv[1]), PSTR("SAVE"))) {       
		u_save_config_in_NVM();
		pv_snprintfP_OK();
		return;
	}
    
    // LOAD
	// config load
	if (!strcmp_P( strupr(argv[1]), PSTR("LOAD"))) {
		u_load_config_from_NVM();
		pv_snprintfP_OK();
		return;
	}
    
    // DEFAULT
	// config default
	if (!strcmp_P( strupr(argv[1]), PSTR("DEFAULT"))) {
		u_config_default();
		pv_snprintfP_OK();
		return;
	}
    
    // POWER
    // pwr_modo {continuo,discreto,mixto}
    if (!strcmp_P( strupr(argv[1]), PSTR("PWRMODO"))) {
        u_config_pwrmodo(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // pwr_on {hhmm}
     if (!strcmp_P( strupr(argv[1]), PSTR("PWRON"))) {
        u_config_pwron(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}  
    
    // pwr_off {hhmm}
     if (!strcmp_P( strupr(argv[1]), PSTR("PWROFF"))) {
        u_config_pwroff(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}   
            
    // DLGID
	if (!strcmp_P( strupr(argv[1]), PSTR("DLGID"))) {
        u_config_dlgid(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
    }
    
    // TIMERPOLL
    // config timerpoll val
	if (!strcmp_P( strupr(argv[1]), PSTR("TIMERPOLL")) ) {
        u_config_timerpoll(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // TIMERDIAL
    // config timerdial val
	if (!strcmp_P( strupr(argv[1]), PSTR("TIMERDIAL")) ) {
		u_config_timerdial(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}

    // AINPUT
	// ainput {0..%d} enable aname imin imax mmin mmax offset
	if (!strcmp_P( strupr(argv[1]), PSTR("AINPUT")) ) {
		ainputs_config_channel ( atoi(argv[2]), argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
        pv_snprintfP_OK();
		return;
	}

    // COUNTER
    // counter enable cname magPP modo(PULSO/CAUDAL)
	if (!strcmp_P( strupr(argv[1]), PSTR("COUNTER")) ) {
        counter_config_channel( argv[2], argv[3], argv[4], argv[5] );
        pv_snprintfP_OK();
		return;
	}

    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void pv_snprintfP_OK(void )
{
	xprintf("ok\r\n\0");
}
//------------------------------------------------------------------------------
static void pv_snprintfP_ERR(void)
{
	xprintf("error\r\n\0");
}
//------------------------------------------------------------------------------
static bool test_valve( char *action)
{

    // enable, disable
    if (!strcmp_P( strupr(action), PSTR("ENABLE"))  ) {
        ENABLE_VALVE();
        return (true);
    }

    if (!strcmp_P( strupr(action), PSTR("DISABLE"))  ) {
        DISABLE_VALVE();
        return (true);
    }

    if (!strcmp_P( strupr(action), PSTR("OPEN"))  ) {
        OPEN_VALVE();
        return (true);
    }

    if (!strcmp_P( strupr(action), PSTR("CLOSE"))  ) {
        CLOSE_VALVE();
        return (true);
    }

    return(false);
}
//------------------------------------------------------------------------------