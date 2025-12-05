#include <Arduino.h>
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"

// Helper function to log button state change
static void logButtonState(const char *name, bool state)
{
    logger.info("[PasteurizerManualControlEvents] " + String(name) + " button " + (state ? "enabled" : "disabled"));
}

// Operation Button Events
void ui_event_OperationButtonEnabled(lv_event_t *e)
{
    logButtonState("Operation", true);
    pasteurizerRelays.activateOperationRelay();
}

void ui_event_OperationButtonDisabled(lv_event_t *e)
{
    logButtonState("Operation", false);
    pasteurizerRelays.deactivateOperationRelay();
}

// Heat Button Events
void ui_event_HeatButtonEnabled(lv_event_t *e)
{
    logButtonState("Heat", true);
    pasteurizerRelays.activateHeatRelay();
}

void ui_event_HeatButtonDisabled(lv_event_t *e)
{
    logButtonState("Heat", false);
    pasteurizerRelays.deactivateHeatRelay();
}

// Chill Button Events
void ui_event_ChillButtonEnabled(lv_event_t *e)
{
    logButtonState("Chill", true);
    pasteurizerRelays.activateChillRelay();
}

void ui_event_ChillButtonDisabled(lv_event_t *e)
{
    logButtonState("Chill", false);
    pasteurizerRelays.deactivateChillRelay();
}

// Pump Button Events
void ui_event_PumpButtonEnabled(lv_event_t *e)
{
    logButtonState("Pump", true);
    pasteurizerRelays.activatePumpRelay();
}

void ui_event_PumpButtonDisabled(lv_event_t *e)
{
    logButtonState("Pump", false);
    pasteurizerRelays.deactivatePumpRelay();
}

// Generic button handler to toggle state and call appropriate event
void handleManualControlButton(
    lv_event_t *e,
    lv_obj_t *button, bool &state,
    void (*onEnabled)(lv_event_t *),
    void (*onDisabled)(lv_event_t *))
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        state = !state;

        // Update button color
        lv_color_t color = state ? lv_color_hex(0x28A745) : lv_color_hex(0xDC3545);
        lv_obj_set_style_bg_color(button, color, LV_PART_MAIN);

        // Call appropriate event
        if (state && onEnabled)
            onEnabled(e);
        if (!state && onDisabled)
            onDisabled(e);
    }
}

// Connect the generic handler to each button
// void ui_event_OperationButton(lv_event_t *e)
// {
//     handleManualControlButton(
//         e, ui_OperationButton,
//         stateOperation,
//         ui_event_OperationButtonEnabled,
//         ui_event_OperationButtonDisabled);
// }

// void ui_event_HeatButton(lv_event_t *e)
// {
//     handleManualControlButton(
//         e, ui_HeatButton, stateHeat,
//         ui_event_HeatButtonEnabled,
//         ui_event_HeatButtonDisabled);
// }

// void ui_event_ChillButton(lv_event_t *e)
// {
//     handleManualControlButton(
//         e, ui_ChillButton, stateChill,
//         ui_event_ChillButtonEnabled,
//         ui_event_ChillButtonDisabled);
// }

// void ui_event_PumpButton(lv_event_t *e)
// {
//     handleManualControlButton(
//         e, ui_PumpButton, statePump,
//         ui_event_PumpButtonEnabled,
//         ui_event_PumpButtonDisabled);
// }
