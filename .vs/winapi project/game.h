#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include "camera.h"
#include "map.h"
#include "character.h"

typedef struct {
    Map* map;
    Camera* camera;
    Character* character;
    int move_direction; //-1 ���� 1������ 0 ���ڸ�
} Game;

void game_init(Game* game, HWND hWnd);
void game_update(Game* game);
void game_render(Game* game, HDC hdc);
void game_handle_input(Game* game, WPARAM wParam, int key_down);

#endif // GAME_H
