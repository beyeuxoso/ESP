#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ssid "IIoT_Room_2.4Ghz"
#define password "1234567890"
#define mqtt_server "broker.mqtt-dashboard.com"

const uint16_t mqtt_port = 1883; //Port của MQT

#define topic1 "OnOffLed"
WiFiClient espClient;
PubSubClient client(espClient);
void setup()
{
	Serial.begin(115200);
	setup_wifi();                             //thực hiện kết nối Wifi
	client.setServer(mqtt_server, mqtt_port); // cài đặt server và lắng nghe client ở port 1883
	client.setCallback(callback);             // gọi hàm callback để thực hiện các chức năng publish/subcribe
	reconnect();
    pinMode(D0,OUTPUT);

}

// Add the main program code into the continuous loop() function
void loop()
{
    client.loop();
    if (!client.connected())
    {
        reconnect();
        Serial.print("disconnected");
    }
    client.publish(topic1, "abc");
    delay(2000);

}

void setup_wifi()
{
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	// in ra thông báo đã kết nối và địa chỉ IP của ESP8266
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length)
{
    //-----------------------------------------------------------------
    //in ra tên của topic và nội dung nhận được
    Serial.print("Co tin nhan moi tu topic: ");
    Serial.println(topic);
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    if (String(topic) == "LedD0")
    {
        if (message == "on")
        {
            digitalWrite(D0, HIGH);
        }
        if (message == "off")
        {
            digitalWrite(D0, LOW);
        }
    }

    Serial.println(message);
    Serial.println();
    //-------------------------------------------------------------------------
}

// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect()
{
    while (!client.connected()) // Chờ tới khi kết nối
    {
        if (client.connect("clientId-CjEDtzG1bn")) //kết nối vào broker
        {
            Serial.println("da ket noi:");
            //đăng kí nhận dữ liệu từ topic
            client.subscribe("LedD0");
        }
        else
        {
            // in ra trạng thái của client khi không kết nối được với broker
            Serial.print("Loi:, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Đợi 5s
            delay(5000);
        }
    }
}