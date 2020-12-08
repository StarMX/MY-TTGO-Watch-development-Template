#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include <TTGO.h>
#define TEMPLATE_APP_NAME "template"
#define TEMPLATE_JSON_CONFIG_FILE "/template.json"

typedef struct
{
    // char copyright[8] = "";
    bool widget = false;
} template_config_t;

void template_setup(void);

void tamplate_show_indicator(bool show = true);
void template_add_widget(void);
void template_remove_widget(void);
void template_set_widget_label(const char *label);

template_config_t *template_get_config(void);
void template_save_config(void);
void template_load_config(void);


#endif