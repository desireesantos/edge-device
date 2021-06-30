#include <ESP8266WiFi.h>
#include <time.h>
#ifdef ESP8266
#include <sys/time.h>
#endif
#include "coap_client.h"
#include "CronAlarms.h"

coapClient coap;
const char *ssid = "ssid";
const char *password = "password";
char *resoureURI = "middleware";
char *coapMessage = "Edge - Message from COAP DEVICE";

IPAddress ip(192, 168, 43, 73);
int port = 5683;
CronId id;

void callback_response(coapPacket &packet, IPAddress ip, int port)
{
  char packetPayload[packet.payloadlen + 1];
  memcpy(packetPayload, packet.payload, packet.payloadlen);
  packetPayload[packet.payloadlen] = NULL;

  //response from coap server
  if (packet.type == 3 && packet.code == 0)
  {
    Serial.println("ping ok");
  }

  Serial.println(packetPayload);
}

void setup()
{

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println(" ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    yield();
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  coap.response(callback_response);
  coap.start();
  Cron.create("*/15 * * * * *", publishMessage, false); // timer for every 15 seconds
}

void publishMessage()
{
  coap.put(ip, port, resoureURI, coapMessage, strlen(coapMessage));
}

void loop()
{
<<<<<<< HEAD
  int msgid = coap.put(ip, port, resoureURI, coapMessage, strlen(coapMessage));
  Serial.println(msgid);
  delay(1000);
}
=======
  Cron.delay();
  coap.loop();
}
>>>>>>> Fix COAP connection issue
