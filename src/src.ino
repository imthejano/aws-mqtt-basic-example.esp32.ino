#include "headers/MQTT.h"
#include "headers/LedBlink.h"

#define MESSAGE_LATENCY_IN_SECONDS 60
#define AWS_IOT_PUBLISH_TOPIC "aws/mqtt/test/thing-will-post-here"
const char *TOPICS_TO_SUBSCRIBE[] = {"aws/mqtt/test/aws-will-post-here", "aws/mqtt/test/aws-will-post-here-too"};

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    configTime(0, 0, ntpServer);
    randomSeed(analogRead(0));

    Serial.print("Connecting to wifi");
    if (connectWIFI())
        Serial.println(" -> success");
    else
        Serial.println(" -> failed");

    Serial.print("configuring aws");
    if (configureAWS(receiveMessageCallback))
        Serial.println(" -> success");
    else
        Serial.println(" -> failed");

    Serial.print("connecting to aws");
    if (connectAWS())
        Serial.println(" -> success");
    else
        Serial.println(" -> failed");

    Serial.print("subscribing to topics");
    if (subscribeToTopics(TOPICS_TO_SUBSCRIBE))
        Serial.println(" -> success");
    else
        Serial.println(" -> failed");
}

void loop()
{
    if (client.connected())
    {
        epochTime = getTime();
        ledLow();
        if ((epochTime + 5) % MESSAGE_LATENCY_IN_SECONDS == 0)
        {
            blinkDelay(10, 200);
            publish();
        }
        client.loop();
    }
    else
    {
        Serial.print("connecting to aws");
        if (connectAWS())
            Serial.println(" -> success");
        else
            Serial.println(" -> failed");
        Serial.print("subscribing to topics");
        if (subscribeToTopics(TOPICS_TO_SUBSCRIBE))
            Serial.println(" -> success");
        else
            Serial.println(" -> failed");
    }
}

void publish()
{
    StaticJsonDocument<DOCUMENT_SIZE> doc;
    doc["title"] = "Test";
    doc["message"] = "Hello, I am a thing using the internet";
    doc["thing_name"] = THINGNAME;
    doc["epoch_time"] = epochTime;

    Serial.println();
    Serial.print("Sending message");
    if (publishMessage(AWS_IOT_PUBLISH_TOPIC, doc))
        Serial.println(" -> success");
    else
        Serial.println(" -> failed");
    blinkDelay(10, 50);
}

void receiveMessageCallback(char *topic, byte *payload, unsigned int length)
{
    blinkDelay(10, 50);
    Serial.println();
    Serial.print("incoming message: ");
    Serial.print(" -> ");
    Serial.println(topic);
    StaticJsonDocument<DOCUMENT_SIZE> doc;
    deserializeJson(doc, payload);
    const char *message = doc["message"];
    Serial.println(message);
}
