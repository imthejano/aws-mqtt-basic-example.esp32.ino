
void ledHigh()
{
    digitalWrite(LED_BUILTIN, HIGH);
}

void ledLow()
{
    digitalWrite(LED_BUILTIN, LOW);
}

void blink(int cycleDuration)
{
    int duration = cycleDuration / 2;
    delay(duration);
    ledHigh();
    delay(duration);
    ledLow();
}

void blinkDelay(int times, int cycleDuration)
{
    for (int i = 0; i < times; i++)
    {
        blink(cycleDuration);
    }
}
