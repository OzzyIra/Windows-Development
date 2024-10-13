#include<Windows.h>
#define IDC_STATIC 1000
#define IDC_EDIT 1001
#define IDC_BUTTON 1002

CONST CHAR g_sz_WINDOW_CLASS[] = "Main Window";

INT CALLBACK WNDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	//Appearance:
	/*wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);*/
	wClass.hIcon = (HICON)LoadImage(hInstance, "Ico\\Detective.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "Ico\\Hacker.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);

	//wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hCursor = (HCURSOR)LoadImage
	(
		hInstance,
		"CUR\\starcraft-original\\Move.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_LOADFROMFILE
	);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	//
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WNDProc;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return msg.message;
}

INT CALLBACK WNDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
		CreateWindowEx
		(NULL,
			"static",
			"Это static_text",
			WS_CHILD | WS_VISIBLE,	// для всех дочерних окон стиль обязательно будет WS_CHILD и WS_VISIBLE
			100, 10,
			200, 20,
			hwnd,
			(HMENU)IDC_STATIC,
			GetModuleHandle(NULL),
			NULL

		);
		CreateWindowEx
		(NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,	//WS_BORDER
			100, 30,
			200, 20,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandleA(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "Применить",
			WS_CHILD | WS_VISIBLE,
			200, 50,
			100, 32,
			hwnd,
			(HMENU)IDC_BUTTON,
			GetModuleHandleA(NULL),
			NULL

		);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SendMessageA(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			HWND hStatic = GetDlgItem(hwnd, IDC_STATIC);
			SendMessageA(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessageA(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_CLOSE:DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
	return FALSE;
}
