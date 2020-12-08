#include "config.h"
#include <TTGO.h>

#include "template_app.h"
#include "template_ui.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/app.h"
#include "gui/widget.h"

#include "hardware/json_psram_allocator.h"

template_config_t template_config;

uint32_t template_ui_tile_num;
// app icon
icon_t *template_app = NULL;
icon_t *template_widget = NULL;

// declare you images or fonts you need
LV_IMG_DECLARE(gps_app_64px);
LV_IMG_DECLARE(info_1_16px);

void template_setup(void)
{
    template_load_config();
    template_ui_tile_num = mainbar_add_app_tile(1, 2, TEMPLATE_APP_NAME);
    // register app icon on the app tile
    // set your own icon and register her callback to activate by an click
    // remember, an app icon must have an size of 64x64 pixel with an alpha channel
    // use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha" to get fancy images
    // the resulting c-file can put in /app/gpss/images/ and declare it like LV_IMG_DECLARE( your_icon );
    template_app = app_register(TEMPLATE_APP_NAME, &gps_app_64px, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            tamplate_show_indicator(false);
            mainbar_jump_to_tilenumber(template_ui_tile_num, LV_ANIM_OFF);
            break;
        }
    });

    //tamplate_show_indicator();
    if (template_config.widget)
        template_add_widget();
    template_ui_setup(template_ui_tile_num);
}

void tamplate_show_indicator(bool show)
{
    show ? app_set_indicator(template_app, ICON_INDICATOR_OK) : app_hide_indicator(template_app);
}

void template_set_widget_label(const char *label)
{
    widget_set_label(template_widget, label);
}
void template_add_widget(void)
{
    template_widget = widget_register(TEMPLATE_APP_NAME, &gps_app_64px, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            widget_hide_indicator(template_widget);
            mainbar_jump_to_tilenumber(template_ui_tile_num, LV_ANIM_OFF);
            break;
        }
    });
    widget_set_indicator(template_widget, ICON_INDICATOR_UPDATE);
}

void template_remove_widget(void)
{
    template_widget = widget_remove(template_widget);
}

template_config_t *template_get_config(void)
{
    return (&template_config);
}

void template_load_config(void)
{
    fs::File file = SPIFFS.open(TEMPLATE_JSON_CONFIG_FILE, FILE_READ);
    if (!file)
        log_e("Can't open file: %s!", TEMPLATE_JSON_CONFIG_FILE);
    else
    {
        int filesize = file.size();
        SpiRamJsonDocument doc(filesize * 4);
        DeserializationError error = deserializeJson(doc, file);
        if (error)
            log_e("update check deserializeJson() failed: %s", error.c_str());
        else
        {
            // strlcpy(template_config.copyright, doc["copyright"], sizeof(template_config.copyright));
            template_config.widget = doc["widget"] | false;
        }
        doc.clear();
    }
    file.close();
}

void template_save_config(void)
{
    fs::File file = SPIFFS.open(TEMPLATE_JSON_CONFIG_FILE, FILE_WRITE);
    if (!file)
        log_e("Can't open file: %s!", TEMPLATE_JSON_CONFIG_FILE);
    else
    {
        SpiRamJsonDocument doc(1000);
        doc["widget"] = template_config.widget;
        // doc["copyright"] = template_config.copyright;
        if (serializeJsonPretty(doc, file) == 0)
            log_e("Failed to write config file");
        doc.clear();
    }
    file.close();
}
