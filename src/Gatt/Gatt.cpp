#include "Config.h"
#include "Gatt.h"

/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "44325abd-809d-447e-8d80-bb57c47f81d9"
#define CHARACTERISTIC_UUID "8995fa13-f9b6-4501-8e54-6247b817a7ab"


Gatt::Gatt(){}

void Gatt::ini(){

	//esp_log_level_set("*", ESP_LOG_DEBUG);
	BLEDevice::init("VibHub");
	BLEServer *pServer = BLEDevice::createServer();
	BLEService *pService = pServer->createService(SERVICE_UUID);
	BLECharacteristic *pCharacteristic = pService->createCharacteristic(
											CHARACTERISTIC_UUID,
											BLECharacteristic::PROPERTY_READ |
											BLECharacteristic::PROPERTY_WRITE
										);

	pCharacteristic->setValue("Characteristic output");
	pService->start();
	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();

	//Serial.println("BLE Setup");

}
