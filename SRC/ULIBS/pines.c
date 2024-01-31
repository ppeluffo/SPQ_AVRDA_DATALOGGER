
#include "pines.h"

// -----------------------------------------------------------------------------
uint8_t FC1_read(void)
{
   return ( ((FC1_PORT.IN) >> FC1 ) & 0x01 );
}
// -----------------------------------------------------------------------------
uint8_t FC2_read(void)
{
   return ( ((FC2_PORT.IN) >> FC2 ) & 0x01 );
}
// -----------------------------------------------------------------------------
void FCx_init(void)
{
    CONFIG_FC1();
    CONFIG_FC2();
}
// -----------------------------------------------------------------------------