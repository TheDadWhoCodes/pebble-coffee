#include <pebble.h>

// --- Main Menu UI Elements ---
static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[1];
static SimpleMenuItem s_menu_items[3];

/** AEROPRESS */
// --- Aeropress Detail UI Elements (Step 1) ---
static Window *s_aeropress_window;
static TextLayer *s_aeropress_text_layer;
static TextLayer *s_aeropress_select_hint_layer;

// --- Aeropress Step 2 UI Elements ---
static Window *s_aeropress_step2_window;
static TextLayer *s_aeropress_step2_text_layer;
static TextLayer *s_aeropress_step2_select_hint_layer;

// --- Aeropress Step 3 (Initial Countdown) UI Elements ---
static Window *s_aeropress_step3_countdown_window;
static TextLayer *s_aeropress_countdown_text_layer;
static TextLayer *s_aeropress_countdown_hint_layer;
static int s_aeropress_countdown_value; // Unique countdown value for Aeropress
static AppTimer *s_aeropress_initial_countdown_timer;

// --- Aeropress Step 4 (Bloom Timer) UI Elements ---
static Window *s_aeropress_brew_timer_window;
static TextLayer *s_aeropress_brew_timer_text_layer; // Timer for bloom
static TextLayer *s_aeropress_brew_message_layer;   // Bloom message
static AppTimer *s_aeropress_bloom_countdown_timer; // Unique timer for Aeropress bloom

// --- Aeropress Step 5 (Pour & Stir) UI Elements ---
static Window *s_aeropress_step5_pour_stir_window;
static TextLayer *s_aeropress_step5_timer_text_layer;
static TextLayer *s_aeropress_step5_message_layer;
static int s_step5_countdown_value;
static AppTimer *s_step5_countdown_timer;

// --- Aeropress Step 6 (Press) UI Elements ---
static Window *s_aeropress_step6_press_window;
static TextLayer *s_aeropress_step6_timer_text_layer;
static TextLayer *s_aeropress_step6_message_layer;
static int s_step6_countdown_value;
static AppTimer *s_step6_countdown_timer;

// --- Aeropress Step 7 (Finish) UI Elements ---
static Window *s_aeropress_step7_finish_window;
static TextLayer *s_aeropress_step7_message_layer;
static TextLayer *s_aeropress_step7_select_hint_layer;


/** FRENCH PRESS */
// --- French Press Detail UI Elements (Step 1) ---
static Window *s_french_press_window;
static TextLayer *s_french_press_text_layer;
static TextLayer *s_french_press_select_hint_layer;

// --- French Press Step 2 (Initial Countdown) UI Elements ---
static Window *s_french_press_step2_countdown_window;
static TextLayer *s_french_press_countdown_text_layer;
static TextLayer *s_french_press_countdown_hint_layer;
static int s_french_press_initial_countdown_value; // Unique initial countdown value for French Press
static AppTimer *s_french_press_initial_countdown_timer;

// --- French Press Step 3 (Main Steep Timer) UI Elements ---
static Window *s_french_press_brew_timer_window;
static TextLayer *s_french_press_brew_timer_text_layer; // Timer for steep
static TextLayer *s_french_press_brew_message_layer;   // Steep message
static int s_french_press_steep_value; // Unique steep value for French Press
static AppTimer *s_french_press_steep_timer; // Unique timer for French Press steep

// --- French Press Step 4 (Stir & Scoop) UI Elements ---
static Window *s_french_press_step4_stir_scoop_window; // Renamed for clarity
static TextLayer *s_french_press_step4_timer_text_layer;
static TextLayer *s_french_press_step4_message_layer;
static int s_french_press_step4_countdown_value; // Corrected variable name
static AppTimer *s_french_press_step4_countdown_timer; // Corrected variable name

// --- French Press Step 5 (Plunge & Press) UI Elements ---
static Window *s_french_press_step5_plunge_press_window; // Renamed for clarity
static TextLayer *s_french_press_step5_timer_text_layer;
static TextLayer *s_french_press_step5_message_layer;
static int s_french_press_step5_countdown_value;
static AppTimer *s_french_press_step5_countdown_timer; // Corrected variable name

// --- French Press Step 6 (Finish) UI Elements ---
static Window *s_french_press_step6_finish_window;
static TextLayer *s_french_press_step6_message_layer;
static TextLayer *s_french_press_step6_select_hint_layer;


