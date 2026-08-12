#pragma once
#include "lvgl.h"
#include "Globals.h"
#include "ui/screens/ui_MenuSelectionScreen.h"

namespace ui_GlobalButtons
{
    /**
     * @brief Global Home button object
     */
    extern lv_obj_t *homeButton;

    /**
     * @brief Initialize global buttons. Must be called once during setup.
     * @param parent The parent LVGL object (typically lv_scr_act() or a top-level container)
     */
    void initGlobalButtons(lv_obj_t *parent = nullptr);

    /**
     * @brief Update global buttons' visibility, position, or parent.
     *        Should be called whenever a new screen loads.
     * @param parent Optional new parent LVGL object
     */
    void updateGlobalButtons(lv_obj_t *parent = nullptr);

    /**
     * @brief Destroy global buttons (for cleanup on shutdown)
     */
    void destroyGlobalButtons();

    /**
     * @brief Cache the current screen in the MenuManager
     * @param currentScreen LVGL screen object to cache
     */
    void _cache(lv_obj_t *currentScreen);
}
