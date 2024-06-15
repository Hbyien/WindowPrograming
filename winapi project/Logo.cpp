#include "Logo.h"
#include <tchar.h>
#include <atlimage.h>

CImage logoImage;

void LoadLogo() {
    HRESULT hr = logoImage.Load(L"Baseimage//Logo.png");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load logo image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }
}

void FreeLogo() {
    logoImage.Destroy();
}

void RenderLogo(HDC hDC) {
    if (!logoImage.IsNull()) {
        int windowWidth = 800;
        int windowHeight = 600;
        logoImage.StretchBlt(hDC, 0, 0, windowWidth, windowHeight, SRCCOPY);
    }
    else {
        TextOut(hDC, 350, 250, _T("LOGO SCREEN"), 11);
    }

}
