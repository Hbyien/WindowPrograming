#include "character.h"
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
    character->state = IDLE;
    character->direction = RIGHT;
    character->move_direction = 0;
    character->current_frame = 0;
    character->animation_timer = 0.0f;
    character->animation_speed = 0.05f; // 애니메이션 속도 설정

    // 이미지 로드
    HRESULT hr = character->images[IDLE].Load(L"Character//Kirby_Standard.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load IDLE image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    hr = character->images[WALK].Load(L"Character//Kirby_Walk.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load WALK image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    hr = character->images[JUMP].Load(L"Character//Kirby_Jump.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load JUMP image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    hr = character->images[FLY].Load(L"Character//Kirby_Fly.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load FLY image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }

    return character;
}

void character_destroy(Character* character) {
    free(character);
}

void character_move(Character* character, Map* map, int dx, int dy) {
    int new_x = character->x + dx;
    float new_y = character->y + dy;

    if (!map_check_collision(map, new_x, character->y, character->width, character->height - 2)) {
        character->x = new_x;
    }

    if (!map_check_collision(map, character->x, new_y, character->width, character->height - 2)) {
        character->y = new_y;
    }
    else {
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

    CImage* current_image = &character->images[character->state];

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, *current_image);

    switch (character->state) {
    case IDLE:
        if (character->direction == RIGHT) {
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, 0, 0, current_image->GetWidth(), current_image->GetHeight(), RGB(0, 0, 0));
        }
        else {
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldTempBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

            // 원본 이미지를 tempDC에 복사
            TransparentBlt(tempDC, 0, 0, render_width, render_height, memDC, 0, 0, current_image->GetWidth(), current_image->GetHeight(), RGB(0, 0, 0));

            // 새로운 DC와 비트맵 생성
            HDC flippedDC = CreateCompatibleDC(hdc);
            HBITMAP flippedBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldFlippedBitmap = (HBITMAP)SelectObject(flippedDC, flippedBitmap);

            // tempDC에 있는 이미지를 flippedDC에 좌우 반전하여 복사
            for (int x = 0; x < render_width; x++) {
                BitBlt(flippedDC, render_width - 1 - x, 0, 1, render_height, tempDC, x, 0, SRCCOPY);
            }

            // flippedDC에서 투명 복사
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, flippedDC, 0, 0, render_width, render_height, RGB(0, 0, 0));

            // 자원 해제
            SelectObject(tempDC, oldTempBitmap);
            DeleteObject(tempBitmap);
            DeleteDC(tempDC);

            SelectObject(flippedDC, oldFlippedBitmap);
            DeleteObject(flippedBitmap);
            DeleteDC(flippedDC);
        }
        break;

    case WALK: {
        int sprite_width = character->images[WALK].GetWidth() / 10;
        int sprite_height = character->images[WALK].GetHeight();
        int frame_x = character->current_frame * sprite_width;

        if (character->direction == RIGHT) {
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));
        }
        else {
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldTempBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

            RECT rect = { 0, 0, render_width, render_height };
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(tempDC, &rect, whiteBrush);
            DeleteObject(whiteBrush);

            // 걷기 애니메이션 프레임을 tempDC에 복사
            TransparentBlt(tempDC, 0, 0, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));

            // 새로운 DC와 비트맵 생성
            HDC flippedDC = CreateCompatibleDC(hdc);
            HBITMAP flippedBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldFlippedBitmap = (HBITMAP)SelectObject(flippedDC, flippedBitmap);

            // tempDC에 있는 이미지를 flippedDC에 좌우 반전하여 복사
            for (int x = 0; x < render_width; x++) {
                BitBlt(flippedDC, render_width - 1 - x, 0, 1, render_height, tempDC, x, 0, SRCCOPY);
            }

            // flippedDC에서 투명 복사
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, flippedDC, 0, 0, render_width, render_height, RGB(255, 255, 255));

            // 자원 해제
            SelectObject(tempDC, oldTempBitmap);
            DeleteObject(tempBitmap);
            DeleteDC(tempDC);

            SelectObject(flippedDC, oldFlippedBitmap);
            DeleteObject(flippedBitmap);
            DeleteDC(flippedDC);
        }
        break;
    }
    case JUMP: {
        int sprite_width = character->images[JUMP].GetWidth() / 6; // Assuming 6 frames for JUMP animation
        int sprite_height = character->images[JUMP].GetHeight();
        int frame_x = character->current_frame * sprite_width;

        if (character->direction == RIGHT) {
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));
        }
        else {
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldTempBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

            RECT rect = { 0, 0, render_width, render_height };
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(tempDC, &rect, whiteBrush);
            DeleteObject(whiteBrush);

            // 점프 애니메이션 프레임을 tempDC에 복사
            TransparentBlt(tempDC, 0, 0, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));

            // 새로운 DC와 비트맵 생성
            HDC flippedDC = CreateCompatibleDC(hdc);
            HBITMAP flippedBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldFlippedBitmap = (HBITMAP)SelectObject(flippedDC, flippedBitmap);

            // tempDC에 있는 이미지를 flippedDC에 좌우 반전하여 복사
            for (int x = 0; x < render_width; x++) {
                BitBlt(flippedDC, render_width - 1 - x, 0, 1, render_height, tempDC, x, 0, SRCCOPY);
            }

            // flippedDC에서 투명 복사
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, flippedDC, 0, 0, render_width, render_height, RGB(255, 255, 255));

            // 자원 해제
            SelectObject(tempDC, oldTempBitmap);
            DeleteObject(tempBitmap);
            DeleteDC(tempDC);

            SelectObject(flippedDC, oldFlippedBitmap);
            DeleteObject(flippedBitmap);
            DeleteDC(flippedDC);
        }
        break;
    }
    case FLY: {
        int sprite_width = character->images[FLY].GetWidth() / 6;
        int sprite_height = character->images[FLY].GetHeight();
        int frame_x = character->current_frame * sprite_width;

        if (character->direction == RIGHT) {
            TransparentBlt(hdc, render_x, render_y, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));
        }
        else {
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldTempBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

            RECT rect = { 0, 0, render_width, render_height };
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(tempDC, &rect, whiteBrush);
            DeleteObject(whiteBrush);

            TransparentBlt(tempDC, 0, 0, render_width, render_height, memDC, frame_x, 0, sprite_width, sprite_height, RGB(255, 255, 255));

            HDC flippedDC = CreateCompatibleDC(hdc);
            HBITMAP flippedBitmap = CreateCompatibleBitmap(hdc, render_width, render_height);
            HBITMAP oldFlippedBitmap = (HBITMAP)SelectObject(flippedDC, flippedBitmap);

            for (int x = 0; x < render_width; x++) {
                BitBlt(flippedDC, render_width - 1 - x, 0, 1, render_height, tempDC, x, 0, SRCCOPY);
            }

            TransparentBlt(hdc, render_x, render_y, render_width, render_height, flippedDC, 0, 0, render_width, render_height, RGB(255, 255, 255));

            SelectObject(tempDC, oldTempBitmap);
            DeleteObject(tempBitmap);
            DeleteDC(tempDC);

            SelectObject(flippedDC, oldFlippedBitmap);
            DeleteObject(flippedBitmap);
            DeleteDC(flippedDC);
        }
        break;
    }
    }

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}

