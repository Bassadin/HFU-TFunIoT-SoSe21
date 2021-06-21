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
    digitalWrite(START_LED_PIN, HIGH);
}

void changeEndLedState(bool newState)
{
    digitalWrite(END_LED_PIN, HIGH);
}