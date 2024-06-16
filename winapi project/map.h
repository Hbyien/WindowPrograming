// map.h
#ifndef MAP_H
#define MAP_H

#include <windows.h>

typedef struct {
    int x;
    int y;
} KumbaSpawnPoint;

typedef struct Cllif {
    int startx;
    int endx;
};

typedef struct {
    int x;
    int y;
} CoinSpawnPoint;

typedef struct {
    HBITMAP map_image;
    HBITMAP cloud_layer_image;
    float cloud_offset_x;
    int width;
    int height;
    RECT* collision_rects;
    int num_collision_rects;
    bool show_collision;
    Cllif cliffs[3];
    KumbaSpawnPoint* spawn_points; // Kumba 소환 위치 배열
    int num_spawn_points; // Kumba 소환 위치 수
    RECT* brick_collision_rects;
    int num_brick_collisions; // 필드명 수정
    RECT* item_brick_collision_rects; // 추가
    int num_item_brick_collisions; // 추가
    CoinSpawnPoint* coin_spawn_points;
    int num_coin_spawn_points;
} Map;

Map* map_create(const wchar_t* map_filename, const wchar_t* cloud_filename, HWND hWnd);
void map_destroy(Map* map);
void map_render(Map* map, HDC hdc, int x, int y, int window_width, int window_height);
void map_toggle_collision(Map* map);
int map_check_collision(Map* map, int x, int y, int width, int height); // 충돌 체크 함수
bool RectsIntersect(const RECT* rect1, const RECT* rect2);
bool check_cliff(Map* map, int new_x);
void map_update(Map* map, float player_velocity_x, float dt);
#endif // MAP_H
