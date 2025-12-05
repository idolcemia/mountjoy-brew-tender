#include "PasteurizerRelays.h"

// ---------------- Constructor ----------------
PasteurizerRelays::PasteurizerRelays(
    int opPin,
    int heatPin,
    int chillPin,
    int pumpPin)
    : _operationPin(opPin), _heatPin(heatPin), _chillPin(chillPin), _pumpPin(pumpPin)
{
}

// ---------------- Initialize pins ----------------
void PasteurizerRelays::begin()
{
    pinMode(_operationPin, OUTPUT);
    pinMode(_heatPin, OUTPUT);
    pinMode(_chillPin, OUTPUT);
    pinMode(_pumpPin, OUTPUT);

    // Set all relays to OFF at startup
    deactivateOperationRelay();
    deactivateHeatRelay();
    deactivateChillRelay();
    deactivatePumpRelay();
}

// ---------------- Relay helpers ----------------
void PasteurizerRelays::setRelay(int pin, bool state)
{
    // HIGH = ON for most relay shields; change if your shield is active LOW
    digitalWrite(pin, state ? HIGH : LOW);
}

// ---------------- Operation ----------------
void PasteurizerRelays::activateOperationRelay() { setRelay(_operationPin, true); }
void PasteurizerRelays::deactivateOperationRelay() { setRelay(_operationPin, false); }

// ---------------- Heat ----------------
void PasteurizerRelays::activateHeatRelay() { setRelay(_heatPin, true); }
void PasteurizerRelays::deactivateHeatRelay() { setRelay(_heatPin, false); }

// ---------------- Chill ----------------
void PasteurizerRelays::activateChillRelay() { setRelay(_chillPin, true); }
void PasteurizerRelays::deactivateChillRelay() { setRelay(_chillPin, false); }

// ---------------- Pump ----------------
void PasteurizerRelays::activatePumpRelay() { setRelay(_pumpPin, true); }
void PasteurizerRelays::deactivatePumpRelay() { setRelay(_pumpPin, false); }
