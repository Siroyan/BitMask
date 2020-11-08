#include <WiFi.h>
#include <HTTPClient.h>
#include "Credentials.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;
const char* serverName = "http://3.114.89.143/api/device-datas";

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;

void setup() {
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.println("Connecting");
	while(WiFi.status() != WL_CONNECTED) {
	delay(500);
	Serial.print(".");
	}
	Serial.println("The wifi connection was successful.");
}

void loop() {
	// サーバーにデータを送信
	if ((millis() - lastTime) > timerDelay) {
		if(WiFi.status()== WL_CONNECTED){
			HTTPClient http;

			http.begin(serverName);

			http.addHeader("Content-Type", "application/json");
			int httpResponseCode = http.POST("{\"deviceName\":\"device-A\",\"temperature\":36.0,\"humidity\":50.2,\"pressure\":1020,\"gas\":12.3}");
			
			Serial.print("HTTP Response code: ");
			Serial.println(httpResponseCode);
			
			// Free resources
			http.end();
		} else {
			Serial.println("WiFi Disconnected");
		}
		lastTime = millis();
	}
}