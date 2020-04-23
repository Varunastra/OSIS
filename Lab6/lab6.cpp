#include <windows.h>
#include <tchar.h>
#include <wingdi.h>

#define IDM_FILE_QUIT 3
#define START_CLICKED 4
#define STOP_CLICKED 5
#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 400

int X = MAIN_WINDOW_WIDTH / 2 - 200;
int Y = MAIN_WINDOW_HEIGHT / 2 - 40;
bool backFlag = false;

void AddMenus(HWND hwnd)
{

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, START_CLICKED, L"&Start");
    AppendMenuW(hMenu, MF_STRING, STOP_CLICKED, L"&Stop");
    AppendMenuW(hMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenuW(hMenu, MF_STRING, NULL, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Actions");
    SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int TIMER_ID = 1;
    // выборка и обработка сообщений
    switch (message)
    {
    case WM_CREATE:
        // CreateWindowW(L"STATIC", lyrics, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | CS_HREDRAW,
        //               20, 20, 250, 100,
        //               hWnd, (HMENU)1, NULL, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0); // реакция на сообщение
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_QUIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case START_CLICKED:
        {
            UINT_PTR ret = SetTimer(hWnd, TIMER_ID, 50, NULL);
            if (ret == 0)
                MessageBox(hWnd, _T("Timer not set"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
        }
        break;
        case STOP_CLICKED:
        {
            KillTimer(hWnd, TIMER_ID);
        }
        break;
        }

    case WM_TIMER:
    {
        if (X >= MAIN_WINDOW_WIDTH - 200)
        {
            backFlag = true;
        }
        if (X <= 150)
        {
            backFlag = false;
        }
        if (!backFlag)
        {
            X += 2;
        }
        else
        {
            X -= 2;
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        const char text[] = "SLAVA UKRAINE";
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOutA(hdc, X, Y, text, strlen(text));
        EndPaint(hWnd, &ps);
    }
    break;

    default:
        // все сообщения не обработанные Вами обработает сама Windows
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
    WNDCLASS wcWindowClass = {0};
    // адрес ф-ции обработки сообщений
    wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
    // стиль окна
    wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    // дискриптор экземпляра приложения
    wcWindowClass.hInstance = hInst;
    // название класса
    wcWindowClass.lpszClassName = lpzClassName;
    // загрузка курсора
    wcWindowClass.hCursor = (HICON)LoadImage(NULL, _T("./icon.ico"), IMAGE_CURSOR, 20, 20, LR_LOADFROMFILE);
    // загрузка цвета окон
    wcWindowClass.hIcon = (HICON)LoadImage(NULL, _T("./window.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wcWindowClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    return RegisterClass(&wcWindowClass); // регистрация класса
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    // имя будущего класса
    LPCTSTR lpzClass = _T("My Window Class!");

    // регистрация класса
    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    // вычисление координат центра экрана
    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
    int x = screen_rect.right / 2 - 150;
    int y = screen_rect.bottom / 2 - 75;

    // создание диалогового окна
    HWND hWnd = CreateWindow(lpzClass, _T("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL,
                             hInstance, NULL);

    // если окно не создано, описатель будет равен 0
    if (!hWnd)
        return 2;

    AddMenus(hWnd);

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

    return msg.wParam; // возвращаем код завершения программы
}