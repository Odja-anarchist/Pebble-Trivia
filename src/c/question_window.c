#include "question_window.h"

static Window *s_question_window;
static StatusBarLayer *s_question_status_bar;
static ActionBarLayer *s_question_action_bar;
static ScrollLayer *s_question_scroll_layer;
static MenuLayer *s_question_answers_menu;
static TextLayer *s_question_question_text;

static GBitmap *s_menu_icon_up;
static GBitmap *s_menu_icon_right;
static GBitmap *s_menu_icon_down;

static uint16_t num_answers = 2;
static int16_t scroll_layer_max_offset = 0;
static AppTimer *action_bar_long_press_timer;

static void question_load_status_bar(Window *window) {
  s_question_status_bar = status_bar_layer_create();
  layer_add_child(window_get_root_layer(window), status_bar_layer_get_layer(s_question_status_bar));
}

// Action Bar

static void question_action_bar_select_callback(ClickRecognizerRef recognizer, void* context) {}
static void question_action_bar_long_select_callback(ClickRecognizerRef recognizer, void* context) {}

static void question_clear_action_bar_long_press_timer() {
  if(action_bar_long_press_timer != NULL) {
    app_timer_cancel(action_bar_long_press_timer);
    action_bar_long_press_timer = NULL;
  }
}

static void question_pan_scroll_up() {
  GPoint offset = scroll_layer_get_content_offset(s_question_scroll_layer);
  offset.y = offset.y + SCROLL_LAYER_MOVE_STEP;
  if (offset.y > 0) {
    offset.y = 0;
  }
  scroll_layer_set_content_offset(s_question_scroll_layer, offset, true);
}

static void question_pan_scroll_down(){
  GPoint offset = scroll_layer_get_content_offset(s_question_scroll_layer);
  offset.y = offset.y - SCROLL_LAYER_MOVE_STEP;
  if (offset.y < scroll_layer_max_offset) {
    offset.y = scroll_layer_max_offset;
  }
  scroll_layer_set_content_offset(s_question_scroll_layer, offset, true);
}

static void question_action_bar_up_callback(ClickRecognizerRef recognizer, void* context) {
  question_clear_action_bar_long_press_timer();
  question_pan_scroll_up();
}
static void question_action_bar_down_callback(ClickRecognizerRef recognizer, void* context) {
  question_clear_action_bar_long_press_timer();
  question_pan_scroll_down();
}

static void question_action_bar_up_timer_listener() {
  question_clear_action_bar_long_press_timer();
  question_pan_scroll_up();
  action_bar_long_press_timer = app_timer_register(LONG_PRESS_TIMER_PERIOD, question_action_bar_up_timer_listener, NULL);
}
static void question_action_bar_down_timer_listener() {
  question_clear_action_bar_long_press_timer();
  question_pan_scroll_down();
  action_bar_long_press_timer = app_timer_register(LONG_PRESS_TIMER_PERIOD, question_action_bar_down_timer_listener, NULL);
}

static void question_action_bar_long_up_callback(ClickRecognizerRef recognizer, void* context) {
  question_clear_action_bar_long_press_timer();
}
static void question_action_bar_up_long_down_callback(ClickRecognizerRef recognizer, void* context) {
  question_action_bar_up_timer_listener();
}
static void question_action_bar_down_long_down_callback(ClickRecognizerRef recognizer, void* context) {
  question_action_bar_down_timer_listener();
}

static void question_click_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, question_action_bar_up_callback);
  window_long_click_subscribe(BUTTON_ID_UP, DIRECTION_LONG_PRESS_THRESHOLD, question_action_bar_up_long_down_callback, question_action_bar_long_up_callback);
  
  window_single_click_subscribe(BUTTON_ID_SELECT, question_action_bar_select_callback);
  window_long_click_subscribe(BUTTON_ID_SELECT, SELECT_LONG_PRESS_THRESHOLD, question_action_bar_long_select_callback, NULL);
  
  window_single_click_subscribe(BUTTON_ID_DOWN, question_action_bar_down_callback);
  window_long_click_subscribe(BUTTON_ID_DOWN, DIRECTION_LONG_PRESS_THRESHOLD, question_action_bar_down_long_down_callback, question_action_bar_long_up_callback);
}

static void question_load_action_bar(Window *window) {
  s_question_action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_question_action_bar, window);
  action_bar_layer_set_click_config_provider(s_question_action_bar, question_click_provider);
  action_bar_layer_set_icon_animated(s_question_action_bar, BUTTON_ID_UP, s_menu_icon_up, true);
  action_bar_layer_set_icon_animated(s_question_action_bar, BUTTON_ID_SELECT, s_menu_icon_right, true);
  action_bar_layer_set_icon_animated(s_question_action_bar, BUTTON_ID_DOWN, s_menu_icon_down, true);
}

// Menu callbacks

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return num_answers;
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  if(cell_index->row == 0) {
    menu_cell_basic_draw(ctx, cell_layer, "True", NULL, NULL);
  } else {
    menu_cell_basic_draw(ctx, cell_layer, "False", NULL, NULL);
  }
}

static void question_load_menu(Window *window) {
  
}


static void question_load_bitmaps() {
  s_menu_icon_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_UP);
  s_menu_icon_right = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_RIGHT);
  s_menu_icon_down = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_DOWN);
}

static void question_unload_bitmaps() {
  gbitmap_destroy(s_menu_icon_up);
  gbitmap_destroy(s_menu_icon_right);
  gbitmap_destroy(s_menu_icon_down);
}

static void question_window_load(Window *window) {
  question_load_bitmaps();
  question_load_status_bar(window);
  question_load_action_bar(window);
}
static void question_window_unload(Window *window) {
  question_unload_bitmaps();
  status_bar_layer_destroy(s_question_status_bar);
  action_bar_layer_destroy(s_question_action_bar);
  menu_layer_destroy(s_question_answers_menu);
  text_layer_destroy(s_question_question_text);
  scroll_layer_destroy(s_question_scroll_layer);
}

void question_window_create() {
  s_question_window = window_create();
  window_set_window_handlers(s_question_window, (WindowHandlers){
    .load = question_window_load,
    .unload = question_window_unload,
  });
}

void question_window_show() {
  window_stack_push(s_question_window, false);
}

void question_winow_destroy() {
  window_destroy(s_question_window);
}