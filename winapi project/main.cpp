// main.c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include <iostream>
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console") 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
Game game;
#define marginX 16
#define marginY 32

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"������ ���α׷��� ����";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, L"������ ���α׷��� ����", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800 + marginX, 600 + marginY, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        game_init(&game, hWnd);
        SetTimer(hWnd, 1, 5, NULL);
        break;
    case WM_TIMER:
        game_update(&game);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // ���� ���۸��� ���� �޸� DC ����
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 800, 600);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // ����� ����� ���� �޸� DC �ʱ�ȭ
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        // ������ �޸� DC�� ������
        game_render(&game, memDC);

        // �޸� DC�� ���� DC�� ����
        BitBlt(hdc, 0, 0, 800, 600, memDC, 0, 0, SRCCOPY);

        // ���ҽ� ����
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
        game_handle_input(&game, wParam, lParam, 1);
        break;
    case WM_KEYUP:
        game_handle_input(&game, wParam, lParam, 0);
        break;
    case WM_LBUTTONDOWN:
        game_handle_input(&game, wParam, lParam, 0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
