// Test protocol for controlling the train.

#include <CmdMessenger.h>
#include <Streaming.h>

// Mustnt conflict / collide with our message payload data. Fine if we use base64 library ^^ above
char field_separator = ',';
char command_separator = ';';

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);


// ------------------ S E R I A L  M O N I T O R -----------------------------
// 0;  communications error
// 1;  acknowledge cmd was received with data returned.
// 2;  ready command.  Send to get acknowledge
// 3;  cmd not recognized
// 4,nn change train speed by an amount ±nn
// 5,nn set speed to value nn (negative values reverse direction)
// 6;	emergency brake
// 7;   request status
// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Mustn't delete this line
};

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  changeTrainSpeed,     // 004
  setTrainSpeed,  	    // 005
  emergencyBrake,		// 006
  requestStatus,		// 007
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ------------------ C A L L B A C K  M E T H O D S -------------------------

void changeTrainSpeed()
{
  // Message data is any ASCII bytes (0-255 value). But can't contain the field
  // separator, command separator chars you decide (eg ',' and ';')
  cmdMessenger.sendCmd(kACK,"speed change received");
  int delSpeed = cmdMessenger.readInt();
	// issue command to change train speed
  Serial.println( delSpeed );
}

void setTrainSpeed()
{
  cmdMessenger.sendCmd(kACK,(char*)"speed set received");
  int setSpeed = cmdMessenger.readInt();
    // issue command to set the speed
  Serial.println( setSpeed, DEC );
}

void emergencyBrake()
{
  cmdMessenger.sendCmd(kACK,(char*)"All STOP!");
	// issue the ALLSTOP command

}

void requestStatus()
{
	// obtain the current train status
  char outBuffer[100];
  float val=12.23;
  int   ival=432;

  String amsg = "Spd:";
  String spdData = amsg + (long) 1230;

  String bmsg = "Pos:";
  String posData = bmsg + ival;
  
  String outString = spdData + "," + posData;
  
  outString.toCharArray(outBuffer,100);
  cmdMessenger.sendCmd(kACK,outBuffer);
}

// ------------------ D E F A U L T  C A L L B A C K S -----------------------
void trainReady()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Train ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------



// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup() 
{
  // Listen on serial connection for messages from the pc
  Serial.begin(57600);  // Arduino Duemilanove, FTDI Serial

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, trainReady);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);

  trainReady();

  // blink
  pinMode(13, OUTPUT);
}


// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 500; // 2 seconds
long previousMillis = 0;
int counter = 0;

void timeout()
{
  // blink
  if (counter % 2)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
  counter ++;
}  

void loop() 
{
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();

  // handle timeout function, if any
  if (  millis() - previousMillis > timeoutInterval )
  {
    timeout();
    previousMillis = millis();
  }

  // Loop.
}

