#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace ::std;

#define ADD_BTN 0
#define CLEAR_BTN 1
#define TO_RIGHT_BTN 2
#define DELETE_BTN 3
#define LISTBOX1 4
#define LISTBOX2 5
#define EDIT 6
#define MAIN_WINDOW_WIDTH 550
#define MAIN_WINDOW_HEIGHT 400

char Buffer[256];

HWND listBox1, listBox2, edit, toRightBtn, deleteBtn, addBtn, clearBtn;

LRESULT CALLBACK WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int TIMER_ID = 1;
    switch (message)
    {
    case WM_CREATE:
    {
        HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;

        addBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("ADD"), WS_CHILD | WS_VISIBLE,
            30, 150, 100, 30, hWnd, HMENU(ADD_BTN), hIns, NULL);

        clearBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("CLEAR"), WS_CHILD | WS_VISIBLE,
            150, 150, 100, 30, hWnd, HMENU(CLEAR_BTN), hIns, NULL);

        listBox1 = CreateWindow(TEXT("LISTBOX"), NULL,
                                WS_CHILD | WS_VISIBLE | LBS_STANDARD |
                                    LBS_WANTKEYBOARDINPUT,
                                30, 30, 200, 100,
                                hWnd, (HMENU)LISTBOX1, hIns, NULL);

        listBox2 = CreateWindow(TEXT("LISTBOX"), NULL,
                                WS_CHILD | WS_VISIBLE | LBS_STANDARD |
                                    LBS_WANTKEYBOARDINPUT,
                                250, 30, 200, 100,
                                hWnd, (HMENU)LISTBOX2, hIns, NULL);

        toRightBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("TO RIGHT"), WS_CHILD | WS_VISIBLE,
            270, 150, 100, 30, hWnd, HMENU(TO_RIGHT_BTN), hIns, NULL);

        deleteBtn = CreateWindow(
            TEXT("BUTTON"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE,
            390, 150, 100, 30, hWnd, HMENU(DELETE_BTN), hIns, NULL);

        edit = CreateWindow(TEXT("EDIT"), NULL, WS_VISIBLE | WS_CHILD,
                            150, 200, 200, 20, hWnd,
                            HMENU(EDIT), hIns, NULL);

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
        case ADD_BTN:
        {
            GetWindowTextA(edit, Buffer, 256);

            int count = SendMessage(listBox1, LB_GETCOUNT, NULL, NULL);

            int subStrIndex = SendMessage(listBox1, LB_FINDSTRING, -1, (LPARAM)Buffer);

            if (subStrIndex == LB_ERR)
            {
                SendMessage(listBox1, LB_ADDSTRING, NULL, (LPARAM)Buffer);
            }

            break;
        }
        case CLEAR_BTN:
        {
            SendMessage(listBox1, LB_RESETCONTENT, NULL, NULL);
            SendMessage(listBox2, LB_RESETCONTENT, NULL, NULL);
            break;
        }
        case TO_RIGHT_BTN:
        {
            int selectedIndex = SendMessage(listBox1, LB_GETCURSEL, NULL, NULL);
            if (selectedIndex != LB_ERR)
            {
                char listBoxText[256];
                SendMessage(listBox1, LB_GETTEXT, selectedIndex, (LPARAM)listBoxText);

                int subStrIndex = SendMessage(listBox2, LB_FINDSTRING, -1, (LPARAM)listBoxText);

                if (subStrIndex == LB_ERR)
                {
                    SendMessage(listBox2, LB_ADDSTRING, NULL, (LPARAM)listBoxText);
                }
            }
            break;
        }
        case DELETE_BTN:
        {
            int selIndexListBox1 = SendMessage(listBox1, LB_GETCURSEL, NULL, NULL);
            int selIndexListBox2 = SendMessage(listBox2, LB_GETCURSEL, NULL, NULL);

            SendMessage(listBox1, LB_DELETESTRING, selIndexListBox1, NULL);
            SendMessage(listBox2, LB_DELETESTRING, selIndexListBox2, NULL);
        }
        }
        break;
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
    wcWindowClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(200, 255, 255)));

    return RegisterClass(&wcWindowClass);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    LPCTSTR lpzClass = _T("My Window Class!");

    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect); 
    int x = screen_rect.right / 2 - 150;
    int y = screen_rect.bottom / 2 - 75;

    HWND hWnd = CreateWindow(lpzClass, _T("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL,
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