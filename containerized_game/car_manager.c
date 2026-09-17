// car_manager.c
#include "car_manager.h"
#include <stdlib.h>

Color colors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE };

void InitCarManager(CarManager *cm) {
    cm->car_count = 0;
    cm->car_speed = STARTING_MOVE_DISTANCE;
}

void CreateCar(CarManager *cm) {
    // 1-in-6 chance per tick to generate a car
    if (GetRandomValue(1, 6) == 1 && cm->car_count < MAX_CARS) {
        Car new_car;
        new_car.width = 60;   // Matches stretch_len from turtle
        new_car.height = 30;  // Matches stretch_wid
        new_car.position.x = 650; // Starts just off-screen to the right
        new_car.position.y = GetRandomValue(80, 500); // Random lane
        new_car.color = colors[GetRandomValue(0, 5)];
        
        cm->cars[cm->car_count] = new_car;
        cm->car_count++;
    }
}

void MoveCars(CarManager *cm) {
    for (int i = 0; i < cm->car_count; i++) {
        cm->cars[i].position.x -= cm->car_speed;
    }
}

void LevelUpCars(CarManager *cm) {
    cm->car_speed += MOVE_INCREMENT;
}

void DrawCars(CarManager *cm) {
    for (int i = 0; i < cm->car_count; i++) {
        DrawRectangle(cm->cars[i].position.x, cm->cars[i].position.y, 
                      cm->cars[i].width, cm->cars[i].height, cm->cars[i].color);
    }
}

bool CheckCarCollisions(CarManager *cm, Player *player) {
    // Simple Bounding Box (AABB) Collision Detection
    Rectangle playerRec = { player->position.x - player->size, player->position.y - player->size, 
                            player->size * 2, player->size * 2 };
    
    for (int i = 0; i < cm->car_count; i++) {
        Rectangle carRec = { cm->cars[i].position.x, cm->cars[i].position.y, 
                             cm->cars[i].width, cm->cars[i].height };
        
        if (CheckCollisionRecs(playerRec, carRec)) {
            return true;
        }
    }
    return false;
}
