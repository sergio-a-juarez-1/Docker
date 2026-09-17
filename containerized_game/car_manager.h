// car_manager.h
#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include "raylib.h"
#include "player.h"

#define MAX_CARS 100
#define STARTING_MOVE_DISTANCE 5
#define MOVE_INCREMENT 3

typedef struct {
    Vector2 position;
    int width;
    int height;
    Color color;
} Car;

typedef struct {
    Car cars[MAX_CARS];
    int car_count;
    int car_speed;
} CarManager;

void InitCarManager(CarManager *cm);
void CreateCar(CarManager *cm);
void MoveCars(CarManager *cm);
void LevelUpCars(CarManager *cm);
void DrawCars(CarManager *cm);
bool CheckCarCollisions(CarManager *cm, Player *player);

#endif