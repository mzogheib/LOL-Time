#include <pebble.h>

// App keys
#define KEY_CHOSEN_COLOR 0

Window *my_window;
Layer *background_layer;
TextLayer *time_layer, *lol_layer;

GFont custom_font;

#ifdef PBL_PLATFORM_BASALT 
  #define TOTAL_COLORS 7
#elif PBL_PLATFORM_APLITE
  #define TOTAL_COLORS 1
#endif
GColor color_array[TOTAL_COLORS];
int current_color, chosen_color;
GColor color_top_bg, color_top_text;
GColor color_bot_bg, color_bot_text;

int current_lol;
#define TOTAL_LOL 7
char lol_array[TOTAL_LOL][5] = {"LOL", "LOLS", "LULZ", "ROFL", "LMAO", "HAHA", "HEHE"};

int init_finished;

// Returns the time
static char *write_time(struct tm tick_time) {
  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", &tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", &tick_time);
  }
   
  // Strip leading zero
  if(buffer[0]=='0') strcpy(buffer, buffer+1);
  
  return (char *)buffer;
}

// Return a random number that is different to the current number
int get_random_number(int current_num, int total_num) {
  int i;
  
  // Get a random number
  i = rand()%total_num;
  
  // If it's the same as the current then try again until different
  while(i==current_num) {
    i = rand()%total_num;
  }
  
  return i;
}
	
// Update the time and lol
void update_time(struct tm *tick_time) {
  text_layer_set_text(time_layer, write_time(*tick_time));
}

// Update the lol
void update_lol() {
	current_lol = get_random_number(current_lol, TOTAL_LOL);
	text_layer_set_text(lol_layer, lol_array[current_lol]);
}

// Update the colors
void update_colors() {
	if(chosen_color==-1) {
		current_color = get_random_number(current_color, TOTAL_COLORS);
	} else {
		current_color = chosen_color;
	}
	
	color_top_bg = color_bot_text = color_array[current_color];
	color_bot_bg = color_top_text = GColorWhite;
	
	text_layer_set_text_color(time_layer, color_top_text);
	text_layer_set_text_color(lol_layer, color_bot_text);
	layer_mark_dirty(background_layer);	
}

// Ticker handler
void handle_tick(struct tm *tick_time, TimeUnits units) {
	if(init_finished==1) {
		update_time(tick_time);
		update_lol();
		update_colors();
	}
	init_finished = 1;
}

static void draw_background(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, color_top_bg);
	graphics_fill_rect(ctx, GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h/2), GCornerNone, 0);
}

void init_colors() {
	color_array[0] = GColorBlack;
#ifdef PBL_PLATFORM_BASALT 
	color_array[1] = GColorRed;
	color_array[2] = GColorBlue;
	color_array[3] = GColorIslamicGreen;
	color_array[4] = GColorFashionMagenta;
	color_array[5] = GColorOrange;
	color_array[6] = GColorImperialPurple;
#endif
	current_color = -1;
	current_color = get_random_number(current_color, TOTAL_COLORS);
	color_top_bg = color_bot_text = color_array[current_color];
	color_bot_bg = color_top_text = GColorWhite;	
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *chosen_color_t = dict_find(iter, KEY_CHOSEN_COLOR);

	// set some global flags to random or otherwise then call the update color function
  if (chosen_color_t) {
		chosen_color = chosen_color_t->value->int32;
    persist_write_int(KEY_CHOSEN_COLOR, chosen_color);
  }
	update_colors();
}

void main_window_load() {
  Layer *window_layer = window_get_root_layer(my_window);
	GRect bounds = layer_get_bounds(window_layer);	
	
	custom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CUSTOM_FONT_42));
	
	background_layer = layer_create(bounds);
  layer_set_update_proc(background_layer, draw_background);
	
	time_layer = text_layer_create(GRect(0, 14, bounds.size.w, 80));
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, color_top_text);
	text_layer_set_font(time_layer, custom_font);
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
	
	lol_layer = text_layer_create(GRect(0, 98, bounds.size.w, 80));
	text_layer_set_background_color(lol_layer, GColorClear);
	text_layer_set_text_color(lol_layer, color_bot_text);
	text_layer_set_font(lol_layer, custom_font);
	text_layer_set_text_alignment(lol_layer, GTextAlignmentCenter);
	
  layer_add_child(window_layer, background_layer);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
  layer_add_child(window_layer, text_layer_get_layer(lol_layer));
	
	// If any persistant data then load those and update colors
  if (persist_read_int(KEY_CHOSEN_COLOR)) {
    chosen_color = persist_read_int(KEY_CHOSEN_COLOR);
		update_colors();
  }

}

void main_window_unload() {
	layer_destroy(background_layer);
  layer_destroy(text_layer_get_layer(time_layer));
  layer_destroy(text_layer_get_layer(lol_layer));
	fonts_unload_custom_font(custom_font);
}

void init() {
  my_window = window_create();
	
	srand(time(NULL));
	init_colors();
	
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
	
  window_stack_push(my_window, true);
		
  // Time for display on start up
	init_finished = 0;
  time_t temp_time = time(NULL); 
  struct tm *tick_time = localtime(&temp_time);
	update_time(tick_time);
	current_lol = -1;
	update_lol();
		
  // Subcribe to ticker 
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);	
	
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void deinit() {
  window_destroy(my_window);
}

int main(void) {
	init();
  app_event_loop();
	deinit();
}