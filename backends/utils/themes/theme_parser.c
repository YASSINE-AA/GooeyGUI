#include "theme_parser.h"

GooeyTheme parser_load_theme_from_file(const char *filePath)
{
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open json theme file.\n");
        exit(1);
    }

    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        exit(1);
    }

    int baseColor, widgetBaseColor, neutralColor, primaryColor;
    // Neutral color
    cJSON *neutral = cJSON_GetObjectItemCaseSensitive(json, "neutral");
    if (cJSON_IsString(neutral) && (neutral->valuestring != NULL))
    {
        neutralColor = (unsigned long)strtol(neutral->valuestring, NULL, 0);
    }

    cJSON *widget_base = cJSON_GetObjectItemCaseSensitive(json, "widget_base");
    if (cJSON_IsString(widget_base) && (widget_base->valuestring != NULL))
    {
        widgetBaseColor = (unsigned long)strtol(widget_base->valuestring, NULL, 0);
    }

    // Base color
    cJSON *base = cJSON_GetObjectItemCaseSensitive(json, "base");
    if (cJSON_IsString(base) && (base->valuestring != NULL))
    {
        baseColor = (unsigned long)strtol(base->valuestring, NULL, 0);
    }

    // Primary color
    cJSON *primary = cJSON_GetObjectItemCaseSensitive(json, "primary");
    if (cJSON_IsString(primary) && (primary->valuestring != NULL))
    {
        primaryColor = (unsigned long)strtol(primary->valuestring, NULL, 0);
    }

    cJSON_Delete(json);

    GooeyTheme theme = {.base = baseColor, .neutral = neutralColor, .widget_base = widgetBaseColor, .primary = primaryColor};

    return theme;
}
