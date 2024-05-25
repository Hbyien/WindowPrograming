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
    // 임시 위치 계산
    int new_x = character->x + dx;
    float new_y = character->y + dy;

    // 임시 위치에서의 충돌 검사
    if (!map_check_collision(map, new_x, character->y, character->width, character->height)) {
        // 충돌이 없을 경우 수평 위치 업데이트
        character->x = new_x;
    }

    if (!map_check_collision(map, character->x, new_y, character->width, character->height)) {
        // 충돌이 없을 경우 수직 위치 업데이트
        character->y = new_y;
    }
    else {
        // 충돌이 있는 경우 점프 상태 해제
        character->is_jumping = false;
        character->jump_velocity = 0;
    }
}

int character_check_collision(Character* character, Map* map) {
    return map_check_collision(map, character->x, character->y, character->width, character->height);
}