void character_handle_input(Character* character, WPARAM wParam, int key_down) {
    if (key_down) {
        switch (wParam) {
        case VK_LEFT:
            character->direction = LEFT;
            character->move_direction = -1;
            break;
        case VK_RIGHT:
            character->direction = RIGHT;
            character->move_direction = 1;
            break;
        case VK_SPACE:
            if (character->is_flying) {
                character->jump_velocity = -100;
            }
            else if (character->is_jumping && !character->is_flying) {
                character->is_flying = true;
                character->state = FLY;
            }
            else if (!character->is_jumping) {
                character->jump_velocity = -300;
                character->is_jumping = true;
                character->state = JUMP;
            }
            break;
        case 'E':
        case 'e':
            if (character->is_flying) {
                character->is_flying = false;
                character->jump_velocity = 0;
                character->state = JUMP;
            }
            break;
        }
    }
    else {
        if (wParam == VK_LEFT || wParam == VK_RIGHT) {
            character->move_direction = 0;
        }
    }
}

void character_update(Character* character, Map* map, float dt) {
    int dx = character->move_direction * (character->is_flying ? 1 : 2);
    character_move(character, map, dx, 0);

    // 이동 상태에 따른 애니메이션 상태 변경
    if (character->move_direction != 0 && !character->is_jumping && !character->is_flying) {
        character->state = WALK;
    }
    else if (!character->is_jumping && !character->is_flying) {
        character->state = IDLE;
    }

    if (character->is_flying) {
        float new_y = character->y + character->jump_velocity * dt;
        if (character->jump_velocity <= 0) {
            character->jump_velocity += character->gravity * 0.8f * dt;
        }
        else {
            character->jump_velocity += character->gravity * 0.05f * dt;
        }
        if (!map_check_collision(map, character->x, new_y, character->width, character->height)) {
            character->y = new_y;
        }
    }
    else if (character->is_jumping) {
        float new_y = character->y + character->jump_velocity * dt;
        character->jump_velocity += character->gravity * dt;
        if (!map_check_collision(map, character->x, new_y, character->width, character->height)) {
            character->y = new_y;
        }
        else {
            character->is_jumping = false;
            character->jump_velocity = 0;
            character->state = IDLE;
        }
    }
    else {

        float new_y = character->y + character->gravity * dt;
        if (!map_check_collision(map, character->x, new_y, character->width, character->height)) {
            character->y = new_y;
            character->is_jumping = true;
            character->jump_velocity = 0;
            character->state = JUMP;
        }
    }

    // 애니메이션 타이머 업데이트 및 프레임 변경
    if (character->state == WALK || character->state == JUMP || character->state == FLY) {
        character->animation_timer += dt;
        if (character->animation_timer >= character->animation_speed) {
            if (character->state == WALK) {
                character->current_frame = (character->current_frame + 1) % 10;
            }
            else if (character->state == JUMP) {
                character->current_frame = (character->current_frame + 1) % 6;
            }
            else if (character->state == FLY) {
                character->current_frame = (character->current_frame + 1) % 6;
            }
            character->animation_timer = 0.0f;
        }
    }
    else {
        character->current_frame = 0;
    }
}
