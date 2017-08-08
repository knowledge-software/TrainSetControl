/*
Magnetic proximity sensor.  This is a practice using a hall sensor as a proximity
sensor to locate a part

The device in this test is 451A 008  There is a curved face (where the part# is
printed) and the flat face
          |----  +vcc         |Rnd Face|
Flat Face |----  gnd          ----------
          |----  sig           |   |   |
                              vc  gnd  sig
          It works best to tie the sig to +vcc with a 10k resistor.
          Magnetic Field is present when signal goes down.
*/

int mhfPin   =  7;    // MagHallEffect connected to digital pin 13
unsigned long  milliTime =  0;
boolean  magState;

void setup()  {
  Serial.begin(57600);    
  // initialize the digital pin as an output:
  pinMode(mhfPin, INPUT);
  
  milliTime = millis();
  magState = false;
}



void loop()  {

  if ( millis()>milliTime+100 ) {
    Serial.print( "Sensor State: " );
    Serial.println( magState, DEC ); 
    milliTime = millis();
    magState = false;
  }
  
  if ( !digitalRead( mhfPin ) ) {
    // MagField is present
    delay( 5 );
    if ( !digitalRead( mhfPin ) )      // Mag field is still present
      magState = true;
//    else    // Mag field not present
//      magState = false;
        
  }// else
//      magState = false;
  
}
