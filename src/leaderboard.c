#include "leaderboard.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static void Leaderboard_dummy(Leaderboard *leaderboard) {
  *leaderboard = (Leaderboard){0};
  PlayerRecord *players = leaderboard->players;
  /// If the number of players is changed in the configuration,
  /// abort immediately as to not overflow the buffer.
  if (LEADERBOARD_NUM != 5) {
    error_out("ADD OR REMOVE DUMMY PLAYERS TO THIS FUNCTION");
    return;
  }
  strncpy(players[0].name, "DESTROYER", PLAYER_NAME_MAX);
  strncpy(players[1].name, "mega_gamer", PLAYER_NAME_MAX);
  strncpy(players[2].name, "D1ddy", PLAYER_NAME_MAX);
  strncpy(players[3].name, "Felipe", PLAYER_NAME_MAX);
  strncpy(players[4].name, "Guilherme", PLAYER_NAME_MAX);
  players[0].score = 100;
  players[1].score = 75;
  players[2].score = 50;
  players[3].score = 25;
  players[4].score = 10;
}

bool Leaderboard_save(Leaderboard *leaderboard, char *path) {
  FILE *f = fopen(path, "wb");
  int out = true;
  if (!f) {
    perror("error saving leaderboard");
    return false;
  }
  if (fwrite(leaderboard, sizeof(Leaderboard), 1, f) != 1) {
    out = false;
  }
  fclose(f);
  return out;
}

bool Leaderboard_init(Leaderboard *leaderboard, char *path) {
  FILE *f = fopen(path, "rb");
  int out = true;
  if (!f) {
    // if the file doesn't exist, we create a dummy leaderboard
    // and save it to the file.
    if (errno == ENOENT) {
      Leaderboard_dummy(leaderboard);
      return Leaderboard_save(leaderboard, path);
    }
    perror("error opening leaderboard file");
    return false;
  }

  if (fread(leaderboard, sizeof(Leaderboard), 1, f) != 1) {
    out = false;
  }
  fclose(f);
  return out;
}

void Leaderboard_update(Leaderboard *leaderboard, PlayerRecord *record) {

  for (int i = 0; i < LEADERBOARD_NUM; ++i) {
    // once found the new record's spot,
    if (record->score > leaderboard->players[i].score) {
      // we push down the remaining scores

      for (int j = LEADERBOARD_NUM - 1; j > i; --j) {
        leaderboard->players[j] = leaderboard->players[j - 1];
      }
      // and place the new record there.
      leaderboard->players[i] = *record;
      return;
    }
  }
}
bool Leaderboard_is_contender(Leaderboard *leaderboard, int score) {
  for (int i = 0; i < LEADERBOARD_NUM; ++i) {
    if (score > leaderboard->players[i].score) {
      return true;
    }
  }
  return false;
}