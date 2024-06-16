#include "ui.h"

void ui_init(UI* ui, const wchar_t* life_image_path, const wchar_t* coin_image_path, int coin_total_frames, float coin_animation_speed) {
    ui->lives = 3; // 초기 목숨 개수
    ui->coins = 0; // 초기 코인 개수

    // 이미지 로드
    HRESULT hr = ui->life_image.Load(life_image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load life image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    ui->life_image.SetTransparentColor(RGB(255, 255, 255)); // 투명 색상 설정 (흰색)

    hr = ui->coin_image.Load(coin_image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load coin image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    ui->coin_image.SetTransparentColor(RGB(255, 255, 255)); // 투명 색상 설정 (흰색)

    // 코인 애니메이션 초기화
    ui->coin_current_frame = 0;
    ui->coin_total_frames = coin_total_frames;
    ui->coin_animation_speed = coin_animation_speed;
    ui->coin_animation_timer = 0.0f;
}

void ui_destroy(UI* ui) {
    ui->life_image.Destroy();
    ui->coin_image.Destroy();
}
void ui_render(UI* ui, HDC hdc, int window_width, int window_height) {
    HDC memDC = CreateCompatibleDC(hdc);

    // 목숨 이미지 그리기 (50, 50 위치에)
    int life_image_width = ui->life_image.GetWidth() / 2;  // 이미지 크기 조정
    int life_image_height = ui->life_image.GetHeight() / 2; // 이미지 크기 조정
    int life_image_spacing = life_image_width + 5; // 간격 조정

    for (int i = 0; i < ui->lives; ++i) {
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, ui->life_image);
        TransparentBlt(hdc, 50 + i * life_image_spacing, 50, life_image_width, life_image_height, memDC, 0, 0, ui->life_image.GetWidth(), ui->life_image.GetHeight(), RGB(0, 0, 0));
        SelectObject(memDC, oldBitmap);
    }

    // 코인 이미지 및 개수 그리기 (목숨 이미지 아래에)
    int coin_image_x = 50;
    int coin_image_y = 50 + life_image_height + 20;  // 간격 조정
    int frame_width = ui->coin_image.GetWidth() / ui->coin_total_frames;
    int frame_height = ui->coin_image.GetHeight();
    int scaled_frame_width = frame_width * 1.5;  // 1.5배 크기 조정
    int scaled_frame_height = frame_height * 1.5; // 1.5배 크기 조정

    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, ui->coin_image);
    TransparentBlt(hdc, coin_image_x, coin_image_y, scaled_frame_width, scaled_frame_height, memDC, ui->coin_current_frame * frame_width, 0, frame_width, frame_height, RGB(255, 255, 255));
    SelectObject(memDC, oldBitmap);

    wchar_t coin_text[10];
    swprintf_s(coin_text, 10, L"x %d", ui->coins);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, coin_image_x + scaled_frame_width + 10, coin_image_y + (scaled_frame_height / 4), coin_text, wcslen(coin_text));

    DeleteDC(memDC);
}
void ui_update(UI* ui, float dt) {
    ui->coin_animation_timer += dt;
    if (ui->coin_animation_timer >= ui->coin_animation_speed) {
        ui->coin_current_frame = (ui->coin_current_frame + 1) % ui->coin_total_frames;
        ui->coin_animation_timer = 0.0f;
    }
}

void ui_update_lives(UI* ui, int lives) {
    ui->lives = lives;
}

void ui_update_coins(UI* ui, int coins) {
    ui->coins = coins;
}
