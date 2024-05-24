// camera.c
#include "camera.h"
#include <stdlib.h>

Camera* camera_create(int window_width, int window_height, int map_width, int map_height) {
    Camera* camera = (Camera*)malloc(sizeof(Camera));
    camera->x = 0;
    camera->y = 0;
    camera->window_width = window_width;
    camera->window_height = window_height;
    camera->map_width = map_width;
    camera->map_height = map_height;
    return camera;
}

void camera_destroy(Camera* camera) {
    free(camera);
}

void camera_move_left(Camera* camera) {
    camera->x -= 10;
    if (camera->x < 0) {
        camera->x = 0;
    }
}

void camera_move_right(Camera* camera) {
    camera->x += 10;
    if (camera->x > camera->map_width - camera->window_width) {
        camera->x = camera->map_width - camera->window_width;
    }
}

void camera_move_up(Camera* camera) {
    camera->y -= 10;
    if (camera->y < 0) {
        camera->y = 0;
    }
}

void camera_move_down(Camera* camera) {
    camera->y += 10;
    if (camera->y > camera->map_height - camera->window_height) {
        camera->y = camera->map_height - camera->window_height;
    }
}

int camera_get_x(Camera* camera) {
    return camera->x;
}

int camera_get_y(Camera* camera) {
    return camera->y;
}

void camera_set(Camera* camera, Character* character) {
    // 카메라의 x 좌표를 캐릭터의 x 좌표를 기준으로 설정하여 화면 중앙에 캐릭터가 오도록 설정
    camera->x = character->x - camera->map_height / 2;

    // 카메라가 맵의 경계를 벗어나지 않도록 설정
    if (camera->x < 0) {
        camera->x = 0;
    }
    else if (camera->x > camera->map_width - camera->map_height) {
        camera->x = camera->map_width - camera->map_height;
    }

    // y 좌표는 고정된 값으로 유지
    camera->y = 0;
}