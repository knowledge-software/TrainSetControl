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


#ifndef SwitchMatrix_h
#define SwitchMatrix_h

enum {
	NoButton=0,
	ButtonA=1,	ButtonB,	ButtonC,	ButtonD,	ButtonE,	ButtonF
	};

class SwitchMatrix
{
  private:
    int 	_lastButtonHit;
    int		averageSignal();
    bool	isSignalNear( int value, int limit );

  public:
    SwitchMatrix();
    int checkButtons(void);
    int checkButtonRelease(void);
    int lastButtonHit(void);
};

#endif

