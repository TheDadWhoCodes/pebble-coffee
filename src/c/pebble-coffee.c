#include <pebble.h>

// --- Main Menu UI Elements ---
static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[1];
static SimpleMenuItem s_menu_items[3];

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
static int s_countdown_value; // Used for step 3 countdown
static AppTimer *s_initial_countdown_timer; // MODIFIED: New timer for initial 3-second countdown

// --- Aeropress Step 4 (Bloom Timer) UI Elements ---
static Window *s_aeropress_brew_timer_window;
static TextLayer *s_aeropress_brew_timer_text_layer; // Timer for bloom
static TextLayer *s_aeropress_brew_message_layer;   // Bloom message
static AppTimer *s_bloom_countdown_timer; // Timer for bloom, using specific name for clarity

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


// --- Callbacks for Menu Items (defined later) ---
static void aeropress_callback(int index, void *ctx);
static void pour_over_callback(int index, void *ctx);
static void french_press_callback(int index, void *ctx);


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
                      "Add an additional 25g of water and stir.\n\n"
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
                      "Insert plunger and press into cup.");
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
                      "Pour another 225g of water\n"
                      "then stir, and wait.");
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
static void bloom_countdown_timer_callback(void *data) {
  if (s_countdown_value > 0) { // Using s_countdown_value for consistency with previous step
    s_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%02d", s_countdown_value); // Display seconds only
    text_layer_set_text(s_aeropress_brew_timer_text_layer, s_buffer);

    s_bloom_countdown_timer = app_timer_register(1000, bloom_countdown_timer_callback, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Bloom Countdown finished! Going to Pour & Stir screen.");
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

  s_countdown_value = 30; // Starting from 30 seconds for bloom
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%02d", s_countdown_value);
  text_layer_set_text(s_aeropress_brew_timer_text_layer, s_buffer);
  s_bloom_countdown_timer = app_timer_register(1000, bloom_countdown_timer_callback, NULL);
}

static void aeropress_brew_timer_window_unload(Window *window) {
  if (s_bloom_countdown_timer) {
    app_timer_cancel(s_bloom_countdown_timer);
    s_bloom_countdown_timer = NULL;
  }
  text_layer_destroy(s_aeropress_brew_timer_text_layer);
  s_aeropress_brew_timer_text_layer = NULL;
  text_layer_destroy(s_aeropress_brew_message_layer);
  s_aeropress_brew_message_layer = NULL;
}


// --- Aeropress Step 3 (Initial Countdown) Window Handlers ---
static void countdown_timer_callback(void *data) {
  if (s_countdown_value > 0) {
    s_countdown_value--;
    static char s_buffer[16];
    snprintf(s_buffer, sizeof(s_buffer), "%d", s_countdown_value);
    text_layer_set_text(s_aeropress_countdown_text_layer, s_buffer);

    s_initial_countdown_timer = app_timer_register(1000, countdown_timer_callback, NULL); // MODIFIED
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Countdown finished! Starting Bloom timer.");
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
  text_layer_set_text(s_aeropress_countdown_hint_layer, "Starting in:");
  text_layer_set_text_alignment(s_aeropress_countdown_hint_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_countdown_hint_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_countdown_hint_layer));

  s_aeropress_countdown_text_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(70, 50), bounds.size.w, 60));
  text_layer_set_text_alignment(s_aeropress_countdown_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_aeropress_countdown_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_aeropress_countdown_text_layer));

  s_countdown_value = 3;
  static char s_buffer[16];
  snprintf(s_buffer, sizeof(s_buffer), "%d", s_countdown_value);
  text_layer_set_text(s_aeropress_countdown_text_layer, s_buffer);
  s_initial_countdown_timer = app_timer_register(1000, countdown_timer_callback, NULL); // MODIFIED
}

static void aeropress_step3_countdown_window_unload(Window *window) {
  if (s_initial_countdown_timer) { // MODIFIED
    app_timer_cancel(s_initial_countdown_timer); // MODIFIED
    s_initial_countdown_timer = NULL; // MODIFIED
  }
  text_layer_destroy(s_aeropress_countdown_text_layer);
  s_aeropress_countdown_text_layer = NULL;
  text_layer_destroy(s_aeropress_countdown_hint_layer);
  s_aeropress_countdown_hint_layer = NULL;
}


// --- Aeropress Step 2 Window Handlers ---
static void aeropress_step2_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Step 2 Next clicked! Going to Step 3 Countdown.");
  vibes_short_pulse();
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
                      "Non-prismo users: Invert Aeropress\n"
                      "Place filter in cap and wet.\n"
                      "Else, no invert, twist Prismo cap on\n");
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
static void aeropress_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress Next Step clicked! Going to Step 2.");
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
                      "Grind 23 grams of espresso-like coffee.\n"
                      "Prepare 300ml of 197F water.");
  text_layer_set_text_alignment(s_aeropress_text_layer, GTextAlignmentLeft);
  text_layer_set_font(s_aeropress_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
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


// --- Menu Item Callbacks ---
static void aeropress_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress selected!");
  if (!s_aeropress_window) {
    s_aeropress_window = window_create();
    window_set_window_handlers(s_aeropress_window, (WindowHandlers) {
      .load = aeropress_window_load,
      .unload = aeropress_window_unload
    });
  }
  window_stack_push(s_aeropress_window, true);
}

static void pour_over_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pour Over selected! (Implement this page)");
  // TODO: Create and push pour over window here
}

static void french_press_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "French Press selected! (Implement this page)");
  // TODO: Create and push french press window here
}

// --- Main Window Handlers ---
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_items[0] = (SimpleMenuItem) {
    .title = "Aeropress",
    .subtitle = "Your morning brew",
    .callback = aeropress_callback,
  };
  s_menu_items[1] = (SimpleMenuItem) {
    .title = "Pour Over",
    .subtitle = "For a nuanced cup",
    .callback = pour_over_callback,
  };
  s_menu_items[2] = (SimpleMenuItem) {
    .title = "French Press",
    .subtitle = "Rich and full-bodied",
    .callback = french_press_callback,
  };

  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = ARRAY_LENGTH(s_menu_items),
    .items = s_menu_items,
  };

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, ARRAY_LENGTH(s_menu_sections), NULL);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

static void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
  s_simple_menu_layer = NULL;
}

// --- App Lifecycle ---
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
  s_main_window = NULL;

  if (s_aeropress_window) {
    window_destroy(s_aeropress_window);
    s_aeropress_window = NULL;
  }
  if (s_aeropress_step2_window) {
    window_destroy(s_aeropress_step2_window);
    s_aeropress_step2_window = NULL;
  }
  if (s_aeropress_step3_countdown_window) {
    window_destroy(s_aeropress_step3_countdown_window);
    s_aeropress_step3_countdown_window = NULL;
  }
  if (s_aeropress_brew_timer_window) {
    window_destroy(s_aeropress_brew_timer_window);
    s_aeropress_brew_timer_window = NULL;
  }
  if (s_aeropress_step5_pour_stir_window) {
    window_destroy(s_aeropress_step5_pour_stir_window);
    s_aeropress_step5_pour_stir_window = NULL;
  }
  if (s_aeropress_step6_press_window) {
    window_destroy(s_aeropress_step6_press_window);
    s_aeropress_step6_press_window = NULL;
  }
  if (s_aeropress_step7_finish_window) {
    window_destroy(s_aeropress_step7_finish_window);
    s_aeropress_step7_finish_window = NULL;
  }
  // TODO: Add destruction for Pour Over and French Press windows here
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}