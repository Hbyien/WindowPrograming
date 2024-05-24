// game.c
#include "game.h"

void game_init(Game* game, HWND hWnd) {
    game->map = map_create(L"STAGE1.png", hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);
    int window_width = rect.right - rect.left;
    int window_height = rect.bottom - rect.top;
    game->camera = camera_create(window_width, window_height, game->map->width, game->map->height);
    game->character = character_create(100, 150, 5, 5); // 캐릭터 초기 위치 및 크기 설정
}

void game_update(Game* game) {
    camera_set(game->camera,game->character);
    
}

void game_render(Game* game, HDC hdc) {
    int window_width = 800;
    int window_height = 600;

    map_render(game->map, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height);

    // 캐릭터 렌더링
    float scale_x = (float)window_width / game->map->height;
    float scale_y = (float)window_height / game->map->height;

    int render_x = (int)((game->character->x - camera_get_x(game->camera)) * scale_x);
    int render_y = (int)((game->character->y - camera_get_y(game->camera)) * scale_y);
    int render_width = (int)(game->character->width * scale_x);
    int render_height = (int)(game->character->height * scale_y);

    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    Rectangle(hdc, render_x, render_y, render_x + render_width, render_y + render_height);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}

void game_handle_input(Game* game, WPARAM wParam, int key_down) {
    if (key_down) {
        int dx = 0;
        int dy = 0;
        switch (wParam) {
        case VK_LEFT:
            dx = -5;
            break;
        case VK_RIGHT:
            dx = 5;
            break;
        case VK_UP:
            dy = -5;
            break;
        case VK_DOWN:
            dy = 5;
            break;
        case 'c':
        case 'C':
            map_toggle_collision(game->map);
            break;
        }
        character_move(game->character, game->map,dx, dy);
    }
}
