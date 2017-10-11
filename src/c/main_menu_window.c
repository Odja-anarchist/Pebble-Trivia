#include "main_menu_window.h"

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 3

static Window *s_main_window;
static SimpleMenuLayer *s_main_menu_layer;
static StatusBarLayer *s_main_menu_status_bar;
static ActionBarLayer *s_main_menu_action_bar;
static SimpleMenuSection s_main_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_main_menu_items[NUM_MENU_ITEMS];

static GBitmap *s_main_menu_arrow_up;
static GBitmap *s_main_menu_arrow_down;
static GBitmap *s_main_menu_arrow_right;

static void main_load_status_bar(Window *window) {
  s_main_menu_status_bar = status_bar_layer_create();
  layer_add_child(window_get_root_layer(window), status_bar_layer_get_layer(s_main_menu_status_bar));
}

static void main_menu_select_callback(ClickRecognizerRef recognizer, void* context) {}
static void main_menu_up_callback(ClickRecognizerRef recognizer, void* context) {}
static void main_menu_down_callback(ClickRecognizerRef recognizer, void* context) {}

static void main_menu_ccp(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, main_menu_select_callback);
  window_single_click_subscribe(BUTTON_ID_UP, main_menu_up_callback);
  window_single_click_subscribe(BUTTON_ID_DOWN, main_menu_down_callback);
}

static void main_load_action_bar(Window *window) {
  s_main_menu_action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_main_menu_action_bar, window);
  action_bar_layer_set_click_config_provider(s_main_menu_action_bar, main_menu_ccp);
  action_bar_layer_set_icon_animated(s_main_menu_action_bar, BUTTON_ID_SELECT, s_main_menu_arrow_right, true);
  action_bar_layer_set_icon_animated(s_main_menu_action_bar, BUTTON_ID_UP, s_main_menu_arrow_up, true);
  action_bar_layer_set_icon_animated(s_main_menu_action_bar, BUTTON_ID_DOWN, s_main_menu_arrow_down, true);
}

static void main_load_menu(Window *window) {}

static void main_load_bitmaps() {
  s_main_menu_arrow_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_UP);
  s_main_menu_arrow_right = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_RIGHT);
  s_main_menu_arrow_down = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_ARROW_DOWN);
}

static void main_unload_bitmaps() {
  gbitmap_destroy(s_main_menu_arrow_up);
  gbitmap_destroy(s_main_menu_arrow_right);
  gbitmap_destroy(s_main_menu_arrow_down);
}

static void main_window_load(Window *window) {
  main_load_bitmaps();
  main_load_status_bar(window);
  main_load_action_bar(window);
}

static void main_window_unload(Window *window) {
  status_bar_layer_destroy(s_main_menu_status_bar);
  action_bar_layer_destroy(s_main_menu_action_bar);
  main_unload_bitmaps();
}

void main_menu_create() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
}

void main_menu_show() {
  window_stack_push(s_main_window, false);
}

void main_menu_destroy() {
  window_destroy(s_main_window);
}