#include "ui.h"

void ui_init(UI* ui, const wchar_t* life_image_path, const wchar_t* coin_image_path, int coin_total_frames, float coin_animation_speed) {
    ui->lives = 3; // �ʱ� ��� ����
    ui->coins = 0; // �ʱ� ���� ����

    // �̹��� �ε�
    HRESULT hr = ui->life_image.Load(life_image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load life image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    ui->life_image.SetTransparentColor(RGB(255, 255, 255)); // ���� ���� ���� (���)

    hr = ui->coin_image.Load(coin_image_path);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load coin image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
    ui->coin_image.SetTransparentColor(RGB(255, 255, 255)); // ���� ���� ���� (���)

    // ���� �ִϸ��̼� �ʱ�ȭ
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

    // ��� �̹��� �׸��� (50, 50 ��ġ��)
    int life_image_width = ui->life_image.GetWidth() / 2;  // �̹��� ũ�� ����
    int life_image_height = ui->life_image.GetHeight() / 2; // �̹��� ũ�� ����
    int life_image_spacing = life_image_width + 5; // ���� ����

    for (int i = 0; i < ui->lives; ++i) {
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, ui->life_image);
        TransparentBlt(hdc, 50 + i * life_image_spacing, 50, life_image_width, life_image_height, memDC, 0, 0, ui->life_image.GetWidth(), ui->life_image.GetHeight(), RGB(0, 0, 0));
        SelectObject(memDC, oldBitmap);
    }

    // ���� �̹��� �� ���� �׸��� (��� �̹��� �Ʒ���)
    int coin_image_x = 50;
    int coin_image_y = 50 + life_image_height + 20;  // ���� ����
    int frame_width = ui->coin_image.GetWidth() / ui->coin_total_frames;
    int frame_height = ui->coin_image.GetHeight();
    int scaled_frame_width = frame_width * 1.5;  // 1.5�� ũ�� ����
    int scaled_frame_height = frame_height * 1.5; // 1.5�� ũ�� ����

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
