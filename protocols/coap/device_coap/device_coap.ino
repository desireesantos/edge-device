#include <ESP8266WiFi.h>
#include "coap_client.h"

coapClient coap;
const char *ssid = "SSID";
const char *password = "PASSWORD";
char *resoureURI = "middleware";
char *coapMessage = "Edge - Message from COAP DEVICE";

IPAddress ip("XXX", "XXX", "XXX", "XXX");
int port = 5683;

// coap client response callback
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
}

void loop()
{
  int msgid = coap.put(ip, port, resoureURI, coapMessage, strlen(coapMessage));
  Serial.println(msgid);
  delay(1000);
}
