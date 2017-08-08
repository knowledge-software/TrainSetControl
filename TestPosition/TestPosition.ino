#include <TrainSpeed.h>
#include <TrainPosition.h>
#include <TrainSignals.h>

/*********************
 *Tools for testing the motor control library
 *
 **********************/

TrainPosition places;
TrainSpeed    gogo;
TrainSignals  lights;
int        dir = 1;

/** Initialize the values **/
void setup() {
  gogo.setAcceleration(1);
  gogo.setDesiredSpeed(0);
  Serial.begin(38400);

}

/** Run the Loop **/
void loop() {
  delay( 1 );  // a 1ms delay yo let things settle.
  gogo.updateSpeed();
  int trig = places.updatePosition();
  speedAction( trig );
  lights.updateSignal( places.trainLocaion() );
  //  speedAction( places.updatePosition(); );

  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.write( inByte );  
    switch( inByte ) {
    case 'U': 
      gogo.changeDesiredSpeed( 5 );
      Serial.println( gogo.getCurrentSpeed() );
      break;

    case 'D': 
      gogo.changeDesiredSpeed( -5 ); 
      Serial.println( gogo.getCurrentSpeed() );
      break;

    case 'B': 
      gogo.HALT(); 
      break;

    case 'A': 
      gogo.setDesiredSpeed( -50 );
      Serial.println( gogo.getCurrentSpeed() );
      break;    

    case 'S': 
      gogo.setDesiredSpeed( 50 );
      Serial.println( gogo.getCurrentSpeed() );
      break; 

    case 'T':
      lights.runTestSequence();
      break;  

    case 'R': 
      Serial.println( gogo.getCurrentSpeed() ); 
      Serial.println( "  *************************" );
      //Serial.println( gogo.isMotorConnected() );
      //Serial.println( gogo.isMotorShort() );
      Serial.print( "  * d: " );
      Serial.println( places.travelDirection() );
      Serial.print( "  * s: " );
      Serial.println( places.travelSpeed() ); 
      Serial.print( "  * l: " );
      Serial.println( places.trainLocaion() ); 
      Serial.println( "  *************************" );
      break;
    }

  }
}

/** Take action on the speed based on location of train **/
void speedAction( int location )  {

  // modify speed based on exact position feedback.
  switch( location ) {
  case PositionA:
    Serial.println( "--posA" );
    break;
  case PositionB:
    Serial.println( "--posB" );
    break;
  case PositionC:
    Serial.println( "--posC" );
    break;
  case Unknown:
    break;
  }

  if ( places.isProximateTo(50) )
    Serial.println("$$$ @50 $$$$");

}



