/* 
  This sketch is designed to manage operations of the Pololu M101B
  Motor conroller.  There are a number of pins that need to be controlled
  and monitored for proper feedback.  This has the description for the
  connections that are connected.
  
  Blk    +5
  Wht    Gnd
  Org    Current Sense  Pin 0   [IN]   VNH3SP30 does not support this.
  Grn    EA             Pin 12  [OUT/IN]
  Red    EB             Pin 08  [OUT/IN]
  Blu     A             Pin 13  [OUT]
  Brn     B             Pin 09  [OUT]
  Yel    PWM speed ctl  Pin 11  [OUT]
  Org    OpenCirc       Pin 10  [IN]
****/

#define  OpenCirc          10 // Org  
//#define  CurrSense       0 // Org
#define  EnA     12        // Grn
#define  EnB     8         // Red

#define  A       13        // Blu
#define  B       9         // Brn

#define  SpdCtl  11        // Yel - PWM

int incomingByte= 0;
int c = 0;

int motorDirec;
int BRAKE = -1;
int turnCCW = 1;
int turnCW = 0;

void setup()   {

  pinMode( OpenCirc, INPUT );
  //  pinMode( CurrSense, INPUT );
  pinMode( EnA, OUTPUT );
  pinMode( EnB, OUTPUT );
  
  pinMode( A, OUTPUT );
  pinMode( B, OUTPUT );
  
  pinMode( SpdCtl, OUTPUT );
  
  Serial.begin(57600);    

  motorDirec = 0;
  enableMotors( HIGH );
  
}

void loop()  {
  int spd;
  
//  if (Serial.available() > 0  && !isFault() ) {
  if ( Serial.available() > 0 ) {
    Serial.println( "Start" );  
    incomingByte = Serial.read();
      spd = incomingByte - '0';
      spd = spd*10;
     if ( !isMotorFault() ) {
          if ( isMotorConnected() ) {
           
              setMotorSpeed( spd );
              
              setMotorDirection (motorDirec);
              motorDirec++;
              
              delay( 2000 );
              
          //    c = readMotorCurrent();
          
    
          
              delay( 2000 );
              
              setMotorDirection( BRAKE );
          } else {
            Serial.println( "MOTOR NOT CONNECTED!" );
          }
    
     } else {
       Serial.println( "MOTOR FAULT!" );
     }  

    Serial.println( "Stop" );
    delay( 500 );
    
  }
    
}

// According to the documentation if I set OutA and OutB high a 0 on this will detect 
// an open cirucit even without applying voltage
boolean isMotorConnected() {
  
  digitalWrite( A, HIGH );
  digitalWrite( EnB, LOW );
  digitalWrite( EnA, HIGH );
  analogWrite( SpdCtl, 0 );
  
  int rc = digitalRead( OpenCirc );
  digitalWrite( A, LOW );
  digitalWrite( B, LOW );
  digitalWrite( EnB, HIGH );
  digitalWrite( EnA, HIGH );

  Serial.print( " openCircuit: " );
  Serial.println( rc, DEC);
  return rc==HIGH;
}

// Enable motion in a given direction or brake
void setMotorDirection( int dir ) {
  // Brake if dir < 0
  if ( dir < 0 ) {
     digitalWrite( B, LOW );
     digitalWrite( A, LOW );
     Serial.println( "...BRAKE" );
     }
   // if dir is odd go left
   else if (dir % 2) {
      digitalWrite( A, LOW );
      digitalWrite( B, HIGH );
     Serial.println( "...left" );
     }
   // go right 
   else {
      digitalWrite( A, HIGH );
      digitalWrite( B, LOW );  
      Serial.println( "...right" );
    }
}

// Read the current sense value.
//int readMotorCurrent()  {
//  int val = analogRead( CurrSense );
//  Serial.print( " Current " );
//  Serial.println(  val, DEC );
//  return val / 4;
//}


// Set the speed of the motor
void setMotorSpeed( int spd )  {
  // spd is between 0 and 100
  spd = (spd * 255)/100;
    Serial.print( " Speed " );
    Serial.println(  spd, DEC );
  analogWrite( SpdCtl, spd );
}

boolean isMotorFault( ) {
  char inA;
  char inB;
  inA = digitalRead( EnA );
  inB = digitalRead( EnB );
  Serial.print( " (d) inA: " ); Serial.print( inA, DEC );
  Serial.print( " (d) inB: " ); Serial.print( inB, DEC );
  Serial.println("");
  return (inA==LOW) || (inB==LOW);
}

void enableMotors( int OnOff ) {
    digitalWrite( EnA, OnOff );  // Enable outputs
    digitalWrite( EnB, OnOff );  // Enable Outputs
    if ( OnOff )
      Serial.println(  " Enabled " );
    else
      Serial.println(  " Disabled " );
    
}
