#ifndef UI_DEBUGLOG_H
#define UI_DEBUGLOG_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Forward declaration of the WiFi connect screen
    extern lv_obj_t *ui_WiFiConnectScreen;

    // Debug Log screen objects
    extern lv_obj_t *ui_DebugLogScreen;
    extern lv_obj_t *ui_LogContainer;
    extern lv_obj_t *ui_LogLabel;

    // Initialize the Debug Log screen
    extern void ui_DebugLog_screen_init();
    extern void ui_DebugLogScreenUpdate();
    extern void ui_DebugLog_screen_destroy();

    static void _attachDebugLogCallback();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // UI_DEBUGLOG_H
