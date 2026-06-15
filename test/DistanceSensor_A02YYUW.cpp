#include "DistanceSensor_A02YYUW.h"

// Header byte and timeout.
static const uint8_t HEADER_BYTE = 0xFF;
static const unsigned long READ_TIMEOUT_MS = 200;

DistanceSensor_A02YYUW::DistanceSensor_A02YYUW(uint16_t targetDistance, Stream *serial, size_t bufferSize)
    : _serial(serial), _target(targetDistance), _bufferSize(bufferSize), _index(0), _count(0)
{
    if (_bufferSize == 0)
        _bufferSize = 1;
    _buffer = (int *)malloc(sizeof(int) * _bufferSize);
    for (size_t i = 0; i < _bufferSize; ++i)
        _buffer[i] = ERROR_DISTANCE;
}

void DistanceSensor_A02YYUW::begin(unsigned long baud)
{
    // initialize the default hardware serial if it's the chosen serial
    if (_serial == &Serial1)
    {
        Serial1.begin(baud);
    }
}

uint8_t DistanceSensor_A02YYUW::readN(uint8_t *buf, size_t len, unsigned long timeoutMs)
{
    size_t offset = 0;
    unsigned long start = millis();
    while (offset < len)
    {
        if (_serial->available())
        {
            int v = _serial->read();
            if (v >= 0)
            {
                buf[offset++] = (uint8_t)v;
            }
        }
        if (millis() - start > timeoutMs)
            break;
    }
    return offset;
}

int DistanceSensor_A02YYUW::readRawDistance()
{
    uint8_t data[4] = {0};
    uint8_t receivedByte = 0;
    unsigned long startTime = millis();

    while (millis() - startTime < READ_TIMEOUT_MS)
    {
        if (readN(&receivedByte, 1, READ_TIMEOUT_MS) == 1 && receivedByte == HEADER_BYTE)
        {
            data[0] = receivedByte;
            if (readN(&data[1], 3, READ_TIMEOUT_MS) == 3)
            {
                uint8_t checksum = data[0] + data[1] + data[2];
                if (checksum == data[3])
                {
                    uint16_t distance = ((uint16_t)data[1] << 8) | data[2];
                    return (int)distance;
                }
            }
        }
    }
    return ERROR_DISTANCE;
}

int DistanceSensor_A02YYUW::getDistance()
{
    int reading = readRawDistance();
    if (reading == ERROR_DISTANCE)
    {
        // if no new reading, but buffer has values, return average of existing; otherwise propagate error
        if (_count == 0)
            return ERROR_DISTANCE;
    }
    else
    {
        // push into circular buffer
        _buffer[_index] = reading;
        _index = (_index + 1) % _bufferSize;
        if (_count < _bufferSize)
            _count++;
    }

    if (_count == 0)
        return ERROR_DISTANCE;
    long sum = 0;
    for (size_t i = 0; i < _count; ++i)
        sum += _buffer[i];
    return (int)(sum / (long)_count);
}

int DistanceSensor_A02YYUW::getDistanceToTarget()
{
    int avg = getDistance();
    if (avg == ERROR_DISTANCE)
        return ERROR_DISTANCE;
    return avg - (int)_target;
}

void DistanceSensor_A02YYUW::setTarget(uint16_t targetDistance)
{
    _target = targetDistance;
}