// --- FUNCTION PROTOTYPES (Forward Declarations) ---
// These are necessary because functions are called before they are defined later in the file.

// Main Menu Callbacks (if you uncomment these later)
static void aeropress_callback(int index, void *ctx);
static void pour_over_callback(int index, void *ctx); // Still unused, consider removing if not needed
static void french_press_callback(int index, void *ctx);

// Aeropress Handlers
static void aeropress_window_load(Window *window);
static void aeropress_window_unload(Window *window);
static void aeropress_click_config_provider(void *context);
static void aeropress_select_click_handler(ClickRecognizerRef recognizer, void *context); // Corrected signature

static void aeropress_step2_window_load(Window *window);
static void aeropress_step2_window_unload(Window *window);
static void aeropress_step2_click_config_provider(void *context);
static void aeropress_step2_select_click_handler(ClickRecognizerRef recognizer, void *context); // Corrected signature

static void aeropress_step3_countdown_window_load(Window *window);
static void aeropress_step3_countdown_window_unload(Window *window);
static void aeropress_initial_countdown_timer_callback(void *data);

static void aeropress_brew_timer_window_load(Window *window);
static void aeropress_brew_timer_window_unload(Window *window);
static void aeropress_bloom_countdown_timer_callback(void *data);

static void aeropress_step5_pour_stir_window_load(Window *window);
static void aeropress_step5_pour_stir_window_unload(Window *window);
static void step5_countdown_timer_callback(void *data);

static void aeropress_step6_press_window_load(Window *window);
static void aeropress_step6_press_window_unload(Window *window);
static void step6_countdown_timer_callback(void *data);

static void aeropress_step7_finish_window_load(Window *window);
static void aeropress_step7_finish_window_unload(Window *window);
static void aeropress_step7_click_config_provider(void *context);
static void aeropress_step7_select_click_handler(ClickRecognizerRef recognizer, void *context); // Corrected signature


// French Press Handlers
static void french_press_window_load(Window *window);
static void french_press_window_unload(Window *window);
static void french_press_click_config_provider(void *context);
static void french_press_select_click_handler(ClickRecognizerRef recognizer, void *context); // Corrected signature

static void french_press_step2_countdown_window_load(Window *window);
static void french_press_step2_countdown_window_unload(Window *window);
static void french_press_initial_countdown_timer_callback(void *data);

static void french_press_brew_timer_window_load(Window *window);
static void french_press_brew_timer_window_unload(Window *window);
static void french_press_steep_countdown_timer_callback(void *data);

static void french_press_step4_stir_scoop_window_load(Window *window);
static void french_press_step4_stir_scoop_window_unload(Window *window);
static void french_press_step4_countdown_timer_callback(void *data);

static void french_press_step5_plunge_press_window_load(Window *window);
static void french_press_step5_plunge_press_window_unload(Window *window);
static void french_press_step5_countdown_timer_callback(void *data);

static void french_press_step5_click_config_provider(void *context);
static void french_press_step5_select_click_handler(ClickRecognizerRef recognizer, void *context);


/** FRENCH PRESS STEPS - HANDLERS */

// --- French Press Step 5 (Finish) Window Handlers ---
static void french_press_step5_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "French Press Finish: Select pressed, returning to main menu.");
  vibes_short_pulse();
  window_stack_pop_all(true); // Pop all windows until main menu is at the top
}

static void french_press_step5_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, french_press_step5_select_click_handler);
}

static void french_press_step5_plunge_press_window_load(Window *window) { // Renamed
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_french_press_step5_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_french_press_step5_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_step5_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_step5_timer_text_layer));

  s_french_press_step5_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_french_press_step5_message_layer,
                      "Insert plunger, slowly press down.\n"
                      "Stop plunger JUST under surface.\n"
                      "Pour gently and enjoy.");
  text_layer_set_text_alignment(s_french_press_step5_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_step5_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_step5_message_layer));

  s_french_press_step6_select_hint_layer = text_layer_create(GRect(0, bounds.size.h - 20, bounds.size.w, 20));
  text_layer_set_text(s_french_press_step6_select_hint_layer, "SELECT to Go Back");
  text_layer_set_text_alignment(s_french_press_step6_select_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_step6_select_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_step6_select_hint_layer));

  window_set_click_config_provider(window, french_press_step5_click_config_provider);
}

