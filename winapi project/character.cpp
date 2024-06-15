#include "character.h"
#include "map.h"
#include <stdlib.h>

Character* character_create(int x, int y, int width, int height) {
    Character* character = (Character*)malloc(sizeof(Character));
    character->x = x;
    character->y = y;
    character->width = width;
    character->height = height;

    character->jump_velocity = 0.f;
    character->gravity = 800.f;
    character->is_jumping = false;
    character->is_flying = false;
    return character;
}

void character_destroy(Character* character) {
    free(character);
}

void character_move(Character* character, Map* map, int dx, int dy) {
    // �ӽ� ��ġ ���
    int new_x = character->x + dx;
    float new_y = character->y + dy;

    // �ӽ� ��ġ������ �浹 �˻�
    if (!map_check_collision(map, new_x, character->y, character->width, character->height)) {
        // �浹�� ���� ��� ���� ��ġ ������Ʈ
        character->x = new_x;
    }

    if (!map_check_collision(map, character->x, new_y, character->width, character->height)) {
        // �浹�� ���� ��� ���� ��ġ ������Ʈ
        character->y = new_y;
    }
    else {
        // �浹�� �ִ� ��� ���� ���� ����
        character->is_jumping = false;
        character->jump_velocity = 0;
    }
    
}

void character_render(Character* character, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height) {
    float scale_x = (float)window_width / map_height;
    float scale_y = (float)window_height / map_height;

    int render_x = (int)((character->x - camera_x) * scale_x);
    int render_y = (int)((character->y - camera_y) * scale_y);
    int render_width = (int)(character->width * scale_x);
    int render_height = (int)(character->height * scale_y);

    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    Rectangle(hdc, render_x, render_y, render_x + render_width, render_y + render_height);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}