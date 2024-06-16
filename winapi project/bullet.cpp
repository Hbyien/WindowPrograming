#include "bullet.h"
#include <iostream>



Bullet* bullet_create(int x, int y, float speed) {
    Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));
    bullet->x = x;
    bullet->y = y;
    bullet->width = 10;  // 총알 크기
    bullet->height = 10;
    bullet->speed = speed; // 속도를 조정 (속도를 낮추고 싶으면 2로 나누지 않도록 변경)
    bullet->active = true;
    bullet->exploding = false;
    bullet->explosion_timer = 0.0f;
    bullet->explosion_duration = 0.1f; // 폭발 애니메이션 지속 시간

    HRESULT hr = bullet->flying_image.Load(BULLET_FLYING_IMAGE_PATH);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load bullet flying image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    hr = bullet->explosion_image.Load(BULLET_EXPLOSION_IMAGE_PATH);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load bullet explosion image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }

    return bullet;
}

void bullet_destroy(Bullet* bullet) {
    bullet->flying_image.Destroy();
    bullet->explosion_image.Destroy();
    free(bullet);
}

bool check_collision_with_wall(Bullet* bullet, Map* map) {
    return map_check_collision(map, bullet->x, bullet->y, bullet->width, bullet->height, NULL);
}

void bullet_update(Bullet* bullet, float dt, Map* map) {
    if (!bullet->active) return;

    if (bullet->exploding) {
        bullet->explosion_timer += dt;
        if (bullet->explosion_timer >= bullet->explosion_duration) {
            bullet->active = false;
        }
    }
    else {
        bullet->x += bullet->speed; // 속도를 낮추기 위해 dt를 곱해줍니다.

        if (check_collision_with_wall(bullet, map)) {
            bullet->exploding = true;
            bullet->explosion_timer = 0.0f;
        }
    }
}

void bullet_render(Bullet* bullet, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height) {
    if (!bullet->active) return;

    float scale_x = (float)window_width / map_height;
    float scale_y = (float)window_height / map_height;

    int render_x = (int)((bullet->x - camera_x) * scale_x);
    int render_y = (int)((bullet->y - camera_y) * scale_y);
    int render_width = (int)(bullet->width * scale_x);
    int render_height = (int)(bullet->height * scale_y);

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap;

    if (bullet->exploding) {
        oldBitmap = (HBITMAP)SelectObject(memDC, bullet->explosion_image);
        TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, 0, 0, bullet->explosion_image.GetWidth(), bullet->explosion_image.GetHeight(), RGB(255, 255, 255));
    }
    else {
        oldBitmap = (HBITMAP)SelectObject(memDC, bullet->flying_image);
        TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, 0, 0, bullet->flying_image.GetWidth(), bullet->flying_image.GetHeight(), RGB(255, 255, 255));
    }

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}
