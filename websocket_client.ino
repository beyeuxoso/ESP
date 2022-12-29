#include <Arduino.h>
#include<ESP8266WiFi.h>
#include <WebSocketsClient.h>
WebSocketsClient webSocket;
const char* ssid = "abc"; //Đổi thành wifi của bạn
const char* password = "12345678"; //Đổi pass luôn
const char* ip_host = "192.168.4.1"; //Đổi luôn IP host của PC nha
const uint16_t port = 81; //Port thích đổi thì phải đổi ở server nữa
void setup()
{
    pinMode(D4, OUTPUT);
    pinMode(D3, INPUT);
    Serial.begin(9600);
    Serial.println("ESP8266 Websocket Client");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    webSocket.begin(ip_host, port);
    webSocket.onEvent(webSocketEvent);

}

// Add the main program code into the continuous loop() function
void loop()
{
    webSocket.loop();
    if (digitalRead(D3) == 0) {
        webSocket.sendTXT("onD4");
        //digitalWrite(D4, LOW);
    }
    else {
        webSocket.sendTXT("offD4");
    }

}
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    String payloadString = (const char*)payload;
    if (payloadString == "onD4") {
        digitalWrite(D4, LOW);
    }
    if (payloadString == "offD4") {
        digitalWrite(D4, HIGH);
    }
}
