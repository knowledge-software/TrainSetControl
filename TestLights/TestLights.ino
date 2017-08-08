
#define SIG05  7
#define SIG02  6
#define SIG06  5
#define SIG03  4
#define SIG04  3
#define SIG01  2
#define GREEN  LOW
#define RED    HIGH


// Wiring Core Includes

// Wiring Core Includes
//#include "WConstants.h"




void setup()
{
    pinMode( SIG05, OUTPUT );
    pinMode( SIG02, OUTPUT );
    pinMode( SIG01, OUTPUT );
    pinMode( 4, OUTPUT );
    pinMode( 3, OUTPUT );
    pinMode( 2, OUTPUT );
}


void loop()
{
  digitalWrite( SIG01, RED );

  delay( 1000 );
  
  digitalWrite( SIG01, GREEN );
  
  delay(1000);
  
}
