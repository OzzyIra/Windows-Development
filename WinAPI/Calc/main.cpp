#include<Windows.h>
#include"resource.h"
#include<float.h>
#include<cstdio>

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc";

CONST CHAR g_OPERATIONS[] = "+-*/";

CONST INT g_BUTTON_SIZE = 50;
CONST INT g_INTERVAL = 5;
CONST INT g_START_X = 10;
CONST INT g_BUTTON_DOUBLE_SIZE = g_BUTTON_SIZE * 2 + g_INTERVAL;
CONST INT g_START_Y = 10;
CONST INT g_DISPLAY_WIDTH = g_BUTTON_SIZE * 5 + g_INTERVAL * 4;
CONST INT g_DISPLAY_HEIGHT = 25;
CONST INT g_BUTTON_START_X = g_START_X;
CONST INT g_BUTTON_START_Y = g_START_Y + g_DISPLAY_HEIGHT + g_INTERVAL;
CONST INT g_OPERATIONS_START_X = g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 3;
CONST INT g_OPERATIONS_START_Y = g_BUTTON_START_Y;
CONST INT g_CONTROL_BUTTONS_START_X = g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 4;
CONST INT g_CONTROL_BUTTONS_START_Y = g_BUTTON_START_Y;


BOOL CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registreation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME - WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_DISPLAY_WIDTH + g_BUTTON_START_X * 2 + 16, g_DISPLAY_HEIGHT + (g_BUTTON_SIZE + g_INTERVAL) * 4 + g_START_Y * 2 + 16 + 23,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запуск цикла сообшений:
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message;
}

BOOL CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
	static DOUBLE a = DBL_MIN, b;	//операнды
	static INT operation;		//операции
	static BOOL input;		//устанавливается при вводе числа 
	static BOOL operation_input;		//устанавливается пр вводе операции
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEditDisplay = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_RIGHT,
			g_START_X, g_START_Y,
			g_DISPLAY_WIDTH, g_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		CHAR sz_digit[2]{};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i + j + 49;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE,
					g_BUTTON_START_X + (g_BUTTON_SIZE + g_INTERVAL) * j, g_BUTTON_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * (2 - i / 3),
					g_BUTTON_SIZE, g_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE,
			g_BUTTON_START_X, g_BUTTON_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * 3,
			g_BUTTON_DOUBLE_SIZE, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandleA(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE,
			g_BUTTON_START_X + g_INTERVAL + g_BUTTON_DOUBLE_SIZE, g_BUTTON_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * 3,
			g_BUTTON_SIZE, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandleA(NULL),
			NULL
		);

		CHAR sz_operation[2]{};
		for (int i = 0; i < 4; i++)
		{
			sz_operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE,
				g_OPERATIONS_START_X, g_OPERATIONS_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * (4 - 1 - i),
				g_BUTTON_SIZE, g_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandleA(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE,
			g_CONTROL_BUTTONS_START_X, g_CONTROL_BUTTONS_START_Y,
			g_BUTTON_SIZE, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandleA(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE,
			g_CONTROL_BUTTONS_START_X, g_CONTROL_BUTTONS_START_Y + g_BUTTON_SIZE + g_INTERVAL,
			g_BUTTON_SIZE, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLEAR,
			GetModuleHandleA(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE,
			g_CONTROL_BUTTONS_START_X, g_CONTROL_BUTTONS_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * 2,
			g_BUTTON_SIZE, g_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandleA(NULL),
			NULL
		);

	}
	break;
	case WM_COMMAND:

	{
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		CHAR sz_digit[2]{};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);

		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (input == FALSE && operation_input == TRUE)
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"");
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48;
			SendMessageA(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (sz_display[0] == '0' && sz_display[1] != '.')
			{
				sz_display[0] = 0;
			}
			strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, NULL, (LPARAM)sz_display);
			input = TRUE;

		}

		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			SendMessageA(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strchr(sz_display, '.'))
				break;
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, NULL, (LPARAM)sz_display);
		}

		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessageA(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strlen(sz_display) == 1)
			{
				sz_display[0] = '0';
			}
			else
			{
				sz_display[strlen(sz_display) - 1] = 0;
			}
			SendMessageA(hEditDisplay, WM_SETTEXT, NULL, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = operation_input = FALSE;
			sz_display[0] = '0';
			SendMessageA(hEditDisplay, WM_SETTEXT, NULL, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessageA(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (a == DBL_MIN)a = strtod(sz_display, NULL);
			else b = strtod(sz_display, NULL);
			if(input)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operation = LOWORD(wParam);
			input = FALSE;
			operation_input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessageA(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if(operation_input) b = strtod(sz_display, NULL);
			switch (operation)
			{
			case IDC_BUTTON_PLUS:	a += b; break;
			case IDC_BUTTON_MINUS:	a -= b; break;
			case IDC_BUTTON_ASTER:	a *= b; break;
			case IDC_BUTTON_SLASH:	a /= b; break;
			}
			operation_input = FALSE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}