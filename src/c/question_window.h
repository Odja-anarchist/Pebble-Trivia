#pragma once

#include <pebble.h>

#define LONG_PRESS_TIMER_PERIOD 250
#define SELECT_LONG_PRESS_THRESHOLD 700
#define DIRECTION_LONG_PRESS_THRESHOLD 400
#define SCROLL_LAYER_MOVE_STEP 32

void question_window_create();
void question_window_show();
void question_winow_destroy();
