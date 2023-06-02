#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "time.h"
#include "Config.h"

unsigned long getTime()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return (0);
    }
    time(&now);
    return now;
}
unsigned long getTimeForce()
{
    time_t now;
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo))
    {
        Serial.print(".");
    }
    time(&now);
    return now;
}

unsigned long epochTime;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

boolean connectWIFI()
{
    // connect to wifi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    return WiFi.status() == WL_CONNECTED;
}

boolean configureAWS(std::function<void(char *, uint8_t *, unsigned int)> callback)
{
    // get local time
    epochTime = getTimeForce();

    // Configure WiFiClientSecure to use the AWS IoT device credentials
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.setServer(AWS_IOT_ENDPOINT, 8883);

    // Create a message handler
    client.setCallback(callback);
    return true;
}

boolean connectAWS()
{
    // connect to AWS IOT service
    for (int iAttemps = 0; iAttemps < 100 && !client.connect(THINGNAME); iAttemps++)
    {
        delay(100);
    }
    if (!client.connected())
    {
        return false;
    }
    return true;
}
boolean subscribeToTopics(const char **subscribeTopics)
{
    for (int topicIndex = 0; topicIndex < sizeof(subscribeTopics); topicIndex++)
    {
        if (!client.subscribe(subscribeTopics[topicIndex]))
        {
            return false;
        }
    }
    return true;
}

boolean publishMessage(char *topic, StaticJsonDocument<DOCUMENT_SIZE> doc)
{
    char jsonBuffer[DOCUMENT_SIZE];
    serializeJson(doc, jsonBuffer);
    bool isSent = client.publish(topic, jsonBuffer);
    for (int iAttemps = 0; iAttemps < 20 && !isSent; iAttemps++)
    {
        client.connect(THINGNAME);
        isSent = client.publish(topic, jsonBuffer);
        delay(500);
    }
    if (isSent)
    {
        return true;
    }
    else
    {
        return false;
    }
}
