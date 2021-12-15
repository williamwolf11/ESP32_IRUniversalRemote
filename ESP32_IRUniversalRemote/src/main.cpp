#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// the packet buffer
extern uint8_t packetbuffer[];

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
int mode = 0;
int buttonpressed = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define SEND_MODE 0
#define REMAP_MODE 1

// ==================== start of TUNEABLE PARAMETERS ====================

// The GPIO an IR detector/demodulator is connected to. Recommended: 14 (D5)
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 4;

// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
const uint16_t kIrLedPin = 14;

// The Serial connection baud rate.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/resender, let's use a larger
// than expected buffer so we can handle very large IR messages.
// i.e. Up to 512 bits.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
const uint8_t kTimeout = 50; // Milli-Seconds

// kFrequency is the modulation frequency all messages will be replayed at.
const uint16_t kFrequency = 38000; // in Hz. e.g. 38kHz.

// ==================== end of TUNEABLE PARAMETERS ====================

// ==================== beginning of WILLS VARIABLES ================== //

// Raw_array variables (one per button)
uint16_t *raw_array1;
uint16_t *raw_array2;
uint16_t *raw_array3;
uint16_t *raw_array4;
uint16_t *raw_array5;
uint16_t *raw_array6;
uint16_t *raw_array7;
uint16_t *raw_array8;
uint16_t *raw_array[8];
// Length variables (one per button)
uint16_t length1;
uint16_t length2;
uint16_t length3;
uint16_t length4;
uint16_t length5;
uint16_t length6;
uint16_t length7;
uint16_t length8;
uint16_t length_arr[8];

int remapNum = 0;
;

// ==================== end of WILLS VARIABLES ================== //

