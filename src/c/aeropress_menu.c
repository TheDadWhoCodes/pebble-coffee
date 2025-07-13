#include "aeropress_menu.h"
#include "aeropress_brew_process.h" // We still need this to kick off the brew process

// The internal callback that will be triggered when the Aeropress menu item is selected
static void prv_aeropress_menu_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Aeropress selected from menu (via prv_aeropress_menu_callback)!");
  vibes_short_pulse(); // You might want to add a vibration here for menu selection confirmation
  aeropress_brew_process_start(); // Call the function from your brew process module
}

// The static SimpleMenuItem that holds the details for the Aeropress entry
// Note: Its callback points to the internal static function 'prv_aeropress_menu_callback'
static SimpleMenuItem s_aeropress_menu_item = {
  .title = "Aeropress",
  .subtitle = "Your morning brew",
  .callback = prv_aeropress_menu_callback,
};

// The public function that main.c will call to get this menu item
SimpleMenuItem* aeropress_menu_get_item() {
    return &s_aeropress_menu_item;
}