#ifndef THEME_PARSER_H
#define THEME_PARSER_H

#include <cjson/cJSON.h>
#include <stdio.h> 
#include <stdlib.h>

typedef struct
{
    unsigned long base; /* Base window background color */

    unsigned long neutral; /* Neutral color for text */

    unsigned long widget_base; /* Base widget color */

    unsigned long primary; /* Primary color */

} GooeyTheme;

GooeyTheme parser_load_theme_from_file(const char* filePath);

#endif