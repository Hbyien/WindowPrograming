#include "coin.h"
#include <stdlib.h>
#include <stdio.h> // ������� ���� ���

CImage coin_image;
int coin_sprite_width;
int coin_sprite_height;
int coin_total_frames;    // �� ������ ��

void coin_init(const wchar_t* image_path, int frames, COLORREF transparent_color) {
    coin_total_frames = frames;
    HRESULT hr = coin_image.Load(image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load coin image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    // ����� ����
    coin_image.SetTransparentColor(transparent_color);
    coin_sprite_width = coin_image.GetWidth() / coin_total_frames;
    coin_sprite_height = coin_image.GetHeight();
}

Coin* coin_create(float x, float y, int width, int height)
{
    Coin* coin = (Coin*)malloc(sizeof(Coin));
    coin->x = x;
    coin->y = y;
    coin->width = width;
    coin->height = height;

    coin->current_frame = 0;
    coin->coin_total_frames = coin_total_frames; // ���� ���� ���
    coin->animation_speed = 0.1f; // �ִϸ��̼� �ӵ�
    coin->animation_timer = 0.0f;
    return coin;
}

void coin_destroy(Coin* coin) {
    free(coin);
}

int coin_check_collision(Coin* coin, Map* map) {
    
    return map_check_collision(map, coin->x, coin->y, coin->width, coin->height);
}

void coin_render(Coin* coin, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height) {
    float scale_x = (float)window_width / map_height;
    float scale_y = (float)window_height / map_height;

    int render_x = (int)((coin->x - camera_x) * scale_x);
    int render_y = (int)((coin->y - camera_y) * scale_y);
    int render_width = (int)(coin->width * scale_x);
    int render_height = (int)(coin->height * scale_y);

    int frame_x = coin->current_frame * coin_sprite_width;

    // �޸� DC ���� �� ����
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, coin_image);

    // ��������Ʈ�� ���� �������� �׸���
    TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, coin_sprite_width, coin_sprite_height, RGB(136, 191, 157)); // ���� ���� ����

    // ���ҽ� ����
    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}


void coin_update(Coin* coin, float dt) {
    coin->animation_timer += dt;
    if (coin->animation_timer >= coin->animation_speed) {
        coin->animation_timer = 0.0f;
        coin->current_frame = (coin->current_frame + 1) % coin->coin_total_frames;
    }
}
