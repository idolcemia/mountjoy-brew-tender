#pragma once
#include <Arduino.h>

class PasteurizerRelays
{
public:
    // Constructor: pass the 4 relay pins
    PasteurizerRelays(int opPin, int heatPin, int chillPin, int pumpPin);

    // Initialize pins (call in setup)
    void begin();

    // Operation relay
    void activateOperationRelay();
    void deactivateOperationRelay();

    // Heat relay
    void activateHeatRelay();
    void deactivateHeatRelay();

    // Chill relay
    void activateChillRelay();
    void deactivateChillRelay();

    // Pump relay
    void activatePumpRelay();
    void deactivatePumpRelay();

private:
    int _operationPin;
    int _heatPin;
    int _chillPin;
    int _pumpPin;

    // Internal helper
    void setRelay(int pin, bool state);
};
