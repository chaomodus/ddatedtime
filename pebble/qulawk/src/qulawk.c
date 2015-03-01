#include <pebble.h>
#include "qulawk-routine.h"
#include "ddate-routine.h"



static Window *window;
static TextLayer *s_bigtime_layer;
static TextLayer *s_bigtime_sm_layer;
static TextLayer *s_desctime_layer;
static TextLayer *s_gmtime_layer;
static TextLayer *s_localtime_layer;
static TextLayer *s_ddate_layer;
static TextLayer *s_ddate_day_layer;

static struct qulawk_t qlawk;
static struct ddate_t ddate;

static int32_t utcoffset = 0;
static time_t last_tz_update = 0;

static char s_bigtime_str[5];
static char s_bigtime_sm_str[6];
static char s_desctime_str[16];
static char s_gmtime_str[10];
static char s_localtime_str[10];
static char s_ddate_str[16];
static char s_ddate_day_str[16];

#define KEY_TIMEZONE_OFFSET 0

static void appmsg_in_received(DictionaryIterator *received, void *context) {
  Tuple *timezone_offset_tuple = dict_find(received, KEY_TIMEZONE_OFFSET);

  if (timezone_offset_tuple) {
    utcoffset = timezone_offset_tuple->value->int32;
    last_tz_update = time(NULL);
    persist_write_int(KEY_TIMEZONE_OFFSET, utcoffset);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_t now = time(NULL);
  struct tm *tm_now = localtime(&now);
  tm_now->tm_year += 1900;
  xlate_ddate(&ddate, tm_now->tm_yday, tm_now->tm_year, tm_now->tm_mon, tm_now->tm_mday);
  snprintf(s_localtime_str, 10, "%02d:%02d:%02d", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  now += utcoffset;
  tm_now = localtime(&now);
  snprintf(s_gmtime_str, 10, "%02d:%02d:%02d", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
  xlate_qulawk(&qlawk, tm_now, 0);
  snprintf(s_bigtime_str, 5, "%1d:%1d", qlawk.session, qlawk.period);
  snprintf(s_bigtime_sm_str, 6, "%02d.%02d",  qlawk.bit, qlawk.moment);
  snprintf(s_desctime_str, 16, "%s %s", sessionNames[qlawk.session-1], periodNames[qlawk.period-1]);
  if (ddate.sttibbs) {
    strcpy(s_ddate_str, "St. Tibb's Day");
    s_ddate_day_str[0] = 0;
  }  else {
    snprintf(s_ddate_str, 16, "%4d-%s-%2d", ddate.d_year, shortSeasonNames[ddate.season-1], ddate.day_of_season);
    strcpy(s_ddate_day_str, longDayNames[ddate.day_of_week-1]);
  }
  text_layer_set_text(s_bigtime_layer, s_bigtime_str);
  text_layer_set_text(s_bigtime_sm_layer, s_bigtime_sm_str);
  text_layer_set_text(s_desctime_layer, s_desctime_str);
  text_layer_set_text(s_gmtime_layer, s_gmtime_str);
  text_layer_set_text(s_localtime_layer, s_localtime_str);
  text_layer_set_text(s_ddate_layer, s_ddate_str);
  text_layer_set_text(s_ddate_day_layer, s_ddate_day_str);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(window, GColorBlack);

  // BIG session:period
  s_bigtime_layer = text_layer_create((GRect) { .origin = { 0, 35 }, .size = { bounds.size.w, 42 } });
  text_layer_set_font(s_bigtime_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_bigtime_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_bigtime_layer, GColorWhite);
  text_layer_set_background_color(s_bigtime_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_bigtime_layer));

  // session and period names
  s_desctime_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = { bounds.size.w, 24 }});
  text_layer_set_font(s_desctime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_desctime_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_desctime_layer, GColorWhite);
  text_layer_set_background_color(s_desctime_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_desctime_layer));

  // bit and moment
  s_bigtime_sm_layer = text_layer_create((GRect) { .origin = { 0, 70 }, .size = { bounds.size.w, 30 }});
  text_layer_set_font(s_bigtime_sm_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_bigtime_sm_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_bigtime_sm_layer, GColorWhite);
  text_layer_set_background_color(s_bigtime_sm_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_bigtime_sm_layer));

  // date
  s_ddate_layer = text_layer_create((GRect) { .origin = { 0, bounds.size.h - 30 }, .size = { bounds.size.w, 28}});
  text_layer_set_font(s_ddate_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_ddate_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_ddate_layer, GColorWhite);
  text_layer_set_background_color(s_ddate_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_ddate_layer));

  // dow
  s_ddate_day_layer = text_layer_create((GRect) { .origin = { 0, bounds.size.h - 50 }, .size = { bounds.size.w, 24}});
  text_layer_set_font(s_ddate_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_ddate_day_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_ddate_day_layer, GColorWhite);
  text_layer_set_background_color(s_ddate_day_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_ddate_day_layer));

  // tiny gmt time
  s_gmtime_layer = text_layer_create((GRect){.origin={0, 0}, .size={bounds.size.w / 2, 25}});
  text_layer_set_text_color(s_gmtime_layer, GColorWhite);
  text_layer_set_background_color(s_gmtime_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_gmtime_layer));

  // tiny local time
  s_localtime_layer = text_layer_create((GRect){.origin={bounds.size.w / 2, 0}, .size={bounds.size.w / 2, 25}});
  text_layer_set_text_color(s_localtime_layer, GColorWhite);
  text_layer_set_background_color(s_localtime_layer, GColorClear);
  text_layer_set_text_alignment(s_localtime_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_localtime_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_bigtime_layer);
  text_layer_destroy(s_gmtime_layer);
  text_layer_destroy(s_localtime_layer);
  text_layer_destroy(s_bigtime_sm_layer);
  text_layer_destroy(s_desctime_layer);
  text_layer_destroy(s_ddate_layer);
  text_layer_destroy(s_ddate_day_layer);
}

static void init(void) {

  /* if (persist_exists(KEY_TIMEZONE_OFFSET)) { */
  /*   utcoffset = persist_read_int(KEY_TIMEZONE_OFFSET); */
  /* } else { */
  /*   utcoffset = 28800; // assume pst */
  /*   persist_write_int(KEY_TIMEZONE_OFFSET, utcoffset); */
  /* } */
  utcoffset = 28800;

  app_message_register_inbox_received(appmsg_in_received);

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