static void french_press_step5_plunge_press_window_unload(Window *window) { // Renamed
  if (s_french_press_step5_countdown_timer) { // Corrected timer variable
    app_timer_cancel(s_french_press_step5_countdown_timer);
    s_french_press_step5_countdown_timer = NULL;
  }
  text_layer_destroy(s_french_press_step5_timer_text_layer); // Corrected TextLayer
  s_french_press_step5_timer_text_layer = NULL;
  text_layer_destroy(s_french_press_step5_message_layer);   // Corrected TextLayer
  s_french_press_step5_message_layer = NULL;
}


// --- French Press Step 4 (Stir & Scoop) Window Handlers ---
static void french_press_step4_countdown_timer_callback(void *data) { // Corrected callback name
  if (s_french_press_step4_countdown_value > 0) {
    s_french_press_step4_countdown_value--;
    static char s_buffer[16];
    int minutes = s_french_press_step4_countdown_value / 60;
    int seconds = s_french_press_step4_countdown_value % 60;
    snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", minutes, seconds);
    text_layer_set_text(s_french_press_step4_timer_text_layer, s_buffer);

    s_french_press_step4_countdown_timer = app_timer_register(1000, french_press_step4_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "French Press Step 4 Countdown finished! Going to Plunge screen.");
    vibes_short_pulse();
    if (!s_french_press_step5_plunge_press_window) { // Renamed window
      s_french_press_step5_plunge_press_window = window_create();
      window_set_window_handlers(s_french_press_step5_plunge_press_window, (WindowHandlers) {
        .load = french_press_step5_plunge_press_window_load, // Corrected handler
        .unload = french_press_step5_plunge_press_window_unload // Corrected handler
      });
    }
    window_stack_push(s_french_press_step5_plunge_press_window, true); // Renamed window
  }
}

static void french_press_step4_stir_scoop_window_load(Window *window) { // Renamed for clarity
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_french_press_step4_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_french_press_step4_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_step4_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_step4_timer_text_layer));

  s_french_press_step4_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_french_press_step4_message_layer,
                      "Stir and break crust at top. Scoop out foam and bits on top.");
  text_layer_set_text_alignment(s_french_press_step4_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_step4_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_step4_message_layer));

  s_french_press_step4_countdown_value = 300; // Wait 5 minutes after stir/scoop
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", s_french_press_step4_countdown_value / 60, s_french_press_step4_countdown_value % 60);
  text_layer_set_text(s_french_press_step4_timer_text_layer, s_buffer);
  s_french_press_step4_countdown_timer = app_timer_register(1000, french_press_step4_countdown_timer_callback, NULL);
}

static void french_press_step4_stir_scoop_window_unload(Window *window) { // Renamed for clarity
  if (s_french_press_step4_countdown_timer) {
    app_timer_cancel(s_french_press_step4_countdown_timer);
    s_french_press_step4_countdown_timer = NULL;
  }
  text_layer_destroy(s_french_press_step4_timer_text_layer);
  s_french_press_step4_timer_text_layer = NULL;
  text_layer_destroy(s_french_press_step4_message_layer);
  s_french_press_step4_message_layer = NULL;
}

// --- French Press Step 3 (Main Steep Timer) Window Handlers ---
static void french_press_steep_countdown_timer_callback(void *data) { // Renamed for clarity
  if (s_french_press_steep_value > 0) {
    s_french_press_steep_value--;
    static char s_buffer[16];
    int minutes = s_french_press_steep_value / 60;
    int seconds = s_french_press_steep_value % 60;
    snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", minutes, seconds);
    text_layer_set_text(s_french_press_brew_timer_text_layer, s_buffer);

    s_french_press_steep_timer = app_timer_register(1000, french_press_steep_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "French Press Steep Countdown finished! Going to Stir and Scoop screen.");
    vibes_short_pulse();
    if (!s_french_press_step4_stir_scoop_window) { // Renamed window
      s_french_press_step4_stir_scoop_window = window_create();
      window_set_window_handlers(s_french_press_step4_stir_scoop_window, (WindowHandlers) {
        .load = french_press_step4_stir_scoop_window_load, // Corrected handler
        .unload = french_press_step4_stir_scoop_window_unload // Corrected handler
      });
    }
    window_stack_push(s_french_press_step4_stir_scoop_window, true); // Renamed window
  }
}

