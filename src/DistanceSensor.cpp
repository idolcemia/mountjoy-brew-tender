#include "DistanceSensor.h"

DistanceSensor::DistanceSensor()
    : _bufferSize(0), _buffer(nullptr), _index(0), _count(0)

{
}

void DistanceSensor::begin(unsigned long baud)
{

    Serial1.begin(baud);
}

uint8_t DistanceSensor::readN(uint8_t *buf, size_t len, unsigned long timeoutMs)
{
    size_t offset = 0;
    unsigned long start = millis();
    while (offset < len)
    {
        if (Serial1.available())
        {
            int v = Serial1.read();
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

int DistanceSensor::readRawDistance()
{
    uint8_t data[4] = {0};
    uint8_t receivedByte = 0;
    unsigned long startTime = millis();

    while (millis() - startTime < READ_TIMEOUT_MS)
    { // Check if timeout
        if (readN(&receivedByte, 1, READ_TIMEOUT_MS) == 1 && receivedByte == HEADER_BYTE)
        { // Find the header byte
            data[0] = receivedByte;
            if (readN(&data[1], 3, READ_TIMEOUT_MS) == 3)
            {                                                   // Read the remaining 3 bytes
                uint8_t checksum = data[0] + data[1] + data[2]; // Checksum
                if (checksum == data[3])
                {
                    uint16_t distance = ((uint16_t)data[1] << 8) | data[2]; // Calculate and return the distance
                    return (int)distance;
                }
            }
        }
    }
    return ERROR_DISTANCE;
}

int DistanceSensor::getDistance()
{
    // lazy allocate buffer on first call
    if (_buffer == nullptr)
    {
        _bufferSize = 5;
        _buffer = (int *)malloc(sizeof(int) * _bufferSize);
        for (size_t i = 0; i < _bufferSize; ++i)
            _buffer[i] = ERROR_DISTANCE;
        _index = 0;
        _count = 0;
    }

    int reading = readRawDistance();
    if (reading == ERROR_DISTANCE)
    {
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
