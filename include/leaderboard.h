#pragma once

#include "config.h"
#include <stdbool.h>

/// Player information contained in the leaderboard.
typedef struct PlayerRecord {
  char name[PLAYER_NAME_BUF];
  int score;
} PlayerRecord;

/// Leaderboard containing top N players, in
/// descending score order.
typedef struct Leaderboard {
  PlayerRecord players[LEADERBOARD_NUM];
} Leaderboard;

/// Reads and initialized a Leaderboard structure from
/// a binary file specified in path.
/// Returns true on success and false otherwise.
bool Leaderboard_init(Leaderboard *leaderboard, char *path);

/// Writes a Leaderboard to a binary file specified in path.
/// Returns true on success and false otherwise.
bool Leaderboard_save(Leaderboard *leaderboard, char *path);

/// Adds a PlayerRecord to a Leaderboard, according to its score.
void Leaderboard_update(Leaderboard *leaderboard, PlayerRecord *record);

/// Returns true if a score is high enough to be on the leaderboard.
bool Leaderboard_is_contender(Leaderboard *leaderboard, int score);