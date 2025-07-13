#pragma once

#include <pebble.h>

// Public function to start the Aeropress brewing process
void aeropress_brew_process_start();

// Functions to destroy resources if they're persistent
void aeropress_brew_process_deinit();