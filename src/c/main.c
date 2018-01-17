#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_hour_layer = NULL;
static TextLayer *s_link_layer = NULL;
static TextLayer *s_minute_layer = NULL;
static TextLayer *s_date_text_layer = NULL;
static TextLayer *s_date_layer = NULL;

static char *hour_text[] = {
  "doișpe",
  "unu",
  "două",
  "trei",
  "patru",
  "cinci",
  "șase",
  "șepte",
  "opt",
  "nouă",
  "zece",
  "unșpe",
};

/*
  0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55
*/

static char *link_text[] = {
  "",
  "și",
  "și",
  "și-un",
  "",
  "și",
  "",
  "fără",
  "fără",
  "făr-un",
  "fără",
  "fără",
};

static char *minute_text[] = {
  "",
  "cinci",
  "zece",
  "sfert",
  "douăzeci",
  "două-ș-cinci",
  "jumate",
  "două-ș-cinci",
  "douăzeci",
  "sfert",
  "zece",
  "cinci",
};

static char *date_text[] = {
  "duminecă",
  "luni",
  "marți",
  "miercuri",
  "joi",
  "vineri",
  "sâmbătă",
};

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  static char hour_display[10];
  static char link_display[10];
  static char minute_display[20];
  static char date_text_display[20];
  static char date_display[20];
  
  int hour = tick_time->tm_hour % 12;
  int seconds = tick_time->tm_min * 60 + tick_time->tm_sec;
  int seconds2 = seconds, diff2 = seconds2 % 60;
  
  // calculations of 5 minutes precision
  int diff = seconds % 300;
  if (diff < 150) { 
    seconds -= diff;
  } else {
    seconds += 300 - diff;
  }
  if (seconds > 1800) {
    hour = (hour + 1) % 12;
  }
  if (seconds == 3600) {
    seconds = 0;
  }
  
  // calculations of 1 minute precision (now only for deciding whether to display "fix")
  if (diff2 < 30) { 
    seconds2 -= diff2;
  } else {
    seconds2 += 60 - diff2;
  }
  if (seconds2 == 3600) {
    seconds2 = 0;
  }
  
  int minute = seconds / 300;
  
  // check to see if the clock is H o'clock sharp
  if (seconds2 / 60 == 0) {
    // in that case we add "fix" 
    strncpy(minute_display, "fix", 20);
    strncpy(link_display, "", 10);
  } else {
    // otherwise we use the normal text which changes every 5 minutes
    strncpy(minute_display, minute_text[minute], 20);
    strncpy(link_display, link_text[minute], 10);
  }
  
  strncpy(hour_display, hour_text[hour], 10);
  strncpy(date_text_display, date_text[tick_time->tm_wday], 20);
  snprintf(date_display, 20, "%02d-%02d-%d", 
           tick_time->tm_mday, tick_time->tm_mon+1, tick_time->tm_year+1900);
  
  text_layer_set_text(s_hour_layer, hour_display);
  text_layer_set_text(s_link_layer, link_display);
  text_layer_set_text(s_minute_layer, minute_display);
  text_layer_set_text(s_date_text_layer, date_text_display);
  text_layer_set_text(s_date_layer, date_display);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_hour_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(8, 5), bounds.size.w, 32));
  s_link_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(40, 37), bounds.size.w, 32));
  s_minute_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(72, 69), bounds.size.w, 32));
  s_date_text_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(113, 110), bounds.size.w, 20));
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(133, 130), bounds.size.w, 20));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  text_layer_set_text(s_hour_layer, "Stai");
  text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_link_layer, GColorClear);
  text_layer_set_text_color(s_link_layer, GColorBlack);
  text_layer_set_text(s_link_layer, "un");
  text_layer_set_font(s_link_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_link_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_minute_layer, GColorClear);
  text_layer_set_text_color(s_minute_layer, GColorBlack);
  text_layer_set_text(s_minute_layer, "pic");
  text_layer_set_font(s_minute_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_minute_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_date_text_layer, GColorClear);
  text_layer_set_text_color(s_date_text_layer, GColorBlack);
  text_layer_set_text(s_date_text_layer, "");
  text_layer_set_font(s_date_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_text_layer, GTextAlignmentCenter);

  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_link_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_minute_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_text_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_link_layer);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_date_text_layer);
  text_layer_destroy(s_date_layer);
}

static void init(void) {
	s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
