#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// the packet buffer
extern uint8_t packetbuffer[];


BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      // if (rxValue.length() > 0) {
      //   Serial.println("*********");
      //   Serial.print("Received Value: ");
      //   for (int i = 0; i < rxValue.length(); i++)
      //     Serial.print(rxValue[i]);
      // }
      // Serial.println();
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '1' && rxValue[3] == '1'){
        Serial.println("Yall pressed button 1");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '2' && rxValue[3] == '1'){
        Serial.println("Yall pressed button 2");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '3' && rxValue[3] == '1'){
        Serial.println("Yall pressed button 3");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '4' && rxValue[3] == '1'){
        Serial.println("Yall pressed button 4");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '5' && rxValue[3] == '1'){
        Serial.println("Yall pressed the up arrow");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '6' && rxValue[3] == '1'){
        Serial.println("Yall pressed the down arrow");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '7' && rxValue[3] == '1'){
        Serial.println("Yall pressed the left arrow");
      }
      if(rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '8' && rxValue[3] == '1'){
        Serial.println("Yall pressed the right arrow");
      }
      if((rxValue[0] == 'R' || rxValue[0] == 'r') && rxValue[1] == 'e' && rxValue[2] == 'm' && rxValue[3] == 'a' && rxValue[4] == 'p'){
        Serial.println("REMAP MODE ACTIVATED");
      }
      if((rxValue[0] == 'C' || rxValue[0] == 'c')&& rxValue[1] == 'a' && rxValue[2] == 'n' && rxValue[3] == 'c' && rxValue[4] == 'e' && rxValue[5] == 'l'){
        Serial.println("REMAP MODE DEACTIVATED");
      }
      // Serial.println("*********");
    }
};


void setup() {
  Serial.begin(9600);

  // Create the BLE Device
  BLEDevice::init("Logitech Sux");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

    if (deviceConnected) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }


}


// #include <Arduino.h>
// #include <IRsend.h>
// #include <IRrecv.h>
// #include <IRremoteESP8266.h>
// #include <IRutils.h>

// // ==================== start of TUNEABLE PARAMETERS ====================

// // The GPIO an IR detector/demodulator is connected to. Recommended: 14 (D5)
// // Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
// const uint16_t kRecvPin = 14;

// // GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
// const uint16_t kIrLedPin = 4;

// // The Serial connection baud rate.
// // NOTE: Make sure you set your Serial Monitor to the same speed.
// const uint32_t kBaudRate = 115200;

// // As this program is a special purpose capture/resender, let's use a larger
// // than expected buffer so we can handle very large IR messages.
// // i.e. Up to 512 bits.
// const uint16_t kCaptureBufferSize = 1024;

// // kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// // message ended.
// const uint8_t kTimeout = 50;  // Milli-Seconds

// // kFrequency is the modulation frequency all messages will be replayed at.
// const uint16_t kFrequency = 38000;  // in Hz. e.g. 38kHz.

// // ==================== end of TUNEABLE PARAMETERS ====================

// // The IR transmitter.
// IRsend irsend(kIrLedPin);
// // The IR receiver.
// IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
// // Somewhere to store the captured message.
// decode_results results;

// // This section of code runs only once at start-up.
// void setup() {
//   irrecv.enableIRIn();  // Start up the IR receiver.
//   irsend.begin();       // Start up the IR sender.

//   Serial.begin(kBaudRate, SERIAL_8N1);
//   while (!Serial)  // Wait for the serial connection to be establised.
//     delay(50);
//   Serial.println();

//   Serial.print("DumbIRRepeater is now running and waiting for IR input "
//                "on Pin ");
//   Serial.println(kRecvPin);
//   Serial.print("and will retransmit it on Pin ");
//   Serial.println(kIrLedPin);
// }

// // The repeating section of the code
// void loop() {
//   // Check if an IR message has been received.
//   if (irrecv.decode(&results)) {  // We have captured something.
//     // The capture has stopped at this point.

//     // Convert the results into an array suitable for sendRaw().
//     // resultToRawArray() allocates the memory we need for the array.
//     uint16_t *raw_array = resultToRawArray(&results);
//     // Find out how many elements are in the array.
//     uint16_t length = getCorrectedRawLength(&results);
//     // Send it out via the IR LED circuit.
//     irsend.sendRaw(raw_array, length, kFrequency);
//     irsend.sendRaw(raw_array, length, kFrequency);
//     irsend.sendRaw(raw_array, length, kFrequency);
//     // Resume capturing IR messages. It was not restarted until after we sent
//     // the message so we didn't capture our own message.
//     irrecv.resume();
//     // Deallocate the memory allocated by resultToRawArray().
//     delete [] raw_array;

//     // Display a crude timestamp & notification.
//     uint32_t now = millis();
//     Serial.printf(
//         "%06u.%03u: A message that was %d entries long was retransmitted.\n",
//         now / 1000, now % 1000, length);
//   }
//   yield();  // Or delay(milliseconds); This ensures the ESP doesn't WDT reset.
// }

// #include <Arduino.h>
// #include <IRrecv.h>
// #include <IRsend.h>
// #include <IRremoteESP8266.h>



// uint16_t RECV_PIN = 4; // for ESP32 micrcontroller
// const uint16_t kIrLed = 14;// ESP8266 GPIO pin to use. Recommended: 4 (D2).

// IRsend irsend(kIrLed);// Set the GPIO to be used to sending the message.

// IRrecv irrecv(RECV_PIN);
// decode_results results;

// void setup() {
//     irsend.begin();
//     Serial.begin(9600);
//     irrecv.enableIRIn();
// }

// void loop() {
//     if (irrecv.decode(&results)) {
//         if (results.value >> 32) //print() & println() can't handle printing long longs. (uint64_t)
//         Serial.print((uint32_t) (results.value >> 32), HEX); //print the first part of the message
//         Serial.println((uint32_t) (results.value & 0xFFFFFFFF), HEX); //print the second part of the message
//         irrecv.resume(); //Receive the next value
//     }
//     Serial.println("Sony");
//     irsend.sendSony(0xa90, 12); // Sony TV power code
//     delay(2000);
//     irsend.sendSony(0xa90, 12); // Sony TV power code
//     delay(2000);
// }