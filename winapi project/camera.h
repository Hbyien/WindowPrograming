// camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "map.h"
#include "character.h"
typedef struct {
    int x;
    int y;
    int window_width;
    int window_height;
    int map_width;
    int map_height;
} Camera;

Camera* camera_create(int window_width, int window_height, int map_width, int map_height);
void camera_destroy(Camera* camera);
void camera_move_left(Camera* camera);
void camera_move_right(Camera* camera);
void camera_move_up(Camera* camera);
void camera_move_down(Camera* camera);
int camera_get_x(Camera* camera);
int camera_get_y(Camera* camera);
void camera_set(Camera* camera,Character* character);
#endif // CAMERA_H
