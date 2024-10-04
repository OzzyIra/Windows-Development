#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_LOGIN_INVITATION[] = "������� ��� ������������";
CONST CHAR g_sz_PASWORD_INVITATION[] = "������� ������";


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// hInstance - ��������� ����������� * .exe - ����� ����� ���������
   //hPrevInst - �� ������������
   //LPSTR - Long Pointer To String
   //lpCmdLine - CommandLine (��������� ������ � ����������� ������� ����������)
   //nCmdShow  - ����� ����������� ���� (���������� �� ���� �����, c������� �� ������ ����� � �.�.)
   //��������: n..., lp... ��� ���������� �������
   //			n - Number
   //			lp - Long Pointer
   //			h - HINSTANCE

	 /*MessageBox
	(
		NULL,
		"Hello Windows!\nThis is MessageBox()",
		"Window title",
		MB_CANCELTRYCONTINUE | MB_HELP | MB_DEFBUTTON3 |
		MB_ICONINFORMATION |
		MB_SERVICE_NOTIFICATION |
		MB_RIGHT
	);*/

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)DlgProc, 0);
	return 0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	hwnd - Handler to Window.���������� ��� ���������� ���� - ��� �����, ��� ������ �������� ����� ���������� � ����.
	uMsg - Message. ���������, ������� ������������ ����.
	wParam, lParam - ��� ��������� ���������, � ������� ��������� ���� ����� ����������.
	LOWORD - ������� �����;
	HIWORD - ������� �����;
	DWORD  - Double Word;
	*/
	switch (uMsg)
	{
	case WM_INITDIALOG:		//1 ��� ��� ������������� ����
	{
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPasword = GetDlgItem(hwnd, IDC_EDIT_PASWORD);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
		SendMessage(hEditPasword, WM_SETTEXT, 0, (LPARAM)g_sz_PASWORD_INVITATION);
	}

	break;
	case WM_COMMAND:		//��������� ������� ������ � ������ �������� ������������ 
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LOGIN:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage((HWND)lParam, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			
			if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer,g_sz_LOGIN_INVITATION) == 0)
			{
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)"");
			}
			if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer,"") == 0)
			{
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
			}
			
		}
		break;

		case IDC_EDIT_PASWORD:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage((HWND)lParam, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_PASWORD_INVITATION) == 0)
			{
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)"");
			}
			if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
			{
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)g_sz_PASWORD_INVITATION);
			}
		}
		break;
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPasword = GetDlgItem(hwnd, IDC_EDIT_PASWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPasword, WM_SETTEXT,0, (LPARAM)sz_buffer);
		}
		break;

		case IDOK: MessageBox(hwnd, "���� ������ ������ OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;

		}
		break;
	case WM_CLOSE:		//��� ������� ������ "�������" X
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}