#include <pebble.h>
#include "aeropress_menu.h" // Include the interface for aeropress_menu module
// ... (other headers if you have pour_over_menu.h, french_press_menu.h etc.)

// --- Main Menu UI Elements ---
static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[1];
static SimpleMenuItem s_menu_items[3]; // Still an array to hold the menu items

// ... (All other static window/layer pointers, but no aeropress specific ones)

// --- Callbacks for Menu Items (defined later) ---
// Note: We no longer declare aeropress_callback here,
//      as it's now handled by the aeropress_menu module.
static void pour_over_callback(int index, void *ctx);
static void french_press_callback(int index, void *ctx);

// ... (All window handler definitions for pour over/french press, if they were in main.c)

// --- Main Window Handlers ---
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Get the Aeropress menu item from its dedicated module
  // main_window_load is "loading" this specific menu item from the aeropress_menu module
  s_menu_items[0] = *aeropress_menu_get_item(); // Dereference the pointer to copy the struct

  // The other menu items remain if they are still defined in main.c
  // Or, they would also come from their respective modules
  s_menu_items[1] = (SimpleMenuItem) {
    .title = "Pour Over",
    .subtitle = "For a nuanced cup",
    .callback = pour_over_callback, // Assuming pour_over_callback is still in main.c or its own module
  };
  s_menu_items[2] = (SimpleMenuItem) {
    .title = "French Press",
    .subtitle = "Rich and full-bodied",
    .callback = french_press_callback, // Assuming french_press_callback is still in main.c or its own module
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

  // Now, in deinit, you'd call deinitialization functions from your modules
  // if they manage persistent resources that need explicit cleanup.
  // Example:
  // aeropress_brew_process_deinit(); // To clean up any Aeropress-related windows that might still be in memory
  // And so on for other modules.

  // Note: The window_destroy calls you had previously for individual windows
  // are now the responsibility of their respective modules' deinit functions,
  // or handled by window_unload when windows are popped from the stack.
  // The `deinit` here ensures everything is cleaned up even if the app exits
  // abruptly or doesn't pop all windows.
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}