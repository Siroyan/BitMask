#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include "Credentials.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;
const char* serverName = "http://3.114.89.143/api/device-datas";

unsigned long lastTimeOfSend = 0;
unsigned long timerDelayOfSend = 10000;

Adafruit_BME680 bme;
String t, p, h, g;

unsigned long lastTimeOfSampling = 0;
unsigned long timerDelayOfSampling = 1000;

void setup() {
	// シリアル通信のセットアップ
	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial OK");

	// WiFiのセットアップ
	WiFi.begin(ssid, password);
	while(WiFi.status() != WL_CONNECTED);
	Serial.println("WiFi OK");

	// BME680のセットアップ
	if (!bme.begin()) {
		Serial.println("Could not find a valid BME680 sensor");
		while (1);
	}
	bme.setTemperatureOversampling(BME680_OS_8X);
	bme.setHumidityOversampling(BME680_OS_2X);
	bme.setPressureOversampling(BME680_OS_4X);
	bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
	bme.setGasHeater(320, 150);
}

void loop() {
	// サーバーにデータを送信
	if ((millis() - lastTimeOfSend) > timerDelayOfSend) {
		lastTimeOfSend = millis();
		Serial.println("Send data to server");
		if(WiFi.status()== WL_CONNECTED){
			HTTPClient http;
			http.begin(serverName);
			http.addHeader("Content-Type", "application/json");
			String postBody = "{\"deviceName\":\"device-A\",\"temperature\":" + t + ",\"humidity\":" + h + ",\"pressure\":" + p + ",\"gas\":" + g + "}";
			Serial.print("Post Body: ");Serial.println(postBody);
			int httpResponseCode = http.POST(postBody);
			Serial.print("HTTP Response code: ");
			Serial.println(httpResponseCode);
			http.end();
		} else {
			Serial.println("WiFi Disconnected");
		}
	}
	// センサーからデータを取得
	if ((millis() - lastTimeOfSampling) > timerDelayOfSampling) {
		lastTimeOfSampling = millis();
		if (!bme.performReading()) {
			Serial.println("Failure");
			return;
		}
		Serial.println("Get sensor data");
		t = String(bme.temperature);
		p = String(bme.pressure / 100.0);
		h = String(bme.humidity);
		g = String(bme.gas_resistance / 1000.0);
	}
}