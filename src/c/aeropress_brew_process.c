#include "aeropress_brew_process.h"
// Include other Pebble headers as needed for UI elements and timers

// --- Aeropress Detail UI Elements (Step 1) ---
static Window *s_aeropress_window;
static TextLayer *s_aeropress_text_layer;
static TextLayer *s_aeropress_select_hint_layer;

// --- Aeropress Step 2 UI Elements ---
static Window *s_aeropress_step2_window;
static TextLayer *s_aeropress_step2_text_layer;
static TextLayer *s_aeropress_step2_select_hint_layer;

// ... and so on, all the static variables and functions related to Aeropress steps.

// --- Aeropress Step 7 (Finish) Window Handlers ---
// ... (All step 7 code from main.c)
// --- Aeropress Step 6 (Press) Window Handlers ---
// ... (All step 6 code from main.c)
// --- Aeropress Step 5 (Pour & Stir) Window Handlers ---
// ... (All step 5 code from main.c)
// --- Aeropress Step 4 (Bloom Timer) Window Handlers ---
// ... (All step 4 code from main.c)
// --- Aeropress Step 3 (Initial Countdown) Window Handlers ---
// ... (All step 3 code from main.c)
// --- Aeropress Step 2 Window Handlers ---
// ... (All step 2 code from main.c)
// --- Aeropress Detail Window Handlers (Step 1) ---
// ... (All step 1 code from main.c)

// The public function to start the process
void aeropress_brew_process_start() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Starting Aeropress brew process module.");
  if (!s_aeropress_window) {
    s_aeropress_window = window_create();
    window_set_window_handlers(s_aeropress_window, (WindowHandlers) {
      .load = aeropress_window_load,
      .unload = aeropress_window_unload
    });
  }
  window_stack_push(s_aeropress_window, true);
}

// Deinit function for this module
void aeropress_brew_process_deinit() {
    // Only destroy windows if they haven't been destroyed by their unload handlers
    // Otherwise, this should be safe as window_destroy handles NULL
    if (s_aeropress_window) window_destroy(s_aeropress_window);
    if (s_aeropress_step2_window) window_destroy(s_aeropress_step2_window);
    // ... and so on for all Aeropress windows
}