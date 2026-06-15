// DistanceSensor.h
#ifndef DISTANCESENSOR
#define DISTANCESENSOR

#include <Arduino.h>

// Class for reading distance from a sensor
// Assumes the sensor is connected via serial communication

class DistanceSensor
{
public:
    // constructor: optional target distance (mm), optional Stream pointer for serial, optional buffer size
    DistanceSensor(void);

    // Call in setup() to initialize default serial if desired (defaults to 9600)
    void begin(unsigned long baud = 9600);

    // Returns averaged distance in millimeters, or ERROR_DISTANCE on failure
    int getDistance(void);

    static const int ERROR_DISTANCE = -1; // Error return value

private:
    const uint8_t HEADER_BYTE = 0xFF;
    const unsigned long READ_TIMEOUT_MS = 200;

    uint8_t readN(uint8_t *buf, size_t len, unsigned long timeoutMs = 200);

    int readRawDistance();

    // smoothing buffer to avoid destructive reads from the serial stream
    size_t _bufferSize;
    int *_buffer;
    size_t _index;
    size_t _count;
};

#endif
