#include "main.h"

static void init() {
  main_menu_create();
  main_menu_show();
}

static void deinit() {
  main_menu_destroy();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}