static void french_press_brew_timer_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_french_press_brew_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_french_press_brew_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_brew_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_brew_timer_text_layer));

  s_french_press_brew_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_french_press_brew_message_layer,
                      "Pour all 500g water in circles over grounds. Steep coffee.");
  text_layer_set_text_alignment(s_french_press_brew_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_brew_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_brew_message_layer));

  s_french_press_steep_value = 240; // 4 minutes for main steep
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", s_french_press_steep_value / 60, s_french_press_steep_value % 60);
  text_layer_set_text(s_french_press_brew_timer_text_layer, s_buffer);
  s_french_press_steep_timer = app_timer_register(1000, french_press_steep_countdown_timer_callback, NULL);
}

static void french_press_brew_timer_window_unload(Window *window) {
  if (s_french_press_steep_timer) {
    app_timer_cancel(s_french_press_steep_timer);
    s_french_press_steep_timer = NULL;
  }
  text_layer_destroy(s_french_press_brew_timer_text_layer);
  s_french_press_brew_timer_text_layer = NULL;
  text_layer_destroy(s_french_press_brew_message_layer);
  s_french_press_brew_message_layer = NULL;
}


// --- French Press Step 2 (Initial Countdown) Window Handlers ---
static void french_press_initial_countdown_timer_callback(void *data) { // Renamed for clarity
  if (s_french_press_initial_countdown_value > 0) {
    s_french_press_initial_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%d", s_french_press_initial_countdown_value);
    text_layer_set_text(s_french_press_countdown_text_layer, s_buffer);

    s_french_press_initial_countdown_timer = app_timer_register(1000, french_press_initial_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "French Press Initial Countdown finished! Starting Steep timer.");
    vibes_short_pulse();
    if (!s_french_press_brew_timer_window) {
        s_french_press_brew_timer_window = window_create();
        window_set_window_handlers(s_french_press_brew_timer_window, (WindowHandlers) {
            .load = french_press_brew_timer_window_load,
            .unload = french_press_brew_timer_window_unload
        });
    }
    window_stack_push(s_french_press_brew_timer_window, true);
  }
}

static void french_press_step2_countdown_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_french_press_countdown_hint_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(40, 20), bounds.size.w, 30));
    text_layer_set_text(s_french_press_countdown_hint_layer, "Brew in:");
    text_layer_set_text_alignment(s_french_press_countdown_hint_layer, GTextAlignmentCenter);
    text_layer_set_font(s_french_press_countdown_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(s_french_press_countdown_hint_layer));

    s_french_press_countdown_text_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(70, 50), bounds.size.w, 60));
    text_layer_set_text_alignment(s_french_press_countdown_text_layer, GTextAlignmentCenter);
    text_layer_set_font(s_french_press_countdown_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(s_french_press_countdown_text_layer));

    s_french_press_initial_countdown_value = 3;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%d", s_french_press_initial_countdown_value);
    text_layer_set_text(s_french_press_countdown_text_layer, s_buffer);
    s_french_press_initial_countdown_timer = app_timer_register(1000, french_press_initial_countdown_timer_callback, NULL);
}

static void french_press_step2_countdown_window_unload(Window *window) {
  if (s_french_press_initial_countdown_timer) { // Corrected timer variable
    app_timer_cancel(s_french_press_initial_countdown_timer);
    s_french_press_initial_countdown_timer = NULL;
  }
  text_layer_destroy(s_french_press_countdown_text_layer);
  s_french_press_countdown_text_layer = NULL;

  text_layer_destroy(s_french_press_countdown_hint_layer);
  s_french_press_countdown_hint_layer = NULL;
}

// --- French Press Detail Window Handlers (Step 1) ---
static void french_press_select_click_handler(ClickRecognizerRef recognizer, void *context) { // Corrected signature
  APP_LOG(APP_LOG_LEVEL_INFO, "French Press Next Step clicked! Going to Step 2.");
  if (!s_french_press_step2_countdown_window) {
    s_french_press_step2_countdown_window = window_create();
    window_set_window_handlers(s_french_press_step2_countdown_window, (WindowHandlers) {
      .load = french_press_step2_countdown_window_load,
      .unload = french_press_step2_countdown_window_unload
    });
  }
  window_stack_push(s_french_press_step2_countdown_window, true);
}

static void french_press_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, french_press_select_click_handler);
}

