// scoreboard.c
#include "scoreboard.h"
#include "raylib.h"
#include <stdio.h>

void InitScoreboard(Scoreboard *sb) {
    sb->level = 1;
    sb->game_over = false;
}

void IncreaseLevel(Scoreboard *sb) {
    sb->level++;
}

void DrawScoreboard(Scoreboard *sb) {
    if (!sb->game_over) {
        // Render Level top left
        char levelText[20];
        sprintf(levelText, "Level: %d", sb->level);
        DrawText(levelText, 20, 20, 24, DARKGRAY);
    } else {
        // Render Game Over text centered
        DrawText("GAME OVER 💀", 300 - MeasureText("GAME OVER 💀", 32)/2, 300 - 16, 32, RED);
    }
}