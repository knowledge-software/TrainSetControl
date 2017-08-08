/*
  SwitchMatrix.cpp - An alternate way of reading a matrix switch using resistors
  and a single analog input port.
  Copyright (c) 2011 Paul Beeken.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/******************************************************************************
 * Includes
 ******************************************************************************/

// Wiring Core Includes
#include <Arduino.h>

#include "SwitchMatrix.h"

/*
	I have a boat load of analog panel momentary contact switches.
	These buttons are organized as a matrix of connections between one
	line and another.  Reading the switches can be done a variety of ways. 
	The traditional way is to hook the leads to a set of digital ports and poll 
	the cross pins to determine which switch was hit.  This project was born from 
	a situation where I had populated all the digital ports for other purposes but
	had many unoccupied analog ports.  By bridging the pins with the right resistors
	we can read the state of the switch matrix with one analog port.  Simply supply
	the switch matrix with one Vcc (usually +5V) and sample the output voltage at
	at one pin after connecting the right resistors.  I have chosen 10k resistors
	(because I had a boatload of these as well as the 2x3 switches).  Pushing the switches
	connects the different resistors in combinations dividing the voltage to easily 
	distinguished levels.
	A matrix switch looks like the diagram below with the switch closing a contact between
	one row and a column.  By feeding row 1 with a +5V (via a 10k resistor) and pin 5 with another
	10k resistor to ground we can get various voltages depending on which button is pressed.  The
	other pins can must be connected with resistors to form a divider that allows us to 
	distinguish which button is pressed.  [!!! Include notes from circuit !!!]
	
	1-2  5k
	2-3 open
	3-4 10k
	4-5 5k
	1 o----A---B---C
	       |   |   |
	2 o----D---E---F
	       |   |   |
	       3   4   5
	tapping a button closes a connection between the horizontal and vertical 
	lines.  By encoding the lines with resistors (see notebook) you can read
	different voltages at pin 5.
*/
//	             "RPM",   "ADV",   "DW",    "^(up)", "TEST", "v(dn)"
enum { ZERO=1023, RPM=745, ADV=853, DW=681, UPA=475, TST=796, DNA=596 };

#define _analogPin	A0

SwitchMatrix::SwitchMatrix()
{
	_lastButtonHit = NoButton;
	pinMode(_analogPin, INPUT);
}

// Force a read of the switch. Need to poll often to catch fast fingered individuals.
//     There appears to be a problem with this scheme in the implementation.  The actual
//		values returned seem to fluctuate with time.  We may need to include a capaitor
//		and avearge values over a number of samples.  The switches themselves may be at
//		fault or there is a lot of noise in conjunction with other circuitry.
int SwitchMatrix::checkButtons(void)
{	// If you change the values of the resistors then these values have to change.
                        //  A,   B,  C,   D,   E,   F
    int switchVoltage[] = { RPM, DW, UPA, ADV, TST, DNA};
    
    // Average 6 values
    int value = averageSignal();

	// We build a little fuzz into the read to allow for noisy input.  This is more than enough.
	if ( value < 900 ) {
      if ( isSignalNear( value, RPM ) ) return ButtonA;
      if ( isSignalNear( value, ADV ) ) return ButtonD;
      if ( isSignalNear( value, DW ) )  return ButtonB;
      if ( isSignalNear( value, UPA ) ) return ButtonC;
      if ( isSignalNear( value, TST ) ) return ButtonE;
      if ( isSignalNear( value, DNA ) ) return ButtonF;
	}

    return NoButton;

}

int SwitchMatrix::averageSignal()
{
  int val = analogRead( _analogPin );
  for( int i=0; i<6; i++ ) {
    delay( 3 ); // 1 ms delay
    val += analogRead( _analogPin );
    }      
  return val/6;
}

bool SwitchMatrix::isSignalNear( int value, int limit )
{
  return abs(value-limit)<5;    
}


// Memory of last succesful button tap.
int SwitchMatrix::lastButtonHit(void)
{
	return _lastButtonHit;
}


// The first function simply polled if the button was hit.  This forces a wait until the 
// button is released.
int SwitchMatrix::checkButtonRelease(void)
{
	int theButton = checkButtons();
	if ( theButton==NoButton )
		return NoButton;
		
	while( checkButtons()==theButton );

	return lastButtonHit();
}