static void french_press_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_french_press_text_layer = text_layer_create(GRect(5, 5, bounds.size.w - 10, bounds.size.h - 40));
  text_layer_set_text(s_french_press_text_layer,
                      "Grind 30 grams of breadcrumb-size coffee, into French press.\n"
                      "Boil minimum 500ml of water (203F).");
  text_layer_set_text_alignment(s_french_press_text_layer, GTextAlignmentLeft);
  text_layer_set_font(s_french_press_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_text_layer));

  s_french_press_select_hint_layer = text_layer_create(GRect(0, bounds.size.h - 20, bounds.size.w, 20));
  text_layer_set_text(s_french_press_select_hint_layer, "SELECT to Continue");
  text_layer_set_text_alignment(s_french_press_select_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_french_press_select_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(s_french_press_select_hint_layer));

  window_set_click_config_provider(window, french_press_click_config_provider);
}

static void french_press_window_unload(Window *window) {
  text_layer_destroy(s_french_press_text_layer);
  s_french_press_text_layer = NULL;

  text_layer_destroy(s_french_press_select_hint_layer);
  s_french_press_select_hint_layer = NULL;
}


/** AEROPRESS STEPS - HANDLERS */

// --- Aeropress Step 7 (Finish) Window Handlers ---
static void aeropress_step7_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Finish: Select pressed, returning to main menu.");
  vibes_short_pulse();
  window_stack_pop_all(true); // Pop all windows until main menu is at the top
}

static void aeropress_step7_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, aeropress_step7_select_click_handler);
}

static void aeropress_step7_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_step7_message_layer = text_layer_create(GRect(5, 10, bounds.size.w - 10, bounds.size.h - 30));
  text_layer_set_text(s_aeropress_step7_message_layer,
                      "Add additional 25g of water and stir.\n\n"
                      "Coffee is ready!");
  text_layer_set_text_alignment(s_aeropress_step7_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step7_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step7_message_layer));

  s_aeropress_step7_select_hint_layer = text_layer_create(GRect(0, bounds.size.h - 20, bounds.size.w, 20));
  text_layer_set_text(s_aeropress_step7_select_hint_layer, "SELECT to Go Back");
  text_layer_set_text_alignment(s_aeropress_step7_select_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step7_select_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step7_select_hint_layer));

  window_set_click_config_provider(window, aeropress_step7_click_config_provider);
}

static void aeropress_step7_window_unload(Window *window) {
  text_layer_destroy(s_aeropress_step7_message_layer);
  s_aeropress_step7_message_layer = NULL;
  text_layer_destroy(s_aeropress_step7_select_hint_layer);
  s_aeropress_step7_select_hint_layer = NULL;
}


// --- Aeropress Step 6 (Press) Window Handlers ---
static void step6_countdown_timer_callback(void *data) {
  if (s_step6_countdown_value > 0) {
    s_step6_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%02d", s_step6_countdown_value); // Display seconds only
    text_layer_set_text(s_aeropress_step6_timer_text_layer, s_buffer);

    s_step6_countdown_timer = app_timer_register(1000, step6_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Step 6 Countdown finished! Going to Finish screen.");
    vibes_short_pulse();
    if (!s_aeropress_step7_finish_window) {
      s_aeropress_step7_finish_window = window_create();
      window_set_window_handlers(s_aeropress_step7_finish_window, (WindowHandlers) {
        .load = aeropress_step7_window_load,
        .unload = aeropress_step7_window_unload
      });
    }
    window_stack_push(s_aeropress_step7_finish_window, true);
  }
}

static void aeropress_step6_press_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_step6_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_aeropress_step6_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step6_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step6_timer_text_layer));

  s_aeropress_step6_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_aeropress_step6_message_layer,
                      "Insert plunger/Plunge and press into cup.");
  text_layer_set_text_alignment(s_aeropress_step6_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step6_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step6_message_layer));

  s_step6_countdown_value = 30; // 30 seconds
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d", s_step6_countdown_value);
  text_layer_set_text(s_aeropress_step6_timer_text_layer, s_buffer);
  s_step6_countdown_timer = app_timer_register(1000, step6_countdown_timer_callback, NULL);
}

