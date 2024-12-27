#pragma once

#include "config.h"

/// Player information contained in the leaderboard.
typedef struct PlayerRecord {
  char name[PLAYER_NAME_MAX + 1];
  int score;
} PlayerRecord;

/// Leaderboard containing top N players, in
/// descending score order.
typedef struct Leaderboard {
  PlayerRecord players[LEADERBOARD_NUM];
} Leaderboard;

/// Reads and initialized a Leaderboard structure from
/// a binary file specified in path.
/// Returns 1 on success and 0 otherwise.
int Leaderboard_init(Leaderboard *leaderboard, char *path);

/// Writes a Leaderboard to a binary file specified in path.
/// Returns 1 on success and 0 otherwise.
int Leaderboard_save(Leaderboard *leaderboard, char *path);

/// Adds a PlayerRecord to a Leaderboard, according to its score.
void Leaderboard_update(Leaderboard *leaderboard, PlayerRecord *record);
