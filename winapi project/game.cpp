// game.c
#include "game.h"
#include <iostream>
    
void game_init(Game* game, HWND hWnd) {

    game->game_state = STATE_LOGO;
    game->timer = 0;
    game->last_update_time = GetTickCount();

    LoadLogo();
    LoadMap();

    { // map 1 초기화 
        game->map = map_create(L"BaseImage//STAGE1.png", hWnd);
        RECT rect;
        GetClientRect(hWnd, &rect);
        int window_width = rect.right - rect.left;
        int window_height = rect.bottom - rect.top;
        game->camera = camera_create(window_width, window_height, game->map->width, game->map->height);
        game->character = character_create(50, 185, 15, 20); // 캐릭터 초기 위치 및 크기 설정
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

        coin_init(L"coin1.png", 4, RGB(136,191,157));
        game->coins_num = game->map->coin_num_spawn_points;
        game->coin = (Coin**)malloc(game->coins_num * sizeof(Coin*));
        for (int i = 0; i < game->coins_num; i++) {
            std::cout << game->coins_num << std::endl;
            game->coin[i] = coin_create(
                (float)game->map->coin_spawn_points[i].x,
                (float)game->map->coin_spawn_points[i].y,
                20, 20);
        }
    }

}
void game_update(Game* game) {
    DWORD current_time = GetTickCount();
    switch (game->game_state) {
    case STATE_LOGO:
        break;
    case STATE_MAP:
        break;
    case STATE_WORLD1:
        float dt = 0.016f;
        for (int i = 0; i < game->kumbas_num; i++) {
            kumba_move(game->kumbas[i], game->map, dt);
        }
        for (int i = 0; i < game->coins_num; i++) {
            coin_update(game->coin[i], dt); // Update coin animation
        }
        character_update(game->character, game->map, dt);
        camera_set(game->camera, game->character);
        break;
    }

    // Update timer every second when in STATE_WORLD1
    if (game->game_state == STATE_WORLD1) {
        if (current_time - game->last_update_time >= 1000) {
            game->timer++;
            game->last_update_time = current_time;
        }
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
        {
            map_render(game->map, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height);
            for (int i = 0; i < game->kumbas_num; ++i) {
                kumba_render(game->kumbas[i], hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
            }
            for (int i = 0; i < game->coins_num; ++i) {
                coin_render(game->coin[i], hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
            }

            character_render(game->character, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);

            // 타이머 그리기
            std::wstring timer_text = L"Timer: ";
            timer_text += (game->timer < 100) ? L"0" : L"";
            timer_text += (game->timer < 10) ? L"0" : L"";
            timer_text += std::to_wstring(game->timer);


            SetTextColor(hdc, RGB(255, 255, 0));
            SetBkMode(hdc, TRANSPARENT);


            HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

            TextOut(hdc, window_width - 150, 10, timer_text.c_str(), timer_text.length());


            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            break;
        }
    }
    
}

void game_handle_input(Game* game, WPARAM wParam, LPARAM lParam, int key_down) {
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
                }
                else if (world_num == 2) {
                    game->game_state = STATE_WORLD2;
                }
                else if (world_num == 3) {
                    game->game_state = STATE_WORLD3;
                }
                break;
            }
            }

        }
        break;

    case STATE_WORLD1:
        character_handle_input(game->character, wParam, key_down);
        if (key_down && (wParam == 'C' || wParam == 'c')) {
            map_toggle_collision(game->map);
        }
        break;
    }
}

