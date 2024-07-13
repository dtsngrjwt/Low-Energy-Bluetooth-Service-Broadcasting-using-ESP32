#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;

#define SERVICE_UUID "00000002-0000-0000-FDFD-FDFDFDFDFDFD"
#define CHARACTERISTIC_UUID_TX "00000002-0000-0000-FDFD-FDFDFDFDFDFD"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(9600);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY);

  //BLE2902 needed to notify

  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising

  pServer->getAdvertising()->start();

  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  if (deviceConnected) {
    txValue = random(10, 50);

    // Conversion of txValue
    char txString[8];
    dtostrf(txValue, 1, 2, txString);

    //Setting the value to the characteristic
    pCharacteristic->setValue(txString);

    //Notifying the connected client
    pCharacteristic->notify();
    Serial.println("Sent value: " + String(txString));
    delay(500);
  }
}