// The IR transmitter.
IRsend irsend(kIrLedPin);
// The IR receiver.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
// Somewhere to store the captured message.
decode_results results;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '1' && rxValue[3] == '1') {
      Serial.println("Yall pressed button 1");
      if (mode == SEND_MODE) {
        buttonpressed = 1;
        /*
          irsend.sendRaw(raw_array1, length1, kFrequency);
          irsend.sendRaw(raw_array1, length1, kFrequency);
          irsend.sendRaw(raw_array1, length1, kFrequency);
          */
      }
      else
      {
        remapNum = 1;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '2' && rxValue[3] == '1') {
      Serial.println("Yall pressed button 2");
      if (mode == SEND_MODE) {
        buttonpressed = 2;
      }
      else {
        remapNum = 2;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '3' && rxValue[3] == '1') {
      Serial.println("Yall pressed button 3");
      if (mode == SEND_MODE) {
        buttonpressed = 3;
      }
      else {
        remapNum = 3;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '4' && rxValue[3] == '1') {
      Serial.println("Yall pressed button 4");
      if (mode == SEND_MODE) {
        buttonpressed = 4;
      }
      else {
        remapNum = 4;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '5' && rxValue[3] == '1') {
      Serial.println("Yall pressed the up arrow");
      if (mode == SEND_MODE) {
        buttonpressed = 5;
      }
      else {
        remapNum = 5;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '6' && rxValue[3] == '1') {
      Serial.println("Yall pressed the down arrow");
      if (mode == SEND_MODE) {
        buttonpressed = 6;
      }
      else {
        remapNum = 6;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '7' && rxValue[3] == '1') {
      Serial.println("Yall pressed the left arrow");
      if (mode == SEND_MODE) {
        buttonpressed = 7;
      }
      else {
        remapNum = 7;
      }
    }
    if (rxValue[0] == '!' && rxValue[1] == 'B' && rxValue[2] == '8' && rxValue[3] == '1') {
      Serial.println("Yall pressed the right arrow");
      if (mode == SEND_MODE) {
        buttonpressed = 8;
      }
      else {
        remapNum = 8;
      }
    }
    if ((rxValue[0] == 'R' || rxValue[0] == 'r') && rxValue[1] == 'e' && rxValue[2] == 'm' && rxValue[3] == 'a' && rxValue[4] == 'p') {
      Serial.println("REMAP MODE ACTIVATED");
      mode = REMAP_MODE;
    }
    if ((rxValue[0] == 'C' || rxValue[0] == 'c') && rxValue[1] == 'a' && rxValue[2] == 'n' && rxValue[3] == 'c' && rxValue[4] == 'e' && rxValue[5] == 'l') {
      Serial.println("REMAP MODE DEACTIVATED");
      mode = SEND_MODE;
    }
  }
};

void setup()
{
  irrecv.enableIRIn(); // Start up the IR receiver.
  irsend.begin();      // Start up the IR sender.

  //Serial.begin(kBaudRate, SERIAL_8N1);
  Serial.begin(9600);
  while (!Serial) // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();

  // Create the BLE Device
  BLEDevice::init("ESP32 Universal IR Remote");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop()
{

  // check if a button was pressed through UART --
  // UART flags the button press and the IR is handled
  // in the main loop since otherwise the connection breaks
  if (buttonpressed != 0) {
    uint16_t sendlength = length_arr[buttonpressed-1];
    /*for (int i = 0; i < 3; i++) {
      irsend.sendRaw(raw_array[buttonpressed-1], length_arr[buttonpressed-1], kFrequency);
    }*/
    for (int i = 0; i < 3; i++) {
      if (buttonpressed == 1) {
        Serial.println("button1");
        irsend.sendRaw(raw_array1, sendlength, kFrequency);
      } else if (buttonpressed == 2) {
        Serial.println("button2");
        irsend.sendRaw(raw_array2, sendlength, kFrequency);
      } else if (buttonpressed == 3) {
        irsend.sendRaw(raw_array3, sendlength, kFrequency);
      } else if (buttonpressed == 4) {
        irsend.sendRaw(raw_array4, sendlength, kFrequency);
      } else if (buttonpressed == 5) {
        irsend.sendRaw(raw_array5, sendlength, kFrequency);
      } else if (buttonpressed == 6) {
        irsend.sendRaw(raw_array6, sendlength, kFrequency);
      } else if (buttonpressed == 7) {
        irsend.sendRaw(raw_array7, sendlength, kFrequency);
      } else if (buttonpressed == 8) {
        irsend.sendRaw(raw_array8, sendlength, kFrequency);
      }
    }
    buttonpressed = 0;
  }

  if (deviceConnected) {
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    txValue++;
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
  // Check if an IR message has been received.
  if (irrecv.decode(&results) && mode == REMAP_MODE) {
    // We have captured something.
    // Convert the results into an array suitable for sendRaw().
    // resultToRawArray() allocates the memory we need for the array.
    uint16_t *raw_array = resultToRawArray(&results);
    // Find out how many elements are in the array.
    uint16_t length = getCorrectedRawLength(&results);

    //Assign the received IR signal to the correct Bluetooth button
    //raw_array[remapNum-1] = raw_array;
    if (remapNum == 1) {
      raw_array1 = raw_array;
      //length1 = length;
      length_arr[0] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 1!");
    }
    else if (remapNum == 2) {
      raw_array2 = raw_array;
      //length2 = length;
      length_arr[1] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 2!");
    }
    else if (remapNum == 3)
    {
      raw_array3 = raw_array;
      //length3 = length;
      length_arr[2] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 3!");
    }
    else if (remapNum == 4)
    {
      raw_array4 = raw_array;
      //length4 = length;
      length_arr[3] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 4!");
    }
    else if (remapNum == 5)
    {
      raw_array5 = raw_array;
      //length5 = length;
      length_arr[4] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 5!");
    }
    else if (remapNum == 6)
    {
      raw_array6 = raw_array;
      //length6 = length;
      length_arr[5] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 6!");
    }
    else if (remapNum == 7)
    {
      raw_array7 = raw_array;
      //length7 = length;
      length_arr[6] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 7!");
    }
    else if (remapNum == 8)
    {
      raw_array8 = raw_array;
      //length8 = length;
      length_arr[7] = length;
      remapNum = 0;
      //mode = SEND_MODE;
      Serial.println("Successfully remapped button 8!");
    }

    // Resume capturing IR messages. It was not restarted until after we sent
    // the message so we didn't capture our own message.
    irrecv.resume();
    // Deallocate the memory allocated by resultToRawArray().
    //delete [] raw_array;
    yield(); // Or delay(milliseconds); This ensures the ESP doesn't WDT reset.
  }
}
