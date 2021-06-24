std::deque<int> measurementDeque;
const int maxDequeSize = 15;

int handleMeasurementQueueForAverageValue(int newMeasurementValue)
{
    int millivoltsDynamoMeasurement = analogReadMilliVolts(DYNAMO_MEASUREMENT_PIN);
    measurementDeque.push_front(millivoltsDynamoMeasurement);

    if (measurementDeque.size() < 5)
    {
        return newMeasurementValue;
    }

    if (measurementDeque.size() > maxDequeSize)
    {
        measurementDeque.pop_back();
    }

    int dequeSum = 0;
    for (int i : measurementDeque)
    {
        dequeSum += i;
    }

    return dequeSum / measurementDeque.size();
}

void changeStartLedState(bool newState)
{
    if (newState)
    {
        startJled.Breathe(1000).Forever();
    }
    else
    {
        startJled.FadeOff(1000).Repeat(1);
    }
}

void changeEndLedState(bool newState)
{
    if (newState)
    {
        endJled.Breathe(1000).Forever();
    }
    else
    {
        endJled.FadeOff(1000).Repeat(1);
    }
}

void goToDeepSleep()
{
    Serial.println("Going to sleep now.");
    esp_deep_sleep_start();
}