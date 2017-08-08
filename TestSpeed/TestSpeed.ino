#include <TrainSpeed.h>


/*********************
*Tools for testing the motor control library
*
**********************/


TrainSpeed    gogo;
int        dir = 1;

/** Initialize the values **/
void setup() {
  gogo.setAcceleration(1);
  gogo.setDesiredSpeed(0);
  Serial.begin(38400);

}

/** Run the Loop **/
void loop() {
  delay( 1 );
  gogo.updateSpeed();

  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.write( inByte );  
    switch( inByte ) {
    case 'U': gogo.changeDesiredSpeed( 5 );
              Serial.println( gogo.getCurrentSpeed() );
              break;

    case 'D': gogo.changeDesiredSpeed( -5 ); 
              Serial.println( gogo.getCurrentSpeed() );
              break;

    case 'B': gogo.HALT(); 
              break;
              
    case 'A': gogo.setDesiredSpeed( -50 );
              Serial.println( gogo.getCurrentSpeed() );
              break;    
    
    case 'S': gogo.setDesiredSpeed( 50 );
              Serial.println( gogo.getCurrentSpeed() );
              break;    

    case 'R': Serial.println( gogo.getCurrentSpeed() ); 
              Serial.println( gogo.isMotorConnected() );
              Serial.println( gogo.isMotorShort() );
              break;
    }

  }
}



