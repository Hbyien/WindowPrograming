#include "kumba.h"
#include <stdlib.h>
#include <stdio.h> // ������� ���� ���

CImage kumba_image;
int sprite_width;
int sprite_height;
int total_frames;    // �� ������ ��

void kumba_init(const wchar_t* image_path, int frames, COLORREF transparent_color) {
    total_frames = frames;
    HRESULT hr = kumba_image.Load(image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load Kumba image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    // ����� ����
    kumba_image.SetTransparentColor(transparent_color);
    sprite_width = kumba_image.GetWidth() / total_frames;
    sprite_height = kumba_image.GetHeight();
}

Kumba* kumba_create(float x, float y, int width, int height, float velocity_x, float velocity_y) {
    Kumba* kumba = (Kumba*)malloc(sizeof(Kumba));
    kumba->x = x;
    kumba->y = y;
    kumba->width = width;
    kumba->height = height;
    kumba->velocity_x = velocity_x;
    kumba->velocity_y = 0.016f; // �ʱ� ���� �ӵ�
    kumba->gravity = 800.0f; // �߷� ����
    kumba->is_alive = true;
    kumba->current_frame = 0;
    kumba->total_frames = total_frames; // ���� ���� ���
    kumba->animation_speed = 0.1f; // �ִϸ��̼� �ӵ�
    kumba->animation_timer = 0.0f;
    return kumba;
}

void kumba_destroy(Kumba* kumba) {
    free(kumba);
}

void kumba_move(Kumba* kumba, Map* map, float dt) {
    if (!kumba->is_alive) return;

    // ���� �̵�
    float new_x = kumba->x + kumba->velocity_x * dt;
    int collision_index = -1;

    if (!map_check_collision(map, (int)new_x, (int)kumba->y, kumba->width, kumba->height, &collision_index) && !check_cliff(map, (int)new_x)) {
        kumba->x = new_x;
    }
    else {
        kumba->velocity_x = -kumba->velocity_x;
    }

    // �߷¿� ���� ���� �̵�
    float new_y = kumba->y + kumba->velocity_y * dt;
    kumba->velocity_y += kumba->gravity * dt;

    if (!map_check_collision(map, (int)kumba->x, (int)new_y, kumba->width, kumba->height, &collision_index)) {
        kumba->y = new_y;
    }
    else {
        kumba->velocity_y = 0; // �浹 �� �ӵ� �ʱ�ȭ
    }

    kumba->animation_timer += dt;
    if (kumba->animation_timer >= kumba->animation_speed) {
        kumba->current_frame = (kumba->current_frame + 1) % kumba->total_frames;
        kumba->animation_timer = 0.0f;
    }
}

int kumba_check_collision(Kumba* kumba, Map* map) {
    if (!kumba->is_alive) return 0;
    int collision_index = -1;
    return map_check_collision(map, (int)kumba->x, (int)kumba->y, kumba->width, kumba->height, &collision_index);
}

void kumba_render(Kumba* kumba, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height) {
    if (!kumba->is_alive) return;

    float scale_x = (float)window_width / map_height;
    float scale_y = (float)window_height / map_height;

    int render_x = (int)((kumba->x - camera_x) * scale_x);
    int render_y = (int)((kumba->y - camera_y) * scale_y);
    int render_width = (int)(kumba->width * scale_x);
    int render_height = (int)(kumba->height * scale_y);

    int frame_x = kumba->current_frame * sprite_width;

    // �޸� DC ���� �� ����
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, kumba_image);

    // ��������Ʈ�� ���� �������� �׸���
    TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(120, 182, 255)); // ���� ���� ����
    // ���ҽ� ����
    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}

bool character_kumba_check_collision(Character* character, Kumba* kumba) {
    RECT character_rect = { character->x, character->y, character->x + character->width, character->y + character->height };
    RECT kumba_rect = { kumba->x, kumba->y, kumba->x + kumba->width, kumba->y + kumba->height };
    RECT intersection;
    return IntersectRect(&intersection, &character_rect, &kumba_rect);
}
