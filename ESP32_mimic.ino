#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs for the BLE service and characteristics
#define SERVICE_UUID "00000002-0000-0000-FDFD-FDFDFDFDFDFD"
#define CHARACTERISTIC_TEMPERATURE_UUID "00002A6E-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_HUMIDITY_UUID "00002A6F-0000-1000-8000-00805F9B34FB"

// Placeholder function for temperature and humidity sensor data
float getTemperature() {
    return 25.0; // Simulated temperature value
}

float getHumidity() {
    return 60.0; // Simulated humidity value
}

BLECharacteristic *pTemperatureCharacteristic;
BLECharacteristic *pHumidityCharacteristic;

void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("ESP32_BLE");
  BLEServer *pServer = BLEDevice::createServer();
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create Temperature Characteristic
  pTemperatureCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_TEMPERATURE_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  
  // Create Humidity Characteristic
  pHumidityCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_HUMIDITY_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE service is broadcasting");
}

void loop() {
  // Update sensor values
  float temperature = getTemperature();
  float humidity = getHumidity();
  
  // Set the new values
  pTemperatureCharacteristic->setValue(temperature);
  pHumidityCharacteristic->setValue(humidity);
  
  // Notify clients
  pTemperatureCharacteristic->notify();
  pHumidityCharacteristic->notify();
  
  delay(1000); // Update every second
}
