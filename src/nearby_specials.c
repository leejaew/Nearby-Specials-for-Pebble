#include <pebble.h>

static Window *window;
static TextLayer *venue_name_layer;
static TextLayer *venue_address_layer;
static TextLayer *venue_specials_layer;

void in_received_handler(DictionaryIterator *received, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got message from Javascript");

  Tuple *venue_tuple = dict_find(received, 1);
  if ( venue_tuple ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "1");
    text_layer_set_text( venue_name_layer, venue_tuple->value->cstring);
  }

  Tuple *address_tuple = dict_find(received, 2);
  if ( address_tuple ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "2");
    text_layer_set_text( venue_address_layer, address_tuple->value->cstring);
  }
  
  Tuple *specials_tuple = dict_find(received, 3);
  if ( specials_tuple ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "3");
    text_layer_set_text( venue_specials_layer, specials_tuple->value->cstring);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  venue_name_layer = text_layer_create((GRect) { .origin = { 0, 30 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(venue_name_layer, "Venue");
  text_layer_set_text_alignment(venue_name_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(venue_name_layer));

  venue_address_layer = text_layer_create((GRect) { .origin = { 0, 50 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(venue_address_layer, "Address");
  text_layer_set_text_alignment(venue_address_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(venue_address_layer));

  venue_specials_layer = text_layer_create((GRect) { .origin = { 0, 90 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(venue_specials_layer, "Specials");
  text_layer_set_text_alignment(venue_specials_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(venue_specials_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(venue_name_layer);
  text_layer_destroy(venue_address_layer);
  text_layer_destroy(venue_specials_layer);
}

static void setupMessaging(void) {
  app_message_register_inbox_received(in_received_handler);
  const uint32_t inbound_size = 128;
  const uint32_t outbound_size = 128;
  app_message_open(inbound_size, outbound_size);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
setupMessaging();
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
