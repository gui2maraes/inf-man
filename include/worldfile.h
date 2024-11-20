#pragma once
#include "level.h"

/// Reads a worldfile into level data.
/// The worldfile should be a character matrix
/// with width and height compatible with the level
/// configuration.
int read_worldfile(char *filepath, Level *level);
