#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <iostream>
#include <windef.h>

#define PAINT_BTN 0
#define CLEAR_BTN 1
#define MAIN_WINDOW_WIDTH 550
#define MAIN_WINDOW_HEIGHT 400

char Buffer[256];

HWND paintBtn, clearBtn;
bool draw = false;
HDC hdc;

RECT rect;

LRESULT CALLBACK WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
    {
        HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;

        paintBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("PAINT"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            180, 300, 50, 50, hWnd, HMENU(PAINT_BTN), hIns, NULL);

        clearBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("CLEAR"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            280, 300, 50, 50, hWnd, HMENU(CLEAR_BTN), hIns, NULL);

        // HDC paintDC = GetDC(paintBtn);

        // HDC clearBtnDC = GetDC(paintBtn);

        // SetBkMode(paintDC, TRANSPARENT);

        // ReleaseDC(paintBtn, paintDC);

        // SetBkMode(clearBtnDC, TRANSPARENT);

        // ReleaseDC(clearBtn, clearBtnDC);

        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case PAINT_BTN:
        {
            draw = true;
            InvalidateRect(hWnd, &rect, TRUE);
            break;
        }
        case CLEAR_BTN:
        {
            draw = false;
            InvalidateRect(hWnd, &rect, TRUE);
            break;
        }
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = GetDC(hWnd);
        SetBkMode(hdc, TRANSPARENT);
        BeginPaint(hWnd, &ps);
        if (draw)
        {
            POINT pt[3];
            pt[0].x = 150;
            pt[0].y = 250;
            pt[1].x = 200;
            pt[1].y = 150;
            pt[2].x = 250;
            pt[2].y = 250;
            POINT ptn[5];
            ptn[0].x = 5.09 * 10;
            ptn[0].y = 5.80 * 10;
            ptn[1].x = 1.68 * 10;
            ptn[1].y = 4.90 * 10;
            ptn[2].x = 1.48 * 10;
            ptn[2].y = 1.38 * 10;
            ptn[3].x = 4.76 * 10;
            ptn[3].y = 0.1 * 10;
            ptn[4].x = 7 * 10;
            ptn[4].y = 2.83 * 10;
            MoveToEx(hdc, 100, 100, 0);
            LineTo(hdc, 150, 150);
            MoveToEx(hdc, 200, 200, 0);
            LineTo(hdc, 300, 200);
            MoveToEx(hdc, 50, 50, 0);
            Arc(hdc, 10, 10, 150, 150, 150, 10, 10, 100);
            Arc(hdc, 10, 10, 100, 100, 10, 10, 100, 10);
            Ellipse(hdc, 140, 100, 190, 130);
            Ellipse(hdc, 200, 100, 250, 130);
            Rectangle(hdc, 100, 50, 300, 40);
            Rectangle(hdc, 50, 50, 100, 80);
            Polygon(hdc, pt, 3);
            Polygon(hdc, ptn, 5);
            RoundRect(hdc, 250, 100, 350, 200, 40, 40);
            RoundRect(hdc, 300, 100, 350, 50, 10, 30);
        }
        ReleaseDC(hWnd, hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpdrawstLogon = (LPDRAWITEMSTRUCT)lParam;
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        if (lpdrawstLogon->hwndItem == paintBtn)
        {
            // FillRect(lpdrawstLogon->hDC, &lpdrawstLogon->rcItem, CreateSolidBrush(RGB(4, 37, 65)));
            Gdiplus::Graphics *gdiGrLogon = new Gdiplus::Graphics(lpdrawstLogon->hDC);
            Gdiplus::Image *gdiImgLogon = new Gdiplus::Image(L"paint.png");
            gdiGrLogon->DrawImage(gdiImgLogon, 0, 0, 50, 50);
            delete gdiImgLogon;
            delete gdiGrLogon;
        }

        if (lpdrawstLogon->hwndItem == clearBtn)
        {
            HBITMAP bitmapHandler;

            Gdiplus::Graphics *gdiGrLogon = new Gdiplus::Graphics(lpdrawstLogon->hDC);
            Gdiplus::Image *gdiImgLogon = new Gdiplus::Image(L"clear.png");
            gdiGrLogon->DrawImage(gdiImgLogon, 0, 0, 50, 50);
            delete gdiImgLogon;
            delete gdiGrLogon;
        }

        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
    WNDCLASS wcWindowClass = {0};
    wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
    wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpzClassName;
    wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcWindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(240, 240, 240));

    return RegisterClass(&wcWindowClass);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    LPCTSTR lpzClass = _T("My Window Class!");

    rect.top = 0;
    rect.left = 0;
    rect.right = MAIN_WINDOW_WIDTH;
    rect.bottom = MAIN_WINDOW_HEIGHT - 140;

    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    int x = screen_rect.right / 2 - 150;
    int y = screen_rect.bottom / 2 - 75;

    HWND hWnd = CreateWindow(lpzClass, _T("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_EX_TRANSPARENT,
                             x, y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL,
                             hInstance, NULL);

    if (!hWnd)
        return 2;

    // цикл сообщений приложения
    MSG msg = {0};                                       // структура сообщения
    int iGetOk = 0;                                      // переменная состояния
    while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // цикл сообщений
    {
        if (iGetOk == -1)
            return 3; // если GetMessage вернул ошибку - выход
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}