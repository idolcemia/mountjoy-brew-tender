// DistanceSensor_A02YYUW.h
#ifndef DISTANCESENSOR_A02YYUW_H
#define DISTANCESENSOR_A02YYUW_H

#include <Arduino.h>

class DistanceSensor_A02YYUW
{
public:
    static const int ERROR_DISTANCE = -1;

    // constructor: optional target distance (mm), optional Stream pointer for serial, optional buffer size
    DistanceSensor_A02YYUW(uint16_t targetDistance = 0, Stream *serial = &Serial1, size_t bufferSize = 5);

    // Call in setup() to initialize default serial if desired (defaults to 9600)
    void begin(unsigned long baud = 9600);

    // Returns averaged distance in millimeters, or ERROR_DISTANCE on failure
    int getDistance();

    // Returns (distance - target): 0 when reached, positive when target is being approached, negative when exceeded
    int getDistanceToTarget();

    void setTarget(uint16_t targetDistance);
    uint16_t _target;

private:
    Stream *_serial;

    size_t _bufferSize;
    int *_buffer;
    size_t _index;
    size_t _count;

    int readRawDistance();
    uint8_t readN(uint8_t *buf, size_t len, unsigned long timeoutMs = 200);
};

#endif
