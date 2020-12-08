#include "config.h"
#include <TTGO.h>

#include "template_app.h"
#include "template_ui.h"
#include "template.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

#include "hardware/wifictl.h"
#include "hardware/powermgm.h"

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
uint32_t template_ui_main_tile_num;
uint32_t template_ui_settings_tile_num;
void template_ui_main(uint32_t tile_num);
void template_ui_settings(uint32_t tile_num);
void template_ui_setup(uint32_t tile_num)
{
    template_ui_main(tile_num);
    template_ui_settings(++tile_num);
}

//main ui start
lv_obj_t *template_ui_tile = NULL;
lv_style_t template_ui_main_style;
lv_task_t *_templateui_main_task;

LV_IMG_DECLARE(exit_32px);
void template_main_task(lv_task_t *task);
void template_ui_main(uint32_t tile_num)
{
    template_ui_main_tile_num = tile_num;
    template_ui_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&template_ui_main_style, mainbar_get_style());


    //exit button
    lv_obj_t *exit_btn = lv_imgbtn_create(template_ui_tile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &template_ui_main_style);
    lv_obj_align(exit_btn, template_ui_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_event_cb(exit_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            mainbar_jump_to_maintile(LV_ANIM_OFF);
            break;
        }
    });

    //setup button
    lv_obj_t *setup_btn = lv_imgbtn_create(template_ui_tile, NULL);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_PRESSED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_PRESSED, &setup_32px);
    lv_obj_add_style(setup_btn, LV_IMGBTN_PART_MAIN, &template_ui_main_style);
    lv_obj_align(setup_btn, template_ui_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_event_cb(setup_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            mainbar_jump_to_tilenumber(template_ui_settings_tile_num, LV_ANIM_ON);
            break;
        }
    });

    /*lvgl ui start */


    /*lvgl ui end */
    mainbar_add_tile_activate_cb(tile_num, [](void) {
        _templateui_main_task = lv_task_create(template_main_task, 10, LV_TASK_PRIO_HIGH, NULL);
    });
    mainbar_add_tile_hibernate_cb(tile_num, [](void) {
        lv_task_del(_templateui_main_task);
    });


    //wifi connect or disconnect do work
    wifictl_register_cb(
        WIFICTL_CONNECT_IP | WIFICTL_OFF_REQUEST | WIFICTL_OFF | WIFICTL_DISCONNECT, [](EventBits_t event, void *arg) {
            switch (event)
            {
            case WIFICTL_CONNECT_IP:
            {
            }
            break;
            case WIFICTL_OFF_REQUEST:
            case WIFICTL_OFF:
            case WIFICTL_DISCONNECT:

                break;
            }
            return (true);
        },
        TEMPLATE_APP_NAME);
    //power wakeup or silence wakup do work
    powermgm_register_loop_cb(
        POWERMGM_WAKEUP | POWERMGM_SILENCE_WAKEUP, [](EventBits_t event, void *arg) {
            switch (event)
            {
            case POWERMGM_SILENCE_WAKEUP:

                break;
            case POWERMGM_WAKEUP:

                break;
            }
            return (true);
        },
        TEMPLATE_APP_NAME);
}

void template_main_task(lv_task_t *task)
{
}
//main ui end




// settings ui start
lv_style_t template_ui_setup_style;
lv_obj_t *template_ui_settings_widget_switch = NULL;
void template_ui_settings(uint32_t tile_num)
{
    template_ui_settings_tile_num = tile_num;
    template_ui_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&template_ui_setup_style, mainbar_get_style());

    lv_style_set_bg_color(&template_ui_setup_style, LV_OBJ_PART_MAIN, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&template_ui_setup_style, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width(&template_ui_setup_style, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style(template_ui_tile, LV_OBJ_PART_MAIN, &template_ui_setup_style);

    lv_obj_t *exit_cont = lv_obj_create(template_ui_tile, NULL);
    lv_obj_set_size(exit_cont, lv_disp_get_hor_res(NULL), 40);
    lv_obj_add_style(exit_cont, LV_OBJ_PART_MAIN, &template_ui_setup_style);
    lv_obj_align(exit_cont, template_ui_tile, LV_ALIGN_IN_TOP_MID, 0, 10);

    lv_obj_t *exit_btn = lv_imgbtn_create(exit_cont, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &template_ui_setup_style);
    lv_obj_align(exit_btn, exit_cont, LV_ALIGN_IN_TOP_LEFT, 10, 0);
    lv_obj_set_event_cb(exit_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            mainbar_jump_to_tilenumber(template_ui_main_tile_num, LV_ANIM_ON);
            break;
        }
    });

    lv_obj_t *exit_label = lv_label_create(exit_cont, NULL);
    lv_obj_add_style(exit_label, LV_OBJ_PART_MAIN, &template_ui_setup_style);
    lv_label_set_text(exit_label, "Settings");
    lv_obj_align(exit_label, exit_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    lv_obj_t *template_ui_settings_widget_switch_cont = lv_obj_create(template_ui_tile, NULL);
    lv_obj_set_size(template_ui_settings_widget_switch_cont, lv_disp_get_hor_res(NULL), 40);
    lv_obj_add_style(template_ui_settings_widget_switch_cont, LV_OBJ_PART_MAIN, &template_ui_setup_style);
    lv_obj_align(template_ui_settings_widget_switch_cont, exit_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    template_ui_settings_widget_switch = lv_switch_create(template_ui_settings_widget_switch_cont, NULL);
    lv_obj_add_protect(template_ui_settings_widget_switch, LV_PROTECT_CLICK_FOCUS);
    lv_obj_add_style(template_ui_settings_widget_switch, LV_SWITCH_PART_INDIC, mainbar_get_switch_style());
    lv_switch_off(template_ui_settings_widget_switch, LV_ANIM_ON);
    lv_obj_align(template_ui_settings_widget_switch, template_ui_settings_widget_switch_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    lv_obj_set_event_cb(template_ui_settings_widget_switch, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_VALUE_CHANGED):
            template_config_t *config = template_get_config();
            config->widget = lv_switch_get_state(obj);
            template_save_config();
            config->widget ? template_add_widget() : template_remove_widget();
            Serial.printf("switch value = %d\r\n", lv_switch_get_state(obj));
            break;
        }
    });
    lv_obj_t *gps_app_widget_switch_label = lv_label_create(template_ui_settings_widget_switch_cont, NULL);
    lv_obj_add_style(gps_app_widget_switch_label, LV_OBJ_PART_MAIN, &template_ui_setup_style);
    lv_label_set_text(gps_app_widget_switch_label, "Widget");
    lv_obj_align(gps_app_widget_switch_label, template_ui_settings_widget_switch_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);

    template_config_t *config = template_get_config();
    if (config->widget)
        lv_switch_on(template_ui_settings_widget_switch, LV_ANIM_OFF);
    else
        lv_switch_off(template_ui_settings_widget_switch, LV_ANIM_OFF);
}
// settings ui end