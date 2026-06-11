#ifndef FLOW_SENSOR_UFM01_H
#define FLOW_SENSOR_UFM01_H

#include <Arduino.h>
#include <OneWire.h>
#include <lvgl.h>

class UFM01;

enum FlowSensorState
{
    FLOW_RESET = 0,
    FLOW_RUN = 1,
    FLOW_PAUSE = 2,
    FLOW_DONE = 3
};

/**
 * Measures flow using a ScioSense UFM01 flow sensor.  This class handles the communication with the sensor
 *  as well as smoothing of the flow readings and control of the fill process.     It also provides an
 * interface for starting and stopping the fill process, and for resetting the state when a new fill is started.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class FlowSensor_UFM01
{
private:
    int _sensorPin;

    int _recentVolumeIndex = 0;
    int _recentVolumeCount = 0;
    float _recentVolumeSum = 0.0f;

#define SERIES_RESISTOR 560
#define ZERO_VOLUME_RESISTANCE 540  // Resistance reading when the tube is empty.  This will depend on your specific sensor and setup, so you should measure it with an empty tube to get an accurate value.
#define CALIBRATION_RESISTANCE 1800 // Resistance at 27.5 cm of rise.
#define CALIBRATION_VOLUME 19.23    // Volume in liters at 27.5 cm of rise.  Assuing a 30cm diameter tube,

    // 699 cubic centimeters per cm of rise, so 27.5 cm * 699 cc/cm = 19227.5 cc = 19.23 liters.  Adjust the calibration volume if your tube diameter is different.

public:
    /**
     * Constructor
     *
     */

    FlowSensorState _state = FLOW_DONE;
    unsigned long _startTimeMillis = 0;
    float _fillAmount;
    uint8_t _address;
    uint8_t _readCommand;
    bool _useReadCommand = false;
    size_t _frameLength = 0;
    float _flowScaleLpmPerCount = 0.0f;
    float _temperatureScaleCPerCount = 0.0f;
    uint32_t _sampleIntervalMs = 0;
    uint32_t _lastPollMs = 0;
    uint32_t _lastValidReadMs = 0;

    // Actual fill time is 56 seconds.
    unsigned long _fillTimeMillis = 0; // Will be calculated in begin()

    FlowSensor_UFM01(
        int sensorPin) : _sensorPin(sensorPin)
    {
    }

    ~FlowSensor_UFM01();

    void begin();

    /**
     * Start reading
     */
    void start();

    /**
     * Stop filling
     */
    void stop();

    int readSensor(int sensorPin);

    /**
     * Reset the fill control state and stop filling
     */
    void reset();

private:
    bool _initialized = false;
    OneWire *_oneWire = nullptr;
    UFM01 *_ufm01 = nullptr;

    void setup(void);
};

#endif // FLOW_SENSOR_UFM01_H
