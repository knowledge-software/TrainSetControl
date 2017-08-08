#include <SwitchMatrix.h>

char* labels[] = {"RPM", "ADV", "DW", "^(up)", "TEST", "v(dn)"};

SwitchMatrix  theButtons;


void setup()  {
  Serial.begin(38400);   
}


void loop()  {
  switch(  theButtons.checkButtons() ) {
    case ButtonA:
      Serial.println( "-RPM" );
      break;
    case ButtonB:
      Serial.println( "-DW" );
      break;
    case ButtonC:
      Serial.println( "-^(up)" );
      break;
    case ButtonD:
      Serial.println( "-ADV" );
      break;
    case ButtonE:
      Serial.println( "-TST" );
      break;
    case ButtonF:
      Serial.println( "-v(dn)" );
      break;
  }

  delay( 100 ); // Wait until next poll
}

