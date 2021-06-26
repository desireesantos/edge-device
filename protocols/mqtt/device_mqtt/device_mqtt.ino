#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#ifdef ESP8266
#endif
#include "CronAlarms.h"

CronId id;

#define TOPIC_SUBSCRIBE "MiddlewareFog/edge/#"
#define TOPIC_PUBLISH "MiddlewareFog/edge/device"
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

    // create timers, to trigger relative to when they're created
    Cron.create("*/1800 * * * * *", sendStatusOutputMQTT, false); // timer for every 30 minutes
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
    Serial.println("MQTT message cron was triggered");
    MQTT.publish(TOPIC_PUBLISH, "Edge - Message from MQTT DEVICE");
    Serial.println(TOPIC_PUBLISH);
    delay(2000);
}

void loop()
{
#ifdef __AVR__
    system_tick(); // must be implemented at 1Hz
#endif
    Cron.delay();
    checkConnectionsWifiMQTT();
    MQTT.loop();
}
