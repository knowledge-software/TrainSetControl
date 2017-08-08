#include <SwitchMatrix.h>

/*
I have a boat load of analog panel momentary contact switches.
These buttons are organized as a matrix of connections between one
line and another.  Reading the switches can be done a variety of ways
but with the analog input I can use a set of resistors to code the inputs

1 o----+---+---+
       |   |   |
2 o----+---+---+
       |   |   |
       3   4   5
tapping a button closes a connection between the horizontal and vertical 
lines.  By encoding the lines with resistors (see notebook) you can read
different voltages at pin 5.
*/

#define _SwitchPin A0

enum { ZERO=1023, RPM=745, ADV=853, DW=681, UPA=475, TST=796, DNA=596 };
char* labels[] = {"RPM", "ADV", "DW", "^(up)", "TEST", "v(dn)"};

int switchPin = A0;
int noButton;  // value of line when no button is pressed

// Learn mode allow us to store values and determine the right voltage for a particular button.
#define  LEARNMODE  0

void setup()  {
  Serial.begin(38400);  
  pinMode(switchPin,INPUT);    // set the pinmode explicitly

  if (LEARNMODE) {
    for( int j=0; j<10; j++ ) {
      int rdng = analogRead( switchPin );
      noButton = noButton + rdng;
      Serial.print( rdng ); Serial.print( " " );
      delay( 2 );
      }
    noButton = noButton /10;
      Serial.print(".   =");
      Serial.println( noButton );
    }

  
}


boolean switchChanged = false;

void loop()  {

  
  if (LEARNMODE) {
    
//   Read and average values from the ADC for each button.

    for( int i=0; i<6; i++ ) {  
      Serial.print( "Press and hold " );
      Serial.println( labels[i] );
      

      int val=0;
      for ( int j=0; j<5; j++ ) {
        int rdng = buttonHit(switchPin);
        val = val + rdng;
        Serial.print( rdng );
        Serial.print( " " );
        delay( 4 );
        }
      val = val/5;
      Serial.print(".   =");
      Serial.println(val);
      delay( 5000 );
      }
    }

    else {
  
//   Test the button reading.  Return values which index the buttons.
          int value = averageSignal();   
          if (  value<900 ){
            Serial.print( "Button[" );
            if ( isSignalNear( value, RPM ) ) Serial.print( "RPM" );
            if ( isSignalNear( value, ADV ) ) Serial.print( "ADV" );
            if ( isSignalNear( value, DW ) ) Serial.print( "DW" );
            if ( isSignalNear( value, UPA ) ) Serial.print( "UPA" );
            if ( isSignalNear( value, TST ) ) Serial.print( "TST" );
            if ( isSignalNear( value, DNA ) ) Serial.print( "DNA" );
            Serial.println( "] " );
          }

        delay( 100 ); // Wait until next poll
        }
}

bool isSignalNear( int value, int limit )
{
  return abs(value-limit)<5;    
}


int averageSignal()
{
  int val = analogRead( _SwitchPin );
  for( int i=0; i<6; i++ ) {
    delay( 3 ); // 1 ms delay
    val += analogRead( _SwitchPin );
    }      
  return val/6;
}

//
// The following function tests to see if the value of the button changed 
//  (ie a button was pressed) and returns the voltage.
int buttonHit( int pin )  {
  
// Read and average values from ADC if different than "no button pressed" value
  if (LEARNMODE) { 
    int val = analogRead( pin );
    while( abs( val-noButton ) < 5 ) {
        val = analogRead( pin );
        }
     return val;
     }

// Read ADC and use voltage o index into the stored values.
   else {

    int switchVoltage[] = {ZERO, RPM, ADV, DW, UPA, TST, DNA};

     int val = analogRead( pin );
     // Debounce the button a bit.  Take a couple of readings a few ms apart
     for( int i=0; i<6; i++ ) {
         delay( 2 ); // 1 ms delay
         val = val+analogRead( pin );
     }      
      
    // Sensing on Pin 1
    for (int i=1;i<7;i++){
      if (abs(val-switchVoltage[i]) < 10)  return i;
    }
    return 0;
  }
}

