// game.c
#include "game.h"
#include <iostream>
    
void game_init(Game* game, HWND hWnd) {
    game->map = map_create(L"STAGE1.png", hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);
    int window_width = rect.right - rect.left;
    int window_height = rect.bottom - rect.top;
    game->camera = camera_create(window_width, window_height, game->map->width, game->map->height);
    game->character = character_create(50, 185, 10, 15); // 캐릭터 초기 위치 및 크기 설정
    kumba_init(L"enemy//kumba.png", 8, RGB(120, 182, 255));
    game->kumbas_num = game->map->num_spawn_points;
    game->kumbas = (Kumba**)malloc(game->kumbas_num * sizeof(Kumba*));

    for (int i = 0; i < game->kumbas_num; i++) {
        std::cout << game->kumbas_num << std::endl;
        game->kumbas[i] = kumba_create(
            (float)game->map->spawn_points[i].x,
            (float)game->map->spawn_points[i].y,
            20, 20, 50.0f, 0.016f);
    }
   
    game->move_direction = 0;
}

void game_update(Game* game) {
    float dt = 0.016f; 
    int dx = 0;
 
    for (int i = 0; i < game->kumbas_num; i++) {
        kumba_move(game->kumbas[i], game->map, dt);
    }

    if (game->move_direction == -1) {
        dx = game->character->is_flying ? -1 : -2; //비행중이면 속도가 느려진다.
    }
    else if (game->move_direction == 1) {
        dx = game->character->is_flying ? 1 : 2;
    }

    character_move(game->character, game->map, dx, 0);

    if (game->character->is_flying)
    {
        float new_y;
        if (game->character->jump_velocity <= 0) {
            // Ascending
            new_y = game->character->y + game->character->jump_velocity * dt;
            game->character->jump_velocity += game->character->gravity * 0.8f * dt; // Faster ascending
        }
        else {
            // Descending
            new_y = game->character->y + game->character->jump_velocity * dt;
            game->character->jump_velocity += game->character->gravity * 0.05f * dt; // Slower descending
        }
        if (!map_check_collision(game->map, game->character->x, new_y, game->character->width, game->character->height)) {
            game->character->y = new_y;
        }
    }
    
    else if (game->character->is_jumping) {
        float new_y = game->character->y + game->character->jump_velocity * dt;
        game->character->jump_velocity += game->character->gravity * dt;

        if (!map_check_collision(game->map, game->character->x, new_y, game->character->width, game->character->height)) {
            game->character->y = new_y;
        }
        else {
            game->character->is_jumping = false;
            game->character->jump_velocity = 0;
        }
    }


    camera_set(game->camera, game->character);
}


void game_render(Game* game, HDC hdc) {
    int window_width = 800;
    int window_height = 600;

    map_render(game->map, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height);
    for (int i = 0; i < game->kumbas_num; ++i) {
        kumba_render(game->kumbas[i], hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
    }
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
            game->move_direction = -1;
            break;
        case VK_RIGHT:
            game->move_direction = 1;
            break;
        case VK_SPACE:
            if (game->character->is_flying) {
                game->character->jump_velocity = -100;
                
            }
            else if (game->character->is_jumping && !game->character->is_flying) // 점프 도중 스페이스바를 누르면 비행모드 진입
            {
                    game->character->is_flying = true;
            }
            else if (!game->character->is_jumping)
            {
                game->character->jump_velocity = -300;
                game->character->is_jumping = true;
            }
            break;
        case 'E':
        case 'e': //비행 도중 e를 누르면 다시 원래대로 돌아온다.
            if (game->character->is_flying) {
                game->character->is_flying = false; 
                game->character->jump_velocity = 0; 
            }
            break;

        case 'c':
        case 'C':
            map_toggle_collision(game->map);
            break;
        }
    }
    else {
        if (wParam == VK_LEFT || wParam == VK_RIGHT) {
            game->move_direction = 0; 
        }
    }
}

