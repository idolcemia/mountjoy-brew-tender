#include "ui/ui.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "Globals.h"
#include "ui/events/events.h"
#include "ui_TempControl.h"
#include "lvgl.h"

// Mock global variables
struct TempControlSettings
{
    float targetTemp = 80.0;
    float kp = 2.0;
    float ki = 0.15;
    float kd = 1.5;
    float rampUp = 10.0;
    float rampDown = -10.0;
    float crossover = 10.0;
    float integratorLimit = 100.0;
} g_TempSettings;

// Screen and widget declarations
lv_obj_t *ui_TempControlScreen = nullptr;
lv_obj_t *ui_TempControlTitle = nullptr;

lv_obj_t *ui_TargetTempLabel = nullptr;
lv_obj_t *ui_TargetTempSpinbox = nullptr;

lv_obj_t *ui_KpLabel = nullptr;
lv_obj_t *ui_KpSpinbox = nullptr;

lv_obj_t *ui_KiLabel = nullptr;
lv_obj_t *ui_KiSpinbox = nullptr;

lv_obj_t *ui_KdLabel = nullptr;
lv_obj_t *ui_KdSpinbox = nullptr;

lv_obj_t *ui_RampUpLabel = nullptr;
lv_obj_t *ui_RampUpSpinbox = nullptr;

lv_obj_t *ui_RampDownLabel = nullptr;
lv_obj_t *ui_RampDownSpinbox = nullptr;

lv_obj_t *ui_CrossoverLabel = nullptr;
lv_obj_t *ui_CrossoverSpinbox = nullptr;

lv_obj_t *ui_IntegratorLabel = nullptr;
lv_obj_t *ui_IntegratorSpinbox = nullptr;

lv_obj_t *ui_SaveSettingsButton = nullptr;
lv_obj_t *ui_CancelButton = nullptr;
lv_obj_t *ui_SettingsStatusLabel = nullptr;

// Custom variables
lv_obj_t *uic_TempControlTitle = nullptr;
lv_obj_t *uic_SaveSettingsButton = nullptr;
lv_obj_t *uic_CancelButton = nullptr;
lv_obj_t *uic_SettingsStatusLabel = nullptr;

// Helper function to create a row with label and spinbox
static lv_obj_t *create_setting_row(
    lv_obj_t *parent, const char *label_text,
    int32_t range_min, int32_t range_max,
    int32_t initial_value, uint8_t digit_count)
{
    // Create container for this row
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 700, 50);
    lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(row, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(row, 0, LV_PART_MAIN);

    // Label on the left
    lv_obj_t *label = lv_label_create(row);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_22, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 5, 0);

    // Decrement button
    lv_obj_t *btn_dec = lv_button_create(row);
    lv_obj_set_size(btn_dec, 45, 45);
    lv_obj_align(btn_dec, LV_ALIGN_RIGHT_MID, -160, 0);
    lv_obj_set_style_radius(btn_dec, 5, LV_PART_MAIN);
    lv_obj_t *label_dec = lv_label_create(btn_dec);
    lv_label_set_text(label_dec, LV_SYMBOL_MINUS);
    lv_obj_set_style_text_font(label_dec, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(label_dec);

    // Spinbox in the middle-right
    lv_obj_t *spinbox = lv_spinbox_create(row);
    lv_spinbox_set_range(spinbox, range_min, range_max);
    lv_spinbox_set_digit_format(spinbox, digit_count, digit_count - 2);
    lv_spinbox_set_value(spinbox, initial_value);
    lv_obj_set_width(spinbox, 100);
    lv_obj_align(spinbox, LV_ALIGN_RIGHT_MID, -55, 0);
    lv_obj_set_style_bg_color(spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(spinbox, &lv_font_montserrat_22, LV_PART_MAIN);
    lv_obj_set_style_text_align(spinbox, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    // Increment button
    lv_obj_t *btn_inc = lv_button_create(row);
    lv_obj_set_size(btn_inc, 45, 45);
    lv_obj_align(btn_inc, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_style_radius(btn_inc, 5, LV_PART_MAIN);
    lv_obj_t *label_inc = lv_label_create(btn_inc);
    lv_label_set_text(label_inc, LV_SYMBOL_PLUS);
    lv_obj_set_style_text_font(label_inc, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(label_inc);

    // Add event callbacks
    lv_obj_add_event_cb(btn_dec, [](lv_event_t *e)
                        {
        lv_obj_t *spinbox = (lv_obj_t*)lv_event_get_user_data(e);
        lv_spinbox_decrement(spinbox); }, LV_EVENT_CLICKED, spinbox);

    lv_obj_add_event_cb(btn_inc, [](lv_event_t *e)
                        {
        lv_obj_t *spinbox = (lv_obj_t*)lv_event_get_user_data(e);
        lv_spinbox_increment(spinbox); }, LV_EVENT_CLICKED, spinbox);

    return spinbox;
}

// Event handlers
void ui_event_TargetTempSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // Handle target temp change
        // handleTargetTempChange(e);
    }
}

void ui_event_KpSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleKpChange(e);
    }
}

