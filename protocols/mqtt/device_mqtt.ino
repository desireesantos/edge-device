#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TOPIC_SUBSCRIBE "MiddleFog/#"
#define TOPIC_PUBLISH "MiddleFog"
#define ID_MQTT "MiddleFog"

const char *SSID = "SSID";
const char *PASSWORD = "PASSWORD";
const char *BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;

WiFiClient espClient;
PubSubClient MQTT(espClient);

void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);

void setup()
{
    initSerial();
    initWiFi();
    initMQTT();
}

void initSerial()
{
    Serial.begin(115200);
}

void initWiFi()
{
    delay(10);
    Serial.println("------WI-FI Connection------");
    Serial.print("Connecting to the network: ");
    Serial.println(SSID);
    Serial.println("Wait");

    reconectWiFi();
}

void initMQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    String msg;

    for (int i = 0; i < length; i++)
    {
        char message = (char)payload[i];
        msg += message;
    }
}

void reconnectMQTT()
{
    while (!MQTT.connected())
    {
        Serial.print("* Trying to connect to MQTT broker: ");
        Serial.println(BROKER_MQTT);

        if (MQTT.connect(ID_MQTT))
        {
            Serial.println("Conected with broker MQTT!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        }
        else
        {
            Serial.println("Failure");
            Serial.println("Try againg after 2s");
            delay(2000);
        }
    }
}

void reconectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("Connecting to the network");
    Serial.print(SSID);
    Serial.println("IP obtained: ");
    Serial.println(WiFi.localIP());
}

void checkConnectionsWifiMQTT(void)
{
    if (!MQTT.connected())
        reconnectMQTT();

    reconectWiFi();
}

void sendStatusOutputMQTT(void)
{
    MQTT.publish(TOPIC_PUBLISH, "FIX DATA");
    Serial.println(TOPIC_PUBLISH);
    delay(2000);
}

void loop()
{
    checkConnectionsWifiMQTT();
    sendStatusOutputMQTT();
    MQTT.loop();
}
