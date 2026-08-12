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

void ui_ManualControl_event_DisableSOLO(lv_event_t *e);
void ui_ManualControl_event_CoolCycleSOLO(lv_event_t *e);
void ui_ManualControl_event_Operation(lv_event_t *e);
void ui_ManualControl_event_WallHeater(lv_event_t *e);
void ui_ManualControl_event_Pump(lv_event_t *e);
void ui_ManualControl_event_Cycle(lv_event_t *e);

void event_DisableSOLOEnabled(lv_event_t *e);
void event_DisableSOLODisabled(lv_event_t *e);
void event_CoolCycleSOLOEnabled(lv_event_t *e);
void event_CoolCycleSOLODisabled(lv_event_t *e);
void event_OperationButtonEnabled(lv_event_t *e);
void event_OperationButtonDisabled(lv_event_t *e);
void event_WallHeaterEnabled(lv_event_t *e);
void event_WallHeaterDisabled(lv_event_t *e);
void event_PumpButtonEnabled(lv_event_t *e);
void event_PumpButtonDisabled(lv_event_t *e);
void event_ChillerButtonEnabled(lv_event_t *e);
void event_ChillerButtonDisabled(lv_event_t *e);
void event_CycleButtonEnabled(lv_event_t *e);
void event_CycleButtonDisabled(lv_event_t *e);

// Screen lifecycle functions
void ui_ManualControl_screen_init();
void ui_ManualControl_screen_destroy();

// Update temperature displays
void ui_ManualControl_screen_update(float coreTemp, float chamberTemp);

#endif // UI_PASTEURIZER_MANUAL_CONTROL_H
