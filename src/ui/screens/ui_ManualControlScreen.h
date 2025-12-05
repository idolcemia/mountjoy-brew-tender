#ifndef UI_PASTEURIZER_MANUAL_CONTROL_H
#define UI_PASTEURIZER_MANUAL_CONTROL_H

#include "lvgl.h"

// Screen object
extern lv_obj_t *ui_ManualControlScreen;

// Title
extern lv_obj_t *ui_ManualControlTitle;

// Buttons
extern lv_obj_t *ui_OperationButton;
extern lv_obj_t *ui_HeatButton;
extern lv_obj_t *ui_ChillButton;
extern lv_obj_t *ui_PumpButton;

// Button states
extern bool stateOperation;
extern bool stateHeat;
extern bool stateChill;
extern bool statePump;

// Temperature labels
extern lv_obj_t *ui_CoreTempLabel;
extern lv_obj_t *ui_ChamberTempLabel;

// Screen lifecycle functions
void ui_ManualControl_screen_init();
void ui_ManualControl_screen_destroy();

// Update temperature displays
void ui_ManualControl_screen_update(float coreTemp, float chamberTemp);

#endif // UI_PASTEURIZER_MANUAL_CONTROL_H
