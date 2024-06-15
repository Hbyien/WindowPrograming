#include "Map.h"
#include "Game.h"  // For changing the game state
#include <tchar.h>
#include <atlimage.h>
#include <string>
#include <wingdi.h> // for TransparentBlt

CImage mapImage;
CImage playerImage;

int mapPlayerX = 170;
int mapPlayerY = 300;
//int targetX = 170;   
//int targetY = 300;
int currentPosition = 0;
//bool isMoving = false;
//DWORD moveStartTime = 0;

const int positionsX[] = { 170, 310, 450, 730 };

void MoveMapPlayerLeft() {
    if (currentPosition > 0/* && !isMoving*/) {
        currentPosition--;
        /*   targetX = positionsX[currentPosition];
           isMoving = true;
           moveStartTime = GetTickCount();*/
        mapPlayerX = positionsX[currentPosition];
    }
}

void MoveMapPlayerRight() {
    if (currentPosition < sizeof(positionsX) / sizeof(positionsX[0]) - 1 /*&& !isMoving*/) {
        currentPosition++;
        /* targetX = positionsX[currentPosition];
         isMoving = true;
         moveStartTime = GetTickCount();*/
        mapPlayerX = positionsX[currentPosition];
    }
}

void LoadMap() {
    HRESULT hr = mapImage.Load(L"Baseimage//Map.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load map image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }

    hr = playerImage.Load(L"Character//Kirby_Standard.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load player image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
}

void FreeMap() {
    mapImage.Destroy();
    playerImage.Destroy();
}

int CheckEnterKey() {
    if (mapPlayerX == 310 && mapPlayerY == 300) {
        return 1;
    }
    else if (mapPlayerX == 450 && mapPlayerY == 300) {
        return 2;
    }
    else if (mapPlayerX == 730 && mapPlayerY == 300) {
        return 3;
    }
}

void RenderMap(HDC hDC) {

    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmp = CreateCompatibleBitmap(hDC, 800, 600);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);


    RECT rect = { 0, 0, 800, 600 };
    FillRect(hMemDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

    if (!mapImage.IsNull()) {
        int windowWidth = 800;
        int windowHeight = 600;
        mapImage.StretchBlt(hMemDC, 0, 0, windowWidth, windowHeight, SRCCOPY);
    }
    else {
        TextOut(hMemDC, 350, 250, _T("MAP SCREEN"), 11);
    }

    if (!playerImage.IsNull()) {

        int playerWidth = playerImage.GetWidth();
        int playerHeight = playerImage.GetHeight();


        playerImage.TransparentBlt(hMemDC, mapPlayerX - playerWidth / 2, mapPlayerY - playerHeight / 2, playerWidth, playerHeight, RGB(0, 0, 0));
    }


    std::wstring coordinates = L"Player Coordinates: (" + std::to_wstring(mapPlayerX) + L", " + std::to_wstring(mapPlayerY) + L")";
    TextOut(hMemDC, 10, 10, coordinates.c_str(), coordinates.size());


    BitBlt(hDC, 0, 0, 800, 600, hMemDC, 0, 0, SRCCOPY);

    // Cleanup
    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);
}
