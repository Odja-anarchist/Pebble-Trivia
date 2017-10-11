#pragma once

#include <pebble.h>

struct question {
  char *difficulty;
  char *question;
  char *answer1;
  char *answer2;
  char *answer3;
  char *answer4;
  int correctAnswer;
};

GRect get_window_bounds(Window *window);