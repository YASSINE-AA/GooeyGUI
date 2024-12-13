#ifndef GOOEY_EVENT_H
#define GOOEY_EVENT_H

typedef enum {
    GOOEY_EVENT_CLICK,
    GOOEY_EVENT_KEY_PRESS,
    GOOEY_EVENT_MOUSE_MOVE,
    GOOEY_EVENT_WINDOW_CLOSE,
    GOOEY_EVENT_EXPOSE,
    GOOEY_EVENT_RESIZE
} GooeyEventType;

typedef struct {
    GooeyEventType type;
    union {
        struct {
            int x, y;
        } click; 

        struct {
            unsigned int state;
            int keycode; 
        } key_press; 

        struct {
            int x, y;
        } mouse_move; 

    } data;
} GooeyEvent;

#endif