void ui_event_KiSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleKiChange(e);
    }
}

void ui_event_KdSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleKdChange(e);
    }
}

void ui_event_RampUpSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleRampUpChange(e);
    }
}

void ui_event_RampDownSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleRampDownChange(e);
    }
}

void ui_event_CrossoverSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleCrossoverChange(e);
    }
}

void ui_event_IntegratorSpinbox(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        // handleIntegratorChange(e);
    }
}

void ui_event_SaveSettingsButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        // Save all spinbox values to global settings
        g_TempSettings.targetTemp = lv_spinbox_get_value(ui_TargetTempSpinbox) / 10.0f;
        g_TempSettings.kp = lv_spinbox_get_value(ui_KpSpinbox) / 100.0f;
        g_TempSettings.ki = lv_spinbox_get_value(ui_KiSpinbox) / 1000.0f;
        g_TempSettings.kd = lv_spinbox_get_value(ui_KdSpinbox) / 100.0f;
        g_TempSettings.rampUp = lv_spinbox_get_value(ui_RampUpSpinbox) / 10.0f;
        g_TempSettings.rampDown = lv_spinbox_get_value(ui_RampDownSpinbox) / 10.0f;
        g_TempSettings.crossover = lv_spinbox_get_value(ui_CrossoverSpinbox) / 10.0f;
        g_TempSettings.integratorLimit = lv_spinbox_get_value(ui_IntegratorSpinbox) / 10.0f;

        lv_label_set_text(ui_SettingsStatusLabel, "Settings Saved!");

        // timer to clear the message after 5 seconds
        lv_timer_create(
            [](lv_timer_t *timer)
            {
                lv_label_set_text(ui_SettingsStatusLabel, "");
                lv_timer_del(timer); // Delete timer after it fires once
            },
            5000, nullptr); // 5000ms = 5 seconds

        // Apply to PID controller
        // handleSaveSettingsButton(e);
    }
}

void ui_event_CancelButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        lv_label_set_text(ui_SettingsStatusLabel, "Changes Cancelled");

        // timer to clear the message after 5 seconds
        lv_timer_create(
            [](lv_timer_t *timer)
            {
                lv_label_set_text(ui_SettingsStatusLabel, "");
                lv_timer_del(timer); // Delete timer after it fires once
            },
            5000, nullptr); // 5000ms = 5 seconds

        // handleCancelButton(e);
    }
}

