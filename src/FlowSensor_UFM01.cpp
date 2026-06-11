#include "FlowSensor_UFM01.h"

#include <OneWire.h>
#include <Wire.h>
#include "Globals.h"

#include <ScioSense_UFM01.h>


FlowSensor_UFM01::~FlowSensor_UFM01()
{
    delete _ufm01;
    _ufm01 = nullptr;
    delete _oneWire;
    _oneWire = nullptr;
}


void FlowSensor_UFM01::begin(void)
{
    _state = FlowSensorState::FLOW_RUN;

    if (_oneWire == nullptr)
    {
        _oneWire = new OneWire(_sensorPin);
    }
    if (_ufm01 == nullptr)
    {
        _ufm01 = new UFM01();
    }

    _ufm01->begin(_oneWire);

    _initialized = true;

  
    while (_ufm01->init() == false)
    {
        Serial.println("Error -- The UFM01 is not connected.");
        delay(1000);
    }

    while (_ufm01->clearAccumulatedFlow() == false)
    {
        Serial.println("Trying to clear the accumulated flow");
        delay(1000);
    }

  

}


int FlowSensor_UFM01::readSensor(int sensorPin)
{
    if (_ufm01 == nullptr)
    {
        return -1;
    }

    if (_ufm01->update() != RESULT_OK)
    {
        return -1;
    }

    Serial.print("AccF[l]:");
    Serial.print(_ufm01->getAccumulatedFlowLiters());

    Serial.print("\tInstF[l/hr]:");
    Serial.print(_ufm01->getInstantFlowLitersPerHour());

    Serial.print("\tT[C]:");
    Serial.println(_ufm01->getTempCelsius());

    delay(1000);
    return 0;
}
   