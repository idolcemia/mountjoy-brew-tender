#ifndef UI_USERSELECTION_H
#define UI_USERSELECTION_H

#ifdef __cplusplus
extern "C"
{
#endif

    // SCREEN: ui_UserSelectionScreen
    extern void ui_UserSelection_screen_init(void);
    extern void ui_UserSelectionScreenUpdate();
    extern void ui_UserSelection_screen_destroy(void);
    extern lv_obj_t *ui_UserSelectionScreen;
    extern void ui_event_user_dropdown(lv_event_t *e);
    extern lv_obj_t *ui_UserDropdown;
    extern lv_obj_t *ui_BSLogo;
    extern lv_obj_t *ui_SelectUserLabel;
    extern void ui_event_ConfirmUserButton(lv_event_t *e);
    extern lv_obj_t *ui_ConfirmUserButton;
    extern lv_obj_t *ui_UserDisplayLabel;
    extern lv_obj_t *ui_ConfirmLabel1;
    // CUSTOM VARIABLES
    extern lv_obj_t *uic_BSLogo;
    extern lv_obj_t *uic_SelectUserLabel;
    extern lv_obj_t *uic_ConfirmUserButton;
    extern lv_obj_t *uic_UserDisplayLabel;
    extern lv_obj_t *uic_ConfirmLabel1;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
