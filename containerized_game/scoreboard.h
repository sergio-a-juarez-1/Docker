// scoreboard.h
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdbool.h>

typedef struct {
    int level;
    bool game_over;
} Scoreboard;

void InitScoreboard(Scoreboard *sb);
void IncreaseLevel(Scoreboard *sb);
void DrawScoreboard(Scoreboard *sb);

#endif