#include "FillControl.h"
#include "ui/ui.h"
#include "lvgl.h"
#include "DistanceSensor.h"

extern DistanceSensor distanceSensor;

void FillControl::begin()
{
    pinMode(_valvePin, OUTPUT);
    digitalWrite(_valvePin, LOW); // Ensure valve is closed at startup
                                  //   _sensor.begin();
    _startDistance = DistanceSensor::ERROR_DISTANCE;
    _hasStartDistance = false;
    _state = FillControlState::FILL_DONE;
    _volumeLiters = 0.0f;
}

void FillControl::start()
{
    digitalWrite(_valvePin, HIGH);
    _state = FillControlState::FILL_RUN;
    _startTimeMillis = millis();

    if (!_hasStartDistance)
    {
        int currentDistance = distanceSensor.getDistance();
        if (currentDistance != DistanceSensor::ERROR_DISTANCE)
        {
            _startDistance = currentDistance;
            _hasStartDistance = true;
        }
    }
}

void FillControl::stop()
{
    digitalWrite(_valvePin, LOW);

    if (_state == FillControlState::FILL_RUN)
    {
        _state = FillControlState::FILL_PAUSE;
    }
    else
    {
        _state = FillControlState::FILL_DONE;
    }
}

void FillControl::reset()
{
    _state = FillControlState::FILL_DONE;
    lv_obj_clear_state(ui_switchFill, LV_STATE_CHECKED);
    lv_slider_set_value(ui_sliderFill, 0, LV_ANIM_OFF);

    stop();
}

void FillControl::updateUI()
{
    float fillPercent = _hasStartDistance ? distanceSensor.getDistance() / abs(_targetDistance - _startDistance) * 100.0f : 0.0f;

    int currentDistance = distanceSensor.getDistance();
    lv_label_set_text(ui_labelFillCm, String(currentDistance).c_str());
    lv_label_set_text(ui_labelFillLiters, String(getFillVolume()).c_str());
    lv_bar_set_value(ui_sliderFill, fillPercent, LV_ANIM_OFF);
}

float FillControl::getFillVolume()
{
    if (!_hasStartDistance)
    {
        int currentDistance = distanceSensor.getDistance();
        if (currentDistance != DistanceSensor::ERROR_DISTANCE)
        {
            _startDistance = currentDistance;
            _hasStartDistance = true;
        }
    }

    int currentDistance = distanceSensor.getDistance();
    if (currentDistance == DistanceSensor::ERROR_DISTANCE)
    {

        return 0.0f;
    }
    else
    {
        if (!_hasStartDistance)
        {
            _startDistance = currentDistance;
            _hasStartDistance = true;
        }
        float deltaMm = float(_startDistance - currentDistance);
        _volumeLiters = (deltaMm / 10.0f) * _surfaceAreaCm2 / 1000.0f;
        if (_volumeLiters < 0.0f)
        {
            _volumeLiters = 0.0f;
        }
    }

    return _volumeLiters;
}

void FillControl::update()
{
    int currentDistance = distanceSensor.getDistance();

    if (currentDistance <= _targetDistance)
    {
        stop();
    }
}

int FillControl::getDistance()
{
    return distanceSensor.getDistance();
}