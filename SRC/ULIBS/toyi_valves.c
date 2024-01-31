
#include "toyi_valves.h"

// -----------------------------------------------------------------------------
void VALVE_EN_init(void)
{
    // Configura el pin como output
	VALVE_EN_PORT.DIR |= VALVE_EN_PIN_bm;	
	DISABLE_VALVE();
}
// -----------------------------------------------------------------------------
void VALVE_CTRL_init(void)
{
    // Configura el pin como output
	VALVE_CTRL_PORT.DIR |= VALVE_CTRL_PIN_bm;	
	CLOSE_VALVE();
}
// -----------------------------------------------------------------------------
void VALVE_init(void)
{
    VALVE_EN_init();
    VALVE_CTRL_init();
}
// -----------------------------------------------------------------------------
t_valve_status *get_valve_status(void)
{
    return &valve_status;
}
// -----------------------------------------------------------------------------
