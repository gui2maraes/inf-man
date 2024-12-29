#pragma once
#include "enemy.h"
#include "level.h"
#include <stdbool.h>

/// Reads a worldfile into level data.
/// The worldfile should be a character matrix
/// with width and height compatible with the level
/// configuration.
/// Returns true on success and false on failure.
bool read_worldfile(char *filepath, Level *level, Enemy *enemies);