static void aeropress_step6_press_window_unload(Window *window) {
  if (s_step6_countdown_timer) {
    app_timer_cancel(s_step6_countdown_timer);
    s_step6_countdown_timer = NULL;
  }
  text_layer_destroy(s_aeropress_step6_timer_text_layer);
  s_aeropress_step6_timer_text_layer = NULL;
  text_layer_destroy(s_aeropress_step6_message_layer);
  s_aeropress_step6_message_layer = NULL;
}


// --- Aeropress Step 5 (Pour & Stir) Window Handlers ---
static void step5_countdown_timer_callback(void *data) {
  if (s_step5_countdown_value > 0) {
    s_step5_countdown_value--;
    static char s_buffer[16];
    int minutes = s_step5_countdown_value / 60;
    int seconds = s_step5_countdown_value % 60;
    snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", minutes, seconds);
    text_layer_set_text(s_aeropress_step5_timer_text_layer, s_buffer);

    s_step5_countdown_timer = app_timer_register(1000, step5_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Step 5 Countdown finished! Going to Press screen.");
    vibes_short_pulse();
    if (!s_aeropress_step6_press_window) {
      s_aeropress_step6_press_window = window_create();
      window_set_window_handlers(s_aeropress_step6_press_window, (WindowHandlers) {
        .load = aeropress_step6_press_window_load,
        .unload = aeropress_step6_press_window_unload
      });
    }
    window_stack_push(s_aeropress_step6_press_window, true);
  }
}

static void aeropress_step5_pour_stir_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_step5_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_aeropress_step5_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step5_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step5_timer_text_layer));

  s_aeropress_step5_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_aeropress_step5_message_layer,
                      "Add additional 225g of water\n"
                      "Stir thoroughly, then steep.");
  text_layer_set_text_alignment(s_aeropress_step5_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step5_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step5_message_layer));

  s_step5_countdown_value = 90; // 1 minute 30 seconds
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d:%02d", s_step5_countdown_value / 60, s_step5_countdown_value % 60);
  text_layer_set_text(s_aeropress_step5_timer_text_layer, s_buffer);
  s_step5_countdown_timer = app_timer_register(1000, step5_countdown_timer_callback, NULL);
}

static void aeropress_step5_pour_stir_window_unload(Window *window) {
  if (s_step5_countdown_timer) {
    app_timer_cancel(s_step5_countdown_timer);
    s_step5_countdown_timer = NULL;
  }
  text_layer_destroy(s_aeropress_step5_timer_text_layer);
  s_aeropress_step5_timer_text_layer = NULL;
  text_layer_destroy(s_aeropress_step5_message_layer);
  s_aeropress_step5_message_layer = NULL;
}


// --- Aeropress Step 4 (Bloom Timer) Window Handlers ---
static void aeropress_bloom_countdown_timer_callback(void *data) { // Renamed for clarity
  if (s_aeropress_countdown_value > 0) {
    s_aeropress_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%02d", s_aeropress_countdown_value); // Display seconds only
    text_layer_set_text(s_aeropress_brew_timer_text_layer, s_buffer);

    s_aeropress_bloom_countdown_timer = app_timer_register(1000, aeropress_bloom_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Bloom Countdown finished! Going to Pour & Stir screen.");
    vibes_short_pulse();
    if (!s_aeropress_step5_pour_stir_window) {
      s_aeropress_step5_pour_stir_window = window_create();
      window_set_window_handlers(s_aeropress_step5_pour_stir_window, (WindowHandlers) {
        .load = aeropress_step5_pour_stir_window_load,
        .unload = aeropress_step5_pour_stir_window_unload
      });
    }
    window_stack_push(s_aeropress_step5_pour_stir_window, true);
  }
}

static void aeropress_brew_timer_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_brew_timer_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, 30));
  text_layer_set_text_alignment(s_aeropress_brew_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_brew_timer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_brew_timer_text_layer));

  s_aeropress_brew_message_layer = text_layer_create(GRect(5, 35, bounds.size.w - 10, bounds.size.h - 35));
  text_layer_set_text(s_aeropress_brew_message_layer,
                      "Coffee into chamber. Pour 50g hot water over coffee, swirl, let bloom.");
  text_layer_set_text_alignment(s_aeropress_brew_message_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_brew_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_brew_message_layer));

  s_aeropress_countdown_value = 30; // Starting from 30 seconds for bloom
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d", s_aeropress_countdown_value);
  text_layer_set_text(s_aeropress_brew_timer_text_layer, s_buffer);
  s_aeropress_bloom_countdown_timer = app_timer_register(1000, aeropress_bloom_countdown_timer_callback, NULL);
}

static void aeropress_brew_timer_window_unload(Window *window) {
  if (s_aeropress_bloom_countdown_timer) {
    app_timer_cancel(s_aeropress_bloom_countdown_timer);
    s_aeropress_bloom_countdown_timer = NULL;
  }
  text_layer_destroy(s_aeropress_brew_timer_text_layer);
  s_aeropress_brew_timer_text_layer = NULL;
  text_layer_destroy(s_aeropress_brew_message_layer);
  s_aeropress_brew_message_layer = NULL;
}


// --- Aeropress Step 3 (Initial Countdown) Window Handlers ---
static void aeropress_initial_countdown_timer_callback(void *data) { // Renamed for clarity
  if (s_aeropress_countdown_value > 0) {
    s_aeropress_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%d", s_aeropress_countdown_value);
    text_layer_set_text(s_aeropress_countdown_text_layer, s_buffer);

    s_aeropress_initial_countdown_timer = app_timer_register(1000, aeropress_initial_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Initial Countdown finished! Starting Bloom timer.");
    vibes_short_pulse();
    if (!s_aeropress_brew_timer_window) {
        s_aeropress_brew_timer_window = window_create();
        window_set_window_handlers(s_aeropress_brew_timer_window, (WindowHandlers) {
            .load = aeropress_brew_timer_window_load,
            .unload = aeropress_brew_timer_window_unload
        });
    }
    window_stack_push(s_aeropress_brew_timer_window, true);
  }
}

static void aeropress_step3_countdown_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_countdown_hint_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(40, 20), bounds.size.w, 30));
  text_layer_set_text(s_aeropress_countdown_hint_layer, "Brew in:");
  text_layer_set_text_alignment(s_aeropress_countdown_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_countdown_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_countdown_hint_layer));

  s_aeropress_countdown_text_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(70, 50), bounds.size.w, 60));
  text_layer_set_text_alignment(s_aeropress_countdown_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_countdown_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_countdown_text_layer));

  s_aeropress_countdown_value = 3;
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%d", s_aeropress_countdown_value);
  text_layer_set_text(s_aeropress_countdown_text_layer, s_buffer);
  s_aeropress_initial_countdown_timer = app_timer_register(1000, aeropress_initial_countdown_timer_callback, NULL);
}

static void aeropress_step3_countdown_window_unload(Window *window) {
  if (s_aeropress_initial_countdown_timer) {
    app_timer_cancel(s_aeropress_initial_countdown_timer);
    s_aeropress_initial_countdown_timer = NULL;
  }
  text_layer_destroy(s_aeropress_countdown_text_layer);
  s_aeropress_countdown_text_layer = NULL;
  text_layer_destroy(s_aeropress_countdown_hint_layer);
  s_aeropress_countdown_hint_layer = NULL;
}


// --- Aeropress Step 2 Window Handlers ---
static void aeropress_step2_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Step 2: Select pressed, going to Step 3 (initial countdown).");
  if (!s_aeropress_step3_countdown_window) {
    s_aeropress_step3_countdown_window = window_create();
    window_set_window_handlers(s_aeropress_step3_countdown_window, (WindowHandlers) {
      .load = aeropress_step3_countdown_window_load,
      .unload = aeropress_step3_countdown_window_unload
    });
  }
  window_stack_push(s_aeropress_step3_countdown_window, true);
}

static void aeropress_step2_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, aeropress_step2_select_click_handler);
}

static void aeropress_step2_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_step2_text_layer = text_layer_create(GRect(5, 5, bounds.size.w - 10, bounds.size.h - 40));
  text_layer_set_text(s_aeropress_step2_text_layer,
                      "Place Aeropress on scale.\n"
                      "Zero scale. Boil 300g of water (197F).");
  text_layer_set_text_alignment(s_aeropress_step2_text_layer, GTextAlignmentLeft);
  text_layer_set_font(s_aeropress_step2_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step2_text_layer));

  s_aeropress_step2_select_hint_layer = text_layer_create(GRect(0, bounds.size.h - 20, bounds.size.w, 20));
  text_layer_set_text(s_aeropress_step2_select_hint_layer, "SELECT to Continue");
  text_layer_set_text_alignment(s_aeropress_step2_select_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_step2_select_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_step2_select_hint_layer));

  window_set_click_config_provider(window, aeropress_step2_click_config_provider);
}

