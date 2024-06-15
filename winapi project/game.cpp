// game.c
#include "game.h"
#include <iostream>
    
void game_init(Game* game, HWND hWnd) {

    game->game_state = STATE_LOGO;
    LoadLogo();
    LoadMap();
    { // map 1 초기화 시 
        game->map = map_create(L"BaseImage//STAGE1.png", hWnd);
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

}

void game_update(Game* game) {
    switch (game->game_state) {
    case STATE_LOGO:
        break;
    case STATE_MAP:
        break;
    case STATE_WORLD1:
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
}


void game_render(Game* game, HDC hdc) {
    int window_width = 800;
    int window_height = 600;
    switch (game->game_state){
        case STATE_LOGO: 
        RenderLogo(hdc);
        break;
        case STATE_MAP:
        RenderMap(hdc);
        break;
        case STATE_WORLD1: 
        map_render(game->map, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height);
        for (int i = 0; i < game->kumbas_num; ++i) {
            kumba_render(game->kumbas[i], hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
        }
        character_render(game->character, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
        break;
        
    }
    
}

void game_handle_input(Game* game, WPARAM wParam ,LPARAM lParam, int key_down) {
    switch (game->game_state) {
    case STATE_LOGO: {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        std::cout << xPos << " " << yPos;
        if (xPos >= 530 && xPos <= 780 && yPos >= 350 && yPos <= 410) {
            game->game_state = STATE_MAP;
        }
        else if (xPos >= 530 && xPos <= 780 && yPos >= 430 && yPos <= 490) {
            PostQuitMessage(0);
        }
        break;
    }
    case STATE_MAP:
        if (key_down) {
            switch (wParam) {
            case VK_LEFT:
                MoveMapPlayerLeft();
                break;
            case VK_RIGHT:
                MoveMapPlayerRight();
                break;
            case VK_RETURN: {
                int world_num = 0;
                world_num = CheckEnterKey();
                std::cout << world_num << std::endl;
                if (world_num == 1) {
                    game->game_state = STATE_WORLD1;
                } else if (world_num == 2) {
                    game->game_state = STATE_WORLD2;
                } else if (world_num == 3) {
                    game->game_state = STATE_WORLD3;
                }
                break;
            }
            }

        }

    case STATE_WORLD1:
    {
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
        break;
    }
    }

}

