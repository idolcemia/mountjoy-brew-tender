#include "ui/ui.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"
#include "Globals.h"
#include "ui/events/events.h"
#include "lvgl.h"
#include "ui/events/PasteurizerManualControlEvents.h"

// Screen and widget declarations
lv_obj_t *ui_ManualControlScreen = nullptr;
lv_obj_t *ui_ManualControlTitle = nullptr;

lv_obj_t *ui_OperationButton = nullptr;
lv_obj_t *ui_HeatButton = nullptr;
lv_obj_t *ui_ChillButton = nullptr;
lv_obj_t *ui_PumpButton = nullptr;

lv_obj_t *ui_CoreTempLabel = nullptr;
lv_obj_t *ui_ChamberTempLabel = nullptr;

// Button states
bool stateOperation = false;
bool stateHeat = false;
bool stateChill = false;
bool statePump = false;

static void logState(const char *name, bool state)
{
    logger.info(String("[ui_ManualControlScreen] ") + String(name) + " button pressed. New state: " + (state ? "ON" : "OFF"));
}

// Helper: update button color based on state
static void update_button_color(lv_obj_t *btn, bool state)
{
    lv_color_t color = state ? lv_color_hex(0x28A745) : lv_color_hex(0xDC3545); // green/red
    lv_obj_set_style_bg_color(btn, color, LV_PART_MAIN);
}

// Event handlers for buttons
static void ui_event_OperationButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateOperation = !stateOperation;
        update_button_color(ui_OperationButton, stateOperation);
        logState("Operation", stateOperation);
        // trigger relay control
        if (stateOperation)
        {
            ui_event_OperationButtonEnabled(e);
        }
        else
        {
            ui_event_OperationButtonDisabled(e);
        }
    }
}

static void ui_event_HeatButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateHeat = !stateHeat;
        update_button_color(ui_HeatButton, stateHeat);
        logState("Heat", stateHeat);
        // trigger relay control
        if (stateHeat)
        {
            ui_event_HeatButtonEnabled(e);
        }
        else
        {
            ui_event_HeatButtonDisabled(e);
        }
    }
}

static void ui_event_ChillButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateChill = !stateChill;
        update_button_color(ui_ChillButton, stateChill);
        logState("Chill", stateChill);
        // trigger relay control
        if (stateChill)
        {
            ui_event_ChillButtonEnabled(e);
        }
        else
        {
            ui_event_ChillButtonDisabled(e);
        }
    }
}

static void ui_event_PumpButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        statePump = !statePump;
        update_button_color(ui_PumpButton, statePump);
        logState("Pump", statePump);
        // trigger relay control
        if (statePump)
        {
            ui_event_PumpButtonEnabled(e);
        }
        else
        {
            ui_event_PumpButtonDisabled(e);
        }
    }
}

// Screen init
void ui_ManualControl_screen_init()
{
    ui_ManualControlScreen = lv_obj_create(nullptr);
    lv_obj_remove_flag(ui_ManualControlScreen, LV_OBJ_FLAG_SCROLLABLE);

    // Background
    lv_obj_set_style_bg_color(ui_ManualControlScreen, lv_color_hex(0x1E1E1E), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_ManualControlScreen, 255, LV_PART_MAIN);

    // Title
    ui_ManualControlTitle = lv_label_create(ui_ManualControlScreen);
    lv_label_set_text(ui_ManualControlTitle, "Pasteurizer Manual Control");
    lv_obj_set_style_text_color(ui_ManualControlTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_ManualControlTitle, &lv_font_montserrat_36, LV_PART_MAIN);
    lv_obj_align(ui_ManualControlTitle, LV_ALIGN_TOP_MID, 0, 10);

    // Temperature displays container
    lv_obj_t *tempContainer = lv_obj_create(ui_ManualControlScreen);
    lv_obj_set_size(tempContainer, 700, 100);
    lv_obj_align(tempContainer, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_set_style_bg_color(tempContainer, lv_color_hex(0x2E2E2E), LV_PART_MAIN);
    lv_obj_set_style_radius(tempContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(tempContainer, 10, LV_PART_MAIN);
    lv_obj_set_flex_flow(tempContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_border_width(tempContainer, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(tempContainer, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    ui_CoreTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(ui_CoreTempLabel, "Core Temp: -- °C");
    lv_obj_set_style_text_font(ui_CoreTempLabel, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_CoreTempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_align(ui_CoreTempLabel, LV_ALIGN_LEFT_MID, 0, 0);

    ui_ChamberTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(ui_ChamberTempLabel, "Chamber Temp: -- °C");
    lv_obj_set_style_text_font(ui_ChamberTempLabel, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_ChamberTempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_align(ui_ChamberTempLabel, LV_ALIGN_RIGHT_MID, 0, 0);

    // Buttons container
    lv_obj_t *btnContainer = lv_obj_create(ui_ManualControlScreen);
    lv_obj_set_size(btnContainer, 700, 200);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_style_bg_color(btnContainer, lv_color_hex(0x2E2E2E), LV_PART_MAIN);
    lv_obj_set_style_radius(btnContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(btnContainer, 20, LV_PART_MAIN);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_border_width(btnContainer, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(btnContainer, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Create Buttons
    auto create_button = [](lv_obj_t *parent, const char *text, lv_event_cb_t cb) -> lv_obj_t *
    {
        lv_obj_t *btn = lv_button_create(parent);
        lv_obj_set_size(btn, 150, 80);
        lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
        update_button_color(btn, false);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, text);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_24, LV_PART_MAIN);
        lv_obj_center(lbl);

        lv_obj_add_event_cb(btn, cb, LV_EVENT_ALL, nullptr);
        return btn;
    };

    ui_OperationButton = create_button(btnContainer, "Operation", ui_event_OperationButton);
    ui_HeatButton = create_button(btnContainer, "Heat", ui_event_HeatButton);
    ui_ChillButton = create_button(btnContainer, "Chill", ui_event_ChillButton);
    ui_PumpButton = create_button(btnContainer, "Pump", ui_event_PumpButton);

    // Load screen
    lv_scr_load(ui_ManualControlScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_ManualControlScreen);
    ui_GlobalLabels::updateNetworkStatus(ui_ManualControlScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_ManualControlScreen);
}

// Screen destroy
void ui_ManualControl_screen_destroy()
{
    if (ui_ManualControlScreen)
        lv_obj_clean(ui_ManualControlScreen);

    ui_ManualControlTitle = nullptr;
    ui_OperationButton = nullptr;
    ui_HeatButton = nullptr;
    ui_ChillButton = nullptr;
    ui_PumpButton = nullptr;
    ui_CoreTempLabel = nullptr;
    ui_ChamberTempLabel = nullptr;
}

// Update temperature labels
void ui_ManualControl_screen_update(float coreTemp, float chamberTemp)
{
    if (!ui_ManualControlScreen)
        return;

    if (ui_CoreTempLabel)
        lv_label_set_text_fmt(ui_CoreTempLabel, "Core Temp: %.1f °C", coreTemp);

    if (ui_ChamberTempLabel)
        lv_label_set_text_fmt(ui_ChamberTempLabel, "Chamber Temp: %.1f °C", chamberTemp);

    ui_GlobalButtons::updateGlobalButtons(ui_ManualControlScreen);
    ui_GlobalLabels::updateNetworkStatus(ui_ManualControlScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_ManualControlScreen);
}
