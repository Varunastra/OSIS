
#include "framework.h"
#include "WindowsProjectSP7CS.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
CRITICAL_SECTION cs;
HANDLE threads[3];
HWND main_hwnd;
bool flag = false;

struct ThreadArgs
{
	int x, y, event_handle, next_event_handle;
} args[3];

void debug(int lol)
{
	char b[15];
	_itoa_s((int)lol, b, 15, 10);
	SetWindowTextA(main_hwnd, b);
}

DWORD WINAPI ThreadProc(ThreadArgs* arg)
{
	while (true)
	{
		EnterCriticalSection(&cs);
		if (arg->x == 290) arg->x = 0;
		arg->x += 10;
		
		InvalidateRect(main_hwnd, NULL, true);
		LeaveCriticalSection(&cs);
		Sleep(100);
	}
	return 1;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECTSP7CS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECTSP7CS));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECTSP7CS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECTSP7CS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   main_hwnd = hWnd;
   args[0].y = 10;
   args[1].y = 110;
   args[2].y = 210;

   args[0].x = 0;
   args[1].x = 0;
   args[2].x = 0;

   args[0].next_event_handle = 1;
   args[1].next_event_handle = 2;
   args[2].next_event_handle = 0;

   args[0].event_handle = 0;
   args[1].event_handle = 1;
   args[2].event_handle = 2;
   InitializeCriticalSectionAndSpinCount(&cs, 0x80000400);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
				if (!flag)
				{
					flag = true;
					for (int i = 0; i < 3; i++)
						threads[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,&(args[i]),0,NULL);
					
				}
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			if (flag)
			{
				HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
				HPEN pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 255));
				SelectObject(hdc, pen);
				SelectObject(hdc, brush);
				for (int i = 0; i < 3; i++)
				{
					Rectangle(hdc, args[i].x, args[i].y, args[i].x + 50, args[i].y + 30);
					Rectangle(hdc, args[i].x + 52, args[i].y, args[i].x + 75, args[i].y + 30);
					Rectangle(hdc, args[i].x + 75, args[i].y + 15, args[i].x + 100, args[i].y + 30);
					Rectangle(hdc, args[i].x + 120, args[i].y + 7, args[i].x + 90, args[i].y + 15);
				}
			}
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
