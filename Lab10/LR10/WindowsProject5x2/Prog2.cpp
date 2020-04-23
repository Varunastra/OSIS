// WindowsProject5x2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Prog2.h"

#define MAX_LOADSTRING 100
//my defines
#define RBUTTON11_ID 1
#define RBUTTON12_ID 2
#define RBUTTON13_ID 3
#define RBUTTON21_ID 4
#define RBUTTON22_ID 5
#define RBUTTON23_ID 6
#define RBUTTON24_ID 7
#define CHECKBOX_ID 8

#define DRAW_MESSAGE L"DrawMessage"
#define COLOR_MESSAGE L"ColorMessage"
#define SHAPE_MESSAGE L"ShapeMessage"

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];               
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND main_hWnd;
UINT WM_DRAW;
UINT WM_COLOR;
UINT WM_SHAPE;

void debug(int lol)
{
	char b[15];
	_itoa_s((int)lol, b, 15, 10);
	SetWindowTextA(main_hWnd, b);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT5X2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT5X2));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT5X2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT5X2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 250, 400, 300, nullptr, nullptr, hInstance, nullptr);

   CreateWindow(L"button", L"Красный",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   40, 40, 90, 30, hWnd, (HMENU)RBUTTON11_ID, hInstance, NULL);
   CreateWindow(L"button", L"Синий",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   40, 65, 90, 30, hWnd, (HMENU)RBUTTON13_ID, hInstance, NULL);
   CreateWindow(L"button", L"Зеленый",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   40, 90, 90, 30, hWnd, (HMENU)RBUTTON12_ID, hInstance, NULL);

   CreateWindow(L"button", L"Ромб",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   250, 10, 90, 30, hWnd, (HMENU)RBUTTON21_ID, hInstance, NULL);
   CreateWindow(L"button", L"Квадрат",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   250, 35, 90, 30, hWnd, (HMENU)RBUTTON22_ID, hInstance, NULL);
   CreateWindow(L"button", L"Круг",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   250, 60, 90, 30, hWnd, (HMENU)RBUTTON23_ID, hInstance, NULL);
   CreateWindow(L"button", L"Звезда",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   250, 85, 90, 30, hWnd, (HMENU)RBUTTON24_ID, hInstance, NULL);

   CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, RBUTTON11_ID);
   CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, RBUTTON21_ID);

   CreateWindow(L"button", L"Draw",
	   WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
	   150, 150, 90, 30, hWnd, (HMENU)CHECKBOX_ID, hInstance, NULL);

   WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
   WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
   WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

   main_hWnd = FindWindow(NULL, L"Prog1");

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
	int wmId;
	int wmEvent;
    switch (message)
    {
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);

			if (wmId >= RBUTTON11_ID && wmId <= RBUTTON13_ID) 
			{
				
				if (main_hWnd == NULL) break;
				

				SendMessage(main_hWnd, WM_COLOR, 0, wmId - RBUTTON11_ID);

				CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, wmId);
			}
			else if (wmId >= RBUTTON21_ID && wmId <= RBUTTON24_ID) 
			{
				if (main_hWnd == NULL) break;
				

				SendMessage(main_hWnd, WM_SHAPE, 0, wmId - RBUTTON21_ID);

				CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, wmId);
			}
			else switch (wmId)
			{
				case CHECKBOX_ID:
					{
						if (main_hWnd == NULL) break;
						

						SendMessage(main_hWnd, WM_DRAW, 0, 0);

						HWND hWndCheck = GetDlgItem(hWnd, CHECKBOX_ID);
						LRESULT state = SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
						//debug(state);
						if (state == BST_CHECKED)
						{
							SendMessage(main_hWnd, WM_DRAW, 0, 1);
						}
						if (state == BST_UNCHECKED)
						{
							SendMessage(main_hWnd, WM_DRAW, 0, 0);
						}
					}
					break;
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
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