static void aeropress_step2_window_unload(Window *window) {
  text_layer_destroy(s_aeropress_step2_text_layer);
  s_aeropress_step2_text_layer = NULL;
  text_layer_destroy(s_aeropress_step2_select_hint_layer);
  s_aeropress_step2_select_hint_layer = NULL;
}


// --- Aeropress Detail Window Handlers (Step 1) ---
// This is the handler for the 'SELECT' button on the *first* Aeropress detail screen (s_aeropress_window)
static void aeropress_select_click_handler(ClickRecognizerRef recognizer, void *context) { // Corrected signature
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Detail: Select pressed, going to Step 2.");
  if (!s_aeropress_step2_window) {
    s_aeropress_step2_window = window_create();
    window_set_window_handlers(s_aeropress_step2_window, (WindowHandlers) {
      .load = aeropress_step2_window_load,
      .unload = aeropress_step2_window_unload
    });
  }
  window_stack_push(s_aeropress_step2_window, true);
}

static void aeropress_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, aeropress_select_click_handler);
}

static void aeropress_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_aeropress_text_layer = text_layer_create(GRect(5, 5, bounds.size.w - 10, bounds.size.h - 40));
  text_layer_set_text(s_aeropress_text_layer,
                      "Grind 23g coffee (fine). Non-Prismo, Invert \n"
                      "Aeropress, insert paper filter into cap, rinse.\n"
                      "Prismo, Normal Aeropress, put cap on.");
  text_layer_set_text_alignment(s_aeropress_text_layer, GTextAlignmentLeft);
  text_layer_set_font(s_aeropress_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_text_layer));

  s_aeropress_select_hint_layer = text_layer_create(GRect(0, bounds.size.h - 20, bounds.size.w, 20));
  text_layer_set_text(s_aeropress_select_hint_layer, "SELECT to Continue");
  text_layer_set_text_alignment(s_aeropress_select_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_select_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_select_hint_layer));

  window_set_click_config_provider(window, aeropress_click_config_provider);
}

static void aeropress_window_unload(Window *window) {
  text_layer_destroy(s_aeropress_text_layer);
  s_aeropress_text_layer = NULL;
  text_layer_destroy(s_aeropress_select_hint_layer);
  s_aeropress_select_hint_layer = NULL;
}


// --- Main Menu Callbacks (where your SimpleMenuLayer items are defined) ---
// These are the callbacks that the SimpleMenuLayer calls when an item is selected.
// They push the *first* window for each brewing method.

static void aeropress_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress selected from main menu!");
  if (!s_aeropress_window) {
    s_aeropress_window = window_create();
    window_set_window_handlers(s_aeropress_window, (WindowHandlers) {
      .load = aeropress_window_load,
      .unload = aeropress_window_unload
    });
  }
  window_stack_push(s_aeropress_window, true);
}

// NOTE: pour_over_callback is still declared but not defined. If you don't need it, remove its prototype and menu item.
static void pour_over_callback(int index, void *ctx) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Pour Over selected - handler not implemented!");
    vibes_long_pulse(); // Indicate it's not ready
}


static void french_press_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "French Press selected from main menu!");
  if (!s_french_press_window) {
    s_french_press_window = window_create();
    window_set_window_handlers(s_french_press_window, (WindowHandlers) {
      .load = french_press_window_load,
      .unload = french_press_window_unload
    });
  }
  window_stack_push(s_french_press_window, true);
}

// This section assumes your `init`, `deinit`, `main_window_load`, `main_window_unload`, and `main` functions are defined elsewhere in your project.
// If you're building a new project from scratch, you'd typically include these as well.

static void main_window_load(Window *window) {
  // Setup menu items
  s_menu_items[0] = (SimpleMenuItem) {
    .title = "Aeropress",
    .subtitle = "Joshua Rey recipe",
    .callback = aeropress_callback,
  };
  s_menu_items[1] = (SimpleMenuItem) {
    .title = "French Press",
    .subtitle = "James Hoffman recipe",
    .callback = french_press_callback,
  };
  s_menu_items[2] = (SimpleMenuItem) {
    .title = "Pour Over",
    .subtitle = "Another James Hoffman recipe",
    .callback = pour_over_callback,
  };

  // Setup menu section
  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = 3,
    .items = s_menu_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, 1, NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

static void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}