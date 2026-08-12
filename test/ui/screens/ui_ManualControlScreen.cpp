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

lv_obj_t *ui_DisableSOLOButton = nullptr;
lv_obj_t *ui_CoolCycleSOLOButton = nullptr;
lv_obj_t *ui_OperationButton = nullptr;
lv_obj_t *ui_WallHeaterButton = nullptr;
lv_obj_t *ui_PumpButton = nullptr;
lv_obj_t *ui_CoreTempLabel = nullptr;
lv_obj_t *ui_ChamberTempLabel = nullptr;

// Button states
bool stateDisableSOLO = false;
bool stateCoolCycleSOLO = false;
bool stateOperation = false;
bool stateWallHeater = false;
bool statePump = false;
bool stateCycle = false;

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

void event_DisableSOLOEnabled(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {

        //  logState("Disable SOLO");

        ui_event_OperationButtonEnabled(e);
    }
}

void event_DisableSOLODisabled(lv_event_t *e)
{
    {

        ui_event_OperationButtonDisabled(e);
    }
}

void event_CoolCycleSOLOEnabled(lv_event_t *e)
{

    ui_event_CoolCycleSOLOEnabled(e);
}

void event_CoolCycleSOLODisabled(lv_event_t *e)
{
    ui_event_CoolCycleSOLODisabled(e);
}

void event_OperationButtonEnabled(lv_event_t *e)
{
    ui_event_OperationButtonEnabled(e);
}

void event_OperationButtonDisabled(lv_event_t *e)
{
    ui_event_OperationButtonDisabled(e);
}

void event_WallHeaterEnabled(lv_event_t *e)
{
    ui_event_WallHeaterEnabled(e);
}

void event_WallHeaterDisabled(lv_event_t *e)
{
    ui_event_WallHeaterDisabled(e);
}

void event_PumpButtonEnabled(lv_event_t *e)
{
    ui_event_PumpButtonEnabled(e);
}

void event_PumpButtonDisabled(lv_event_t *e)
{
    ui_event_PumpButtonDisabled(e);
}

void event_ChillerButtonEnabled(lv_event_t *e)
{
    ui_event_ChillerButtonEnabled(e);
}

void event_ChillerButtonDisabled(lv_event_t *e)
{
    ui_event_ChillerButtonDisabled(e);
}

void event_CycleButtonEnabled(lv_event_t *e)
{
    ui_event_CycleButtonEnabled(e);
}

void event_CycleButtonDisabled(lv_event_t *e)
{
    ui_event_CycleButtonDisabled(e);
}

/* ============================================================
   LOCAL WRAPPER EVENT HANDLERS for ManualControlScreen buttons
   ============================================================ */

static void ui_ManualControl_event_CoolCycleSOLO(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateCoolCycleSOLO = !stateCoolCycleSOLO;
        update_button_color(ui_CoolCycleSOLOButton, stateCoolCycleSOLO);
        logState("Cool Cycle SOLO", stateCoolCycleSOLO);

        if (stateCoolCycleSOLO)
            ui_event_CoolCycleSOLOEnabled(e);
        else
            ui_event_CoolCycleSOLODisabled(e);
    }
}

static void ui_ManualControl_event_Operation(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateOperation = !stateOperation;
        update_button_color(ui_OperationButton, stateOperation);
        logState("Operation", stateOperation);

        if (stateOperation)
            ui_event_OperationButtonEnabled(e);
        else
            ui_event_OperationButtonDisabled(e);
    }
}

static void ui_ManualControl_event_WallHeater(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateWallHeater = !stateWallHeater;
        update_button_color(ui_WallHeaterButton, stateWallHeater);
        logState("Wall Heater", stateWallHeater);

        if (stateWallHeater)
            ui_event_WallHeaterEnabled(e);
        else
            ui_event_WallHeaterDisabled(e);
    }
}

static void ui_ManualControl_event_Pump(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        statePump = !statePump;
        update_button_color(ui_PumpButton, statePump);
        logState("Pump", statePump);

        if (statePump)
            ui_event_PumpButtonEnabled(e);
        else
            ui_event_PumpButtonDisabled(e);
    }
}

static void ui_ManualControl_event_DisableSOLO(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        stateDisableSOLO = !stateDisableSOLO;
        update_button_color(ui_DisableSOLOButton, stateDisableSOLO);
        logState("Disable SOLO", stateDisableSOLO);

        if (stateDisableSOLO)
            event_DisableSOLOEnabled(e);
        else
            event_DisableSOLODisabled(e);
    }
}

/* ============================================================
   SCREEN INIT
   ============================================================ */
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

    // Temperature container
    lv_obj_t *tempContainer = lv_obj_create(ui_ManualControlScreen);
    lv_obj_set_size(tempContainer, 700, 140);
    lv_obj_align(tempContainer, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_set_style_bg_color(tempContainer, lv_color_hex(0x2E2E2E), LV_PART_MAIN);
    lv_obj_set_style_radius(tempContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(tempContainer, 20, LV_PART_MAIN);
    lv_obj_set_flex_flow(tempContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tempContainer, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_CoreTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(ui_CoreTempLabel, "Core Temp:\n-- C\n-- F");
    lv_obj_set_style_text_font(ui_CoreTempLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_CoreTempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    ui_ChamberTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(ui_ChamberTempLabel, "Chamber Temp:\n-- C\n-- F");
    lv_obj_set_style_text_font(ui_ChamberTempLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_ChamberTempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // ---------- BUTTON CREATOR ----------
    auto create_button = [](lv_obj_t *parent, const char *text, lv_event_cb_t cb) -> lv_obj_t *
    {
        lv_obj_t *btn = lv_button_create(parent);

        // Smaller height so everything fits
        lv_obj_set_width(btn, LV_PCT(48)); // two per row
        lv_obj_set_height(btn, 60);        // reduced from 80

        lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
        update_button_color(btn, false);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, text);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_24, LV_PART_MAIN);
        lv_obj_center(lbl);

        lv_obj_add_event_cb(btn, cb, LV_EVENT_ALL, nullptr);
        return btn;
    };

    // ---------- BUTTON CONTAINER ----------
    lv_obj_t *btnContainer = lv_obj_create(ui_ManualControlScreen);
    lv_obj_set_size(btnContainer, 700, LV_SIZE_CONTENT); // height adjusts to content
    lv_obj_align_to(btnContainer, tempContainer, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_set_style_bg_color(btnContainer, lv_color_hex(0x2E2E2E), LV_PART_MAIN);
    lv_obj_set_style_radius(btnContainer, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(btnContainer, 10, LV_PART_MAIN);    // reduce padding
    lv_obj_set_style_pad_row(btnContainer, 10, LV_PART_MAIN);    // reduce row spacing
    lv_obj_set_style_pad_column(btnContainer, 10, LV_PART_MAIN); // spacing between columns

    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(
        btnContainer,
        LV_FLEX_ALIGN_SPACE_EVENLY,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_START);

    // ---------- CREATE BUTTONS ----------
    // First two rows, 2 per row
    ui_DisableSOLOButton = create_button(btnContainer, "Disable SOLO", ui_ManualControl_event_DisableSOLO);
    ui_CoolCycleSOLOButton = create_button(btnContainer, "Cool SOLO", ui_ManualControl_event_CoolCycleSOLO);
    ui_OperationButton = create_button(btnContainer, "Operation", ui_ManualControl_event_Operation);
    ui_WallHeaterButton = create_button(btnContainer, "Wall Heater", ui_ManualControl_event_WallHeater);

    // Last button, full width
    ui_PumpButton = lv_button_create(btnContainer);
    lv_obj_set_width(ui_PumpButton, LV_PCT(100)); // full width
    lv_obj_set_height(ui_PumpButton, 60);         // same as other buttons
    lv_obj_set_style_radius(ui_PumpButton, 8, LV_PART_MAIN);
    update_button_color(ui_PumpButton, false);

    lv_obj_t *lblPump = lv_label_create(ui_PumpButton);
    lv_label_set_text(lblPump, "Pump");
    lv_obj_set_style_text_font(lblPump, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(lblPump);

    lv_obj_add_event_cb(ui_PumpButton, ui_ManualControl_event_Pump, LV_EVENT_ALL, nullptr);

    // Load screen
    lv_scr_load(ui_ManualControlScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_ManualControlScreen);
    ui_GlobalLabels::updateNetworkStatus(ui_ManualControlScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_ManualControlScreen);
}

/* ============================================================
   Destroy & Update (unchanged)
   ============================================================ */

void ui_ManualControl_screen_destroy()
{
    if (ui_ManualControlScreen)
        lv_obj_clean(ui_ManualControlScreen);

    ui_ManualControlTitle = nullptr;
    ui_DisableSOLOButton = nullptr;
    ui_CoolCycleSOLOButton = nullptr;
    ui_OperationButton = nullptr;
    ui_WallHeaterButton = nullptr;
    ui_PumpButton = nullptr;
    ui_CoreTempLabel = nullptr;
    ui_ChamberTempLabel = nullptr;
}

static float celsiusToFahrenheit(float celsius)
{
    return celsius * 9.0 / 5.0 + 32.0;
}

void ui_ManualControl_screen_update(float coreTemp, float chamberTemp)
{
    logger.info("[ui_ManualControl_screen_update] Function called");
    logger.info(String("[ui_ManualControl_screen_update] Core Temp: ") + String(coreTemp, 2) + " °C");
    logger.info(String("[ui_ManualControl_screen_update] Chamber Temp: ") + String(chamberTemp, 2) + " °C");
    logger.info(String("[ui_ManualControl_screen_update] Chamber Temp isnan: ") + String(isnan(chamberTemp)));

    if (!ui_ManualControlScreen)
    {
        logger.error("[ui_ManualControl_screen_update] ui_ManualControlScreen is NULL!");
        return;
    }
    logger.info("[ui_ManualControl_screen_update] Screen object is valid");

    if (ui_CoreTempLabel)
    {
        if (isnan(coreTemp))
        {
            logger.info("[ui_ManualControl_screen_update] Core temp is NAN, setting to '--'");
            lv_label_set_text(ui_CoreTempLabel, "Core Temp:\n-- C\n-- F");
        }
        else
        {
            float coreTempF = celsiusToFahrenheit(coreTemp);
            logger.info(String("[ui_ManualControl_screen_update] Setting core temp label to: ") + String(coreTemp, 1) + " C / " + String(coreTempF, 1) + " F");
            char buf[80];
            snprintf(buf, sizeof(buf), "Core Temp:\n%.1f C\n%.1f F", coreTemp, coreTempF);
            logger.info(String("[ui_ManualControl_screen_update] Buffer contents: ") + String(buf));
            lv_label_set_text(ui_CoreTempLabel, buf);
        }
    }
    else
    {
        logger.error("[ui_ManualControl_screen_update] ui_CoreTempLabel is NULL!");
    }

    if (ui_ChamberTempLabel)
    {
        if (isnan(chamberTemp))
        {
            logger.info("[ui_ManualControl_screen_update] Chamber temp is NAN, setting to '--'");
            lv_label_set_text(ui_ChamberTempLabel, "Chamber Temp:\n-- C\n-- F");
        }
        else
        {
            float chamberTempF = celsiusToFahrenheit(chamberTemp);
            logger.info(String("[ui_ManualControl_screen_update] Setting chamber temp label to: ") + String(chamberTemp, 1) + " C / " + String(chamberTempF, 1) + " F");
            char buf[80];
            snprintf(buf, sizeof(buf), "Chamber Temp:\n%.1f C\n%.1f F", chamberTemp, chamberTempF);
            logger.info(String("[ui_ManualControl_screen_update] Buffer contents: ") + String(buf));
            lv_label_set_text(ui_ChamberTempLabel, buf);
        }
    }
    else
    {
        logger.error("[ui_ManualControl_screen_update] ui_ChamberTempLabel is NULL!");
    }

    logger.info("[ui_ManualControl_screen_update] Update complete");

    // ui_GlobalButtons::updateGlobalButtons(ui_ManualControlScreen);
    // ui_GlobalLabels::updateNetworkStatus(ui_ManualControlScreen);
    // ui_GlobalLabels::updateUserSelectionLabel(ui_ManualControlScreen);
}