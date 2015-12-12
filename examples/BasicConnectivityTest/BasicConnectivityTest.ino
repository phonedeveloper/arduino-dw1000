/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file BasicConnectivityTest.ino
 * Use this to test connectivity with your DW1000 from Arduino.
 * It performs an arbitrary setup of the chip and prints some information.
 * 
 * Please change RESET_PIN and INTERRUPT_PIN constant values (below) to
 * reflect the pin number on your Arduino to which the DW1000/DWM1000's
 * RST and INT lines are connected, respectively.
 *
 * PLEASE NOTE: OUTPUT DOESN'T NECESSARILY MEAN IT IS WORKING
 * The sketch produces a lot of formatted output whether or not the
 * device is connected/working and doesn't generate an error if not
 * working.  If you are seeing all 'FF's for values, your module 
 * probably is not talking to your Arduino.
 *
 * You can change the DEVICE_ADDRESS, NETWORK_ID, and/or
 * EXTENDED_UNIQUE_IDENTIFIER constants (below) and reload the sketch 
 * to confirm if the device is able to be written to and read from.
 *
 * Other constants that appear above setup() can also be adjusted for
 * your needs.
 */

#include <SPI.h>
#include <DW1000.h>

// Set the Arduino pins that your DWM1000's RST and INT are connected to.
const static int RESET_PIN     = 9;
const static int INTERRUPT_PIN = 2;

// If you are unsure that you are talking to your module, change the 
// values below and verify that they change after you reload the sketch.
const static char* EXTENDED_UNIQUE_IDENTIFIER = "01.02.03.04.05.06.07.08";
const static int DEVICE_ADDRESS = 5;
const static int NETWORK_ID     = 10;

// The size of the buffer to hold a message returned by the library. If
// too large, it will consume all the memory available and produce 
// unpredictable results. If too small, the library will overwrite it and
// produce unpredictable results.
const static int MAX_MESSAGE_SIZE = 128;

// Determines how frequently the sketch will poll for status from the 
// device. This can be reduced to as little as one second (1000) or less
// if you are troubleshooting, say, serial communications.
const static int POLL_DELAY = 10000;    // in milliseconds
 
void setup() {
  // DEBUG monitoring
  Serial.begin(9600);
  
  // Give things time to settle so that the following serial statements
  // appear in an open Arduino IDE serial monitor window.
  delay(1000);
  
  // Remind user about RST and INT pin settings
  Serial.print(F("Sketch configured for DWM1000 RST on Arduino pin "));
  Serial.print(RESET_PIN);
  Serial.print(F(" and INT on pin "));
  Serial.print(INTERRUPT_PIN);
  Serial.println(F("."));
  
  // initialize the driver
  DW1000.begin(INTERRUPT_PIN, RESET_PIN);
  delay(1000);
  DW1000.select(SS);
  Serial.println(F("DW1000 initialized ..."));

  // set the extended unique identifier
  DW1000.setEUI((char*) EXTENDED_UNIQUE_IDENTIFIER);

  // general configuration
  DW1000.newConfiguration(); 
  DW1000.setDeviceAddress(DEVICE_ADDRESS);
  DW1000.setNetworkId(NETWORK_ID);
  DW1000.commitConfiguration();
  Serial.println(F("Committed configuration ..."));

  // wait a bit
  delay(1000);
}

void loop() {
  // DEBUG chip info and registers pretty printed
  char msg[MAX_MESSAGE_SIZE];

  Serial.println();

  // Ensures initial Serial.println() statements are displayed in
  // an open Arduino IDE serial monitor window when the sketch is loaded.
  delay(1000);
  
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print(F("Device ID: ")); Serial.println(msg);

  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print(F("Unique ID: ")); Serial.println(msg);

  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print(F("Network ID & Device Address: ")); Serial.println(msg);

  DW1000.getPrintableDeviceMode(msg); 
  Serial.print(F("Device mode: ")); Serial.println(msg);

  // wait a bit
  delay(POLL_DELAY);
}