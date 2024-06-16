#ifndef BULLET_H
#define BULLET_H

#include <windows.h>
#include <vector>
#include <atlimage.h>
#include "map.h"

#define BULLET_FLYING_IMAGE_PATH L"BaseImage//fire.png"
#define BULLET_EXPLOSION_IMAGE_PATH L"BaseImage//explosion.png"


typedef struct Bullet {
    int x, y;
    int width, height;
    float speed;
    bool active;
    CImage flying_image;
    CImage explosion_image;
    bool exploding;
    float explosion_timer;
    float explosion_duration;
    std::vector<Bullet*> bullets;
} Bullet;

Bullet* bullet_create(int x, int y, float speed);
void bullet_destroy(Bullet* bullet);
void bullet_update(Bullet* bullet, float dt, Map* map);
void bullet_render(Bullet* bullet, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);

#endif // BULLET_H
