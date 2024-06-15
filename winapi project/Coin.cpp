#include "coin.h"
#include "character.h"
#include <stdlib.h>

Coin* coin_create(int x, int y, int width, int height, const wchar_t* image_path, int total_frames, float animation_speed) {
    Coin* coin = (Coin*)malloc(sizeof(Coin));
    coin->x = x;
    coin->y = y;
    coin->width = width;
    coin->height = height;
    coin->current_frame = 0;
    coin->total_frames = total_frames;
    coin->animation_speed = animation_speed;
    coin->animation_timer = 0.0f;

    HRESULT hr = coin->image.Load(image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load coin image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    coin->image.SetTransparentColor(RGB(255, 255, 255)); // 흰색 부분 투명 처리

    return coin;
}

void coin_destroy(Coin* coin) {
    coin->image.Destroy();
    free(coin);
}

void coin_update(Coin* coin, float dt) {
    coin->animation_timer += dt;
    if (coin->animation_timer >= coin->animation_speed) {
        coin->current_frame = (coin->current_frame + 1) % coin->total_frames;
        coin->animation_timer = 0.0f;
    }
}

void coin_render(Coin* coin, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height) {
    float scale_x = (float)window_width / map_height;
    float scale_y = (float)window_height / map_height;

    int render_x = (int)((coin->x - camera_x) * scale_x);
    int render_y = (int)((coin->y - camera_y) * scale_y);
    int render_width = (int)(coin->width * scale_x);
    int render_height = (int)(coin->height * scale_y);

    int sprite_width = coin->image.GetWidth() / coin->total_frames;
    int sprite_height = coin->image.GetHeight();
    int frame_x = coin->current_frame * sprite_width;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, coin->image);

    TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}

bool coin_character_check_collision(Character* character, Coin* coin) {
    RECT character_rect = { character->x, character->y, character->x + character->width, character->y + character->height };
    RECT coin_rect = { coin->x, coin->y, coin->x + coin->width, coin->y + coin->height };
    RECT intersection;
    return IntersectRect(&intersection, &character_rect, &coin_rect);
}
