// player.c
#include "player.h"

void InitPlayer(Player *player) {
    player->position = (Vector2){ 300, STARTING_POSITION_Y }; // Centered horizontally
    player->size = 20;
    player->color = LIME;
}

void MovePlayerUp(Player *player) {
    player->position.y -= MOVE_DISTANCE;
}

bool IsPlayerAtFinishLine(Player *player) {
    if (player->position.y < FINISH_LINE_Y) {
        ResetPlayer(player);
        return true;
    }
    return false;
}

void ResetPlayer(Player *player) {
    player->position = (Vector2){ 300, STARTING_POSITION_Y };
}

void DrawPlayer(Player *player) {
    // Draws a triangle representation of the turtle facing up
    Vector2 v1 = { player->position.x, player->position.y - player->size };
    Vector2 v2 = { player->position.x - player->size, player->position.y + player->size };
    Vector2 v3 = { player->position.x + player->size, player->position.y + player->size };
    DrawTriangle(v1, v2, v3, player->color);
}