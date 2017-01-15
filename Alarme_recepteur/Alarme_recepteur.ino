// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>


int VW_MESSAGE_LEN = 9;

void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MESSAGE_LEN];
    uint8_t buflen = VW_MESSAGE_LEN;
    digitalWrite(13,false);

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if (buf[0] == 'I') {
	        int i;

        digitalWrite(13, true); // Flash a light to show received good message
	      // Message with a good checksum received, dump it.
	      Serial.print("Got: ");
	      Serial.println((char *) buf);
        digitalWrite(13, HIGH);
        delay(10000);

        
      }
    }
}
