#pragma once

#include <pebble.h>

// This function provides the SimpleMenuItem for Aeropress to other modules (like main.c)
// It returns a pointer to the static SimpleMenuItem defined within aeropress_menu.c
SimpleMenuItem* aeropress_menu_get_item();

// If you decide aeropress_menu needs an init/deinit (less common for just a menu item provider)
// void aeropress_menu_init();
// void aeropress_menu_deinit();