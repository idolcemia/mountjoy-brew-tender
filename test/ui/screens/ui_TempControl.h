#ifndef UI_TEMPCONTROL_H
#define UI_TEMPCONTROL_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // SCREEN: ui_TempControlScreen
    extern void ui_TempControl_screen_init(void);
    extern void ui_TempControlScreenUpdate();
    extern void ui_TempControl_screen_destroy(void);
    extern lv_obj_t *ui_TempControlScreen;

    // Title Label
    extern lv_obj_t *ui_TempControlTitle;

    // Target Temperature Spinbox
    extern lv_obj_t *ui_TargetTempLabel;
    extern lv_obj_t *ui_TargetTempSpinbox;
    extern void ui_event_TargetTempSpinbox(lv_event_t *e);

    // PID Coefficients Spinboxes
    extern lv_obj_t *ui_KpLabel;
    extern lv_obj_t *ui_KpSpinbox;
    extern void ui_event_KpSpinbox(lv_event_t *e);

    extern lv_obj_t *ui_KiLabel;
    extern lv_obj_t *ui_KiSpinbox;
    extern void ui_event_KiSpinbox(lv_event_t *e);

    extern lv_obj_t *ui_KdLabel;
    extern lv_obj_t *ui_KdSpinbox;
    extern void ui_event_KdSpinbox(lv_event_t *e);

    // Ramp Rate Limits Spinboxes
    extern lv_obj_t *ui_RampUpLabel;
    extern lv_obj_t *ui_RampUpSpinbox;
    extern void ui_event_RampUpSpinbox(lv_event_t *e);

    extern lv_obj_t *ui_RampDownLabel;
    extern lv_obj_t *ui_RampDownSpinbox;
    extern void ui_event_RampDownSpinbox(lv_event_t *e);

    // Crossover Distance Spinbox
    extern lv_obj_t *ui_CrossoverLabel;
    extern lv_obj_t *ui_CrossoverSpinbox;
    extern void ui_event_CrossoverSpinbox(lv_event_t *e);

    // Integrator Windup Limit Spinbox
    extern lv_obj_t *ui_IntegratorLabel;
    extern lv_obj_t *ui_IntegratorSpinbox;
    extern void ui_event_IntegratorSpinbox(lv_event_t *e);

    // Save and Cancel Buttons
    extern lv_obj_t *ui_SaveSettingsButton;
    extern void ui_event_SaveSettingsButton(lv_event_t *e);

    extern lv_obj_t *ui_CancelButton;
    extern void ui_event_CancelButton(lv_event_t *e);

    // Status Label
    extern lv_obj_t *ui_SettingsStatusLabel;

    // CUSTOM VARIABLES (optional)
    extern lv_obj_t *uic_TempControlTitle;
    extern lv_obj_t *uic_SaveSettingsButton;
    extern lv_obj_t *uic_CancelButton;
    extern lv_obj_t *uic_SettingsStatusLabel;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif