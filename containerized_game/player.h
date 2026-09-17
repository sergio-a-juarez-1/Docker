// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

#define STARTING_POSITION_Y 550 // Near bottom of a 600x600 screen
#define MOVE_DISTANCE 10
#define FINISH_LINE_Y 50

typedef struct {
    Vector2 position;
    int size;
    Color color;
} Player;

void InitPlayer(Player *player);
void MovePlayerUp(Player *player);
bool IsPlayerAtFinishLine(Player *player);
void ResetPlayer(Player *player);
void DrawPlayer(Player *player);

#endif