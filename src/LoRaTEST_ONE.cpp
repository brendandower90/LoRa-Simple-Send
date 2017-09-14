//LoRaWAN simple send example
#include <Sodaq_RN2483.h>
#include "Sodaq_RN2483_internal.h"
#include <Sodaq_wdt.h>
#include <Arduino.h>

#define debugSerial SerialUSB
#define loraSerial Serial1
#define endl SerialUSB.println();


// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
   0x0C, 0x50, 0x01, 0x1A
};

// Application Server Key
const uint8_t appSKey[16] =
{
   0x77, 0x2E, 0x22, 0xE7, 0x0C, 0x2D, 0x49, 0x5D,
   0xCF, 0xEC, 0x39, 0x87, 0x7D, 0xE8, 0xCB, 0x16
};

// Network Server Key
const uint8_t nwkSKey[16] =
{
   0x72, 0xCC, 0x77, 0x83, 0x0C, 0xC4, 0xBD, 0xB1,
   0x77, 0xF6, 0x8C, 0x55, 0xD1, 0xEE, 0x7C, 0x33
};

// Some complete random hex
uint8_t testPayload[] =
{
  0x53, 0x4F, 0x44, 0x41, 0x51
};

void setup()
{
  while ((!debugSerial) && (millis() < 10000));

  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());

  LoRaBee.setDiag(debugSerial); // optional
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    debugSerial.println("Connection to the network was successful.");
  }
  else
  {
    debugSerial.println("Connection to the network failed!");
  }

  debugSerial.println("Sleeping for 5 seconds before starting sending out test packets.");
  for (uint8_t i = 3; i > 0; i--)
  {
    debugSerial.print(".");
    delay(1000);
  }

}

void loop()
{

  // send 10 packets, with at least a 5 seconds delay after each transmission (more seconds if the device is busy)
  uint8_t i = 10;
  while (i > 0)
 // testPayload[0] = i;

   //LoRaBee.setMacParam(STR_PWR_IDX, 10%i);
  {
    switch (LoRaBee.sendReqAck(1, testPayload, 5,1))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      i--;
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe program will now halt.");
      while (1) {};
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
    delay(30000);
  }
}
