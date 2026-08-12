#include "ui_GlobalButtons.h"

namespace ui_GlobalButtons
{
    lv_obj_t *homeButton = nullptr;

    void initGlobalButtons(lv_obj_t *parent)
    {
        logger.info("[ui_GlobalButtons] initGlobalButtons called");

        if (!parent)
        {
            logger.info("[ui_GlobalButtons] No parent passed, using lv_layer_top()");
            parent = lv_layer_top();
        }
        else
        {
            logger.info("[ui_GlobalButtons] Using provided parent: " + String((uintptr_t)parent, HEX));
        }

        if (!homeButton)
        {
            logger.info("[ui_GlobalButtons] Creating homeButton");
            homeButton = lv_btn_create(parent);
            lv_obj_set_size(homeButton, 80, 40);
            lv_obj_align(homeButton, LV_ALIGN_BOTTOM_RIGHT, -10, -20);
            lv_obj_set_style_bg_color(homeButton, lv_color_hex(0xF0A31E), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(homeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_t *lbl = lv_label_create(homeButton);
            lv_label_set_text(lbl, "Home");
            lv_obj_center(lbl);

            lv_obj_add_event_cb(
                homeButton,
                [](lv_event_t *e)
                {
                    lv_obj_t *currentScreen = lv_scr_act();
                    _cache(currentScreen);

                    logger.info("[ui_GlobalButtons] Home button pressed");
                    menuManager.queueMenu(PasteurizerMenu::MENU_MENU_SELECTION);
                },
                LV_EVENT_CLICKED, nullptr);
        }
        else
        {
            logger.info("[ui_GlobalButtons] homeButton already exists");
            if (parent != lv_obj_get_parent(homeButton))
            {
                logger.info("[ui_GlobalButtons] Re-parenting homeButton");
                lv_obj_set_parent(homeButton, parent);
            }
        }

        updateGlobalButtons(parent);
    }

    void updateGlobalButtons(lv_obj_t *parent)
    {
        if (!homeButton)
        {
            logger.warn("[ui_GlobalButtons] updateGlobalButtons called but homeButton is nullptr!");
            return;
        }

        if (parent)
        {
            lv_obj_set_parent(homeButton, parent);
            logger.info("[ui_GlobalButtons] Set parent for homeButton: " + String((uintptr_t)parent, HEX));
        }

        const String currentMenuName = menuManager.getCurrentMenuName();
        logger.info("[ui_GlobalButtons] Current menu: " + currentMenuName);

        if (currentMenuName.isEmpty())
        {
            logger.warn("[ui_GlobalButtons] Current menu is empty, skipping visibility update");
            return;
        }

        if (currentMenuName == PasteurizerMenu::MENU_MENU_SELECTION)
        {
            lv_obj_add_flag(homeButton, LV_OBJ_FLAG_HIDDEN);
            logger.info("[ui_GlobalButtons] Home button hidden on menu: " + currentMenuName);
        }
        else
        {
            lv_obj_clear_flag(homeButton, LV_OBJ_FLAG_HIDDEN);
            logger.info("[ui_GlobalButtons] Home button visible on menu: " + currentMenuName);
        }

        lv_obj_align(homeButton, LV_ALIGN_BOTTOM_RIGHT, -10, -20);
        logger.info("[ui_GlobalButtons] Home button aligned bottom-right");
    }

    void destroyGlobalButtons()
    {
        if (homeButton)
        {
            lv_obj_del(homeButton);
            homeButton = nullptr;
            logger.info("[ui_GlobalButtons] homeButton destroyed");
        }
    }

    void _cache(lv_obj_t *currentScreen)
    {
        const String currentMenuName = menuManager.getCurrentMenuName();
        logger.info("[ui_GlobalButtons] Caching screen for menu: " + currentMenuName);

        if (currentMenuName.isEmpty() || !currentScreen)
        {
            logger.warn("[ui_GlobalButtons] Cannot cache screen, menu name empty or currentScreen nullptr");
            return;
        }

        if (!menuManager.setCachedScreen(currentMenuName, currentScreen))
        {
            logger.warn("[ui_GlobalButtons] Failed to cache screen for menu: " + currentMenuName);
        }
        else
        {
            logger.info("[ui_GlobalButtons] Cached screen for menu: " + currentMenuName);
        }
    }
}
