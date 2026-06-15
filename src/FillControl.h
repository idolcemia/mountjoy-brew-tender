#ifndef FILL_CONTROL_H
#define FILL_CONTROL_H

#include <Arduino.h>
#include <lvgl.h>
#include "DistanceSensor.h"

enum FillControlState
{
    FILL_RESET = 0,
    FILL_RUN = 1,
    FILL_PAUSE = 2,
    FILL_DONE = 3
};

/**
 * Controls liquid filling using a fluid level sensor and solenoid valve.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class FillControl
{
private:
    // The volume of liquid that has been filled, in liters.
    float _volumeLiters;
    int _valvePin;
    int _targetDistance;
    float _surfaceAreaCm2;
    int _startDistance;
    bool _hasStartDistance;


public:
    /**
     * Constructor
     *
     */

    FillControlState _state = FILL_DONE;
    unsigned long _startTimeMillis = 0;
    float _fillAmount = 0.0f;

    // Actual fill time is 56 seconds.
    unsigned long _fillTimeMillis = 0; // Will be calculated in begin()

    void begin();

    FillControl(
        int valvePin,
        int targetDistance,
        float surfaceAreaCm2 = 707.0f,
        float fillAmount = 0.0f)
        : _valvePin(valvePin),
          _targetDistance(targetDistance),
          _surfaceAreaCm2(surfaceAreaCm2),
          _startDistance(DistanceSensor::ERROR_DISTANCE),
          _hasStartDistance(false),
          _fillAmount(fillAmount)
    {
    }

    float getFillVolume();

    /**
     * Start filling
     */
    void start();

    /**
     * Stop filling
     */
    void stop();

    /**
     * Reset the fill control state and stop filling
     */
    void reset();

    /**
     * Update the fill control state
     */
    void update();

    void updateUI();

    int getDistance();
};

#endif // FILL_CONTROL_H