void ui_TempControl_screen_init(void)
{
    ui_TempControlScreen = lv_obj_create(nullptr);
    lv_obj_remove_flag(ui_TempControlScreen, LV_OBJ_FLAG_SCROLLABLE);

    // Background styling
    lv_obj_set_style_bg_color(ui_TempControlScreen, lv_color_hex(0xEE7B01), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_TempControlScreen, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_TempControlScreen, lv_color_hex(0x2E1A05), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_TempControlScreen, LV_GRAD_DIR_VER, LV_PART_MAIN);

    // Title
    ui_TempControlTitle = lv_label_create(ui_TempControlScreen);
    lv_label_set_text(ui_TempControlTitle, "Temperature Control");
    lv_obj_set_style_text_color(ui_TempControlTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_TempControlTitle, &lv_font_montserrat_36, LV_PART_MAIN);
    lv_obj_align(ui_TempControlTitle, LV_ALIGN_TOP_MID, 0, 10);

    // Create scrollable container for all settings
    lv_obj_t *container = lv_obj_create(ui_TempControlScreen);
    lv_obj_set_size(container, 720, 320);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_bg_color(container, lv_color_hex(0x956207), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(container, 200, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_radius(container, 10, LV_PART_MAIN);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(container, LV_DIR_VER);
    lv_obj_set_style_pad_all(container, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_row(container, 5, LV_PART_MAIN);

    // Create all setting rows
    ui_TargetTempSpinbox = create_setting_row(
        container, "Target Temp (°C):",
        0, 1500, (int32_t)(g_TempSettings.targetTemp * 10), 4);

    ui_KpSpinbox = create_setting_row(
        container, "Kp (P Gain):",
        0, 10000, (int32_t)(g_TempSettings.kp * 100), 5);

    ui_KiSpinbox = create_setting_row(
        container, "Ki (I Gain):",
        0, 10000, (int32_t)(g_TempSettings.ki * 1000), 5);

    ui_KdSpinbox = create_setting_row(
        container, "Kd (D Gain):",
        0, 10000, (int32_t)(g_TempSettings.kd * 100), 5);

    ui_RampUpSpinbox = create_setting_row(
        container, "Ramp Up (°C/min):",
        0, 500, (int32_t)(g_TempSettings.rampUp * 10), 4);

    ui_RampDownSpinbox = create_setting_row(
        container, "Ramp Down (°C/min):",
        -500, 0, (int32_t)(g_TempSettings.rampDown * 10), 4);

    ui_CrossoverSpinbox = create_setting_row(
        container, "Crossover (°C):",
        0, 500, (int32_t)(g_TempSettings.crossover * 10), 4);

    ui_IntegratorSpinbox = create_setting_row(
        container, "Integrator Limit:",
        0, 5000, (int32_t)(g_TempSettings.integratorLimit * 10), 5);

    // Save Button
    ui_SaveSettingsButton = lv_button_create(ui_TempControlScreen);
    lv_obj_set_size(ui_SaveSettingsButton, 180, 50);
    lv_obj_align(ui_SaveSettingsButton, LV_ALIGN_BOTTOM_MID, -95, -15);
    lv_obj_set_style_bg_color(ui_SaveSettingsButton, lv_color_hex(0x28A745), LV_PART_MAIN);
    lv_obj_set_style_radius(ui_SaveSettingsButton, 8, LV_PART_MAIN);

    lv_obj_t *ui_SaveLabel = lv_label_create(ui_SaveSettingsButton);
    lv_label_set_text(ui_SaveLabel, "Save");
    lv_obj_set_style_text_font(ui_SaveLabel, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_SaveLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(ui_SaveLabel);

    // Cancel Button
    ui_CancelButton = lv_button_create(ui_TempControlScreen);
    lv_obj_set_size(ui_CancelButton, 180, 50);
    lv_obj_align(ui_CancelButton, LV_ALIGN_BOTTOM_MID, 95, -15);
    lv_obj_set_style_bg_color(ui_CancelButton, lv_color_hex(0xDC3545), LV_PART_MAIN);
    lv_obj_set_style_radius(ui_CancelButton, 8, LV_PART_MAIN);

    lv_obj_t *ui_CancelLabel = lv_label_create(ui_CancelButton);
    lv_label_set_text(ui_CancelLabel, "Cancel");
    lv_obj_set_style_text_font(ui_CancelLabel, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_CancelLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(ui_CancelLabel);

    // Status Label (between buttons)
    ui_SettingsStatusLabel = lv_label_create(ui_TempControlScreen);
    lv_label_set_text(ui_SettingsStatusLabel, "");
    // lv_color_hex(0x0000FF)     // Pure blue (bright)
    //     lv_color_hex(0x007BFF) // Bootstrap blue (nice medium blue)
    //     lv_color_hex(0x2196F3) // Material Design blue
    //     lv_color_hex(0x1E90FF) // Dodger blue
    //     lv_color_hex(0x4169E1) // Royal blue
    //     lv_color_hex(0x000080) // Navy blue (dark)
    //     lv_color_hex(0x5B9BD5) // Soft blue
    lv_obj_set_style_text_color(ui_SettingsStatusLabel, lv_color_hex(0x000080), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_SettingsStatusLabel, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(ui_SettingsStatusLabel, LV_ALIGN_TOP_MID, 0, 175);

    // Add event callbacks
    lv_obj_add_event_cb(ui_TargetTempSpinbox, ui_event_TargetTempSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_KpSpinbox, ui_event_KpSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_KiSpinbox, ui_event_KiSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_KdSpinbox, ui_event_KdSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_RampUpSpinbox, ui_event_RampUpSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_RampDownSpinbox, ui_event_RampDownSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_CrossoverSpinbox, ui_event_CrossoverSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_IntegratorSpinbox, ui_event_IntegratorSpinbox, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_SaveSettingsButton, ui_event_SaveSettingsButton, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_CancelButton, ui_event_CancelButton, LV_EVENT_ALL, nullptr);

    // Create a keyboard (initially hidden)
    lv_obj_t *keyboard = lv_keyboard_create(ui_TempControlScreen);
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Start hidden
    lv_obj_set_size(keyboard, 500, LV_VER_RES);
    lv_obj_align(keyboard, LV_ALIGN_LEFT_MID, 0, 0);

    // Keyboard event handler to show/hide when spinbox is clicked
    auto spinbox_click_handler = [](lv_event_t *e)
    {
        lv_obj_t *spinbox = (lv_obj_t *)lv_event_get_target(e);
        lv_obj_t *keyboard = (lv_obj_t *)lv_event_get_user_data(e);

        if (lv_event_get_code(e) == LV_EVENT_CLICKED)
        {
            // Show keyboard and associate it with this spinbox
            lv_keyboard_set_textarea(keyboard, spinbox);
            lv_obj_remove_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        }
    };

    // Add click events to all spinboxes to show keyboard
    lv_obj_add_event_cb(ui_TargetTempSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_KpSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_KiSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_KdSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_RampUpSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_RampDownSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_CrossoverSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);
    lv_obj_add_event_cb(ui_IntegratorSpinbox, spinbox_click_handler, LV_EVENT_CLICKED, keyboard);

    // Keyboard close button handler
    lv_obj_add_event_cb(keyboard, [](lv_event_t *e)
                        {
    if (lv_event_get_code(e) == LV_EVENT_READY || 
        lv_event_get_code(e) == LV_EVENT_CANCEL) {
        lv_obj_t *keyboard = (lv_obj_t *)lv_event_get_target(e);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    } }, LV_EVENT_ALL, nullptr);

    // Global labels (if needed)
    ui_GlobalLabels::initNetworkStatus(ui_TempControlScreen);
    ui_GlobalLabels::initUserSelectionLabel(ui_TempControlScreen);

    // Load screen
    lv_scr_load(ui_TempControlScreen);
}

void ui_TempControl_screen_destroy(void)
{
    if (ui_TempControlScreen)
    {
        lv_obj_clean(ui_TempControlScreen);
    }

    ui_TempControlTitle = nullptr;
    ui_TargetTempLabel = nullptr;
    ui_TargetTempSpinbox = nullptr;
    ui_KpLabel = nullptr;
    ui_KpSpinbox = nullptr;
    ui_KiLabel = nullptr;
    ui_KiSpinbox = nullptr;
    ui_KdLabel = nullptr;
    ui_KdSpinbox = nullptr;
    ui_RampUpLabel = nullptr;
    ui_RampUpSpinbox = nullptr;
    ui_RampDownLabel = nullptr;
    ui_RampDownSpinbox = nullptr;
    ui_CrossoverLabel = nullptr;
    ui_CrossoverSpinbox = nullptr;
    ui_IntegratorLabel = nullptr;
    ui_IntegratorSpinbox = nullptr;
    ui_SaveSettingsButton = nullptr;
    ui_CancelButton = nullptr;
    ui_SettingsStatusLabel = nullptr;

    uic_TempControlTitle = nullptr;
    uic_SaveSettingsButton = nullptr;
    uic_CancelButton = nullptr;
    uic_SettingsStatusLabel = nullptr;
}