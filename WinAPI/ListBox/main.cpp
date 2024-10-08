#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include "resource.h"
#include<stdio.h>

CONST CHAR* g_LIST_BOX_ITEMS[] = { "This","is","my","First","List","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK DlgProcADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(g_LIST_BOX_ITEMS)/sizeof(g_LIST_BOX_ITEMS[0]); i++)
		{
			SendMessage(hCombo, LB_ADDSTRING, 0, (LPARAM)g_LIST_BOX_ITEMS[i]);
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hCombo, LB_GETCURSEL, 0, 0);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hCombo, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE]{"Вы ничего не выбрали"};

			if (i != LB_ERR)
			sprintf(sz_message, "Вы выбрали элемент № %i со значеинем %s", i, (LPARAM)sz_buffer);
			MessageBox(hwnd, sz_message, "Info!", MB_OK | MB_ICONINFORMATION);

		}
		break;
		case IDC_BUTTON_DELETE:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, i, 0);

		}
		break;
		case IDC_BUTTON_ADD:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, (DLGPROC)DlgProcADD, 0);

		}
		break;
		case IDC_LIST1:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, (DLGPROC)DlgProcChange,0);
			}
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);

		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)

	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_NAME));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessageA(GetDlgItem(hwnd, IDC_EDIT_NAME), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST1);
			if (strcmp(sz_buffer,"") == 0)
				MessageBox(hwnd, "Строка пустая", "Info!", MB_OK | MB_ICONINFORMATION);
			
			else if (SendMessage(hListBox,LB_FINDSTRING,-1,(LPARAM)sz_buffer) == LB_ERR)
			{
				SendMessageA(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			}
			else
				MessageBox(hwnd, "Такой элемент уже существует!", "ERROR!", MB_OK | MB_ICONERROR);
			
		}
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
			 
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменить");
		SendMessageA(GetDlgItem(hwnd, IDOK), WM_SETTEXT, 0, (LPARAM)"Сохранить");
		SendMessageA(GetDlgItem(hwnd, IDC_STATIC_INVITE), WM_SETTEXT, 0, (LPARAM)"Внесите изменения");
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		HWND hListBox = GetDlgItem(GetParent(hwnd), IDC_LIST1);
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_NAME);
		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessageA(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessageA(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		SendMessageA(hEdit, EM_SETSEL, 0, SendMessageA(hEdit, WM_GETTEXTLENGTH, 0, 0));
		SetFocus(hEdit);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hListBox = GetDlgItem(GetParent(hwnd), IDC_LIST1);
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_NAME);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessageA(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			
			if (strcmp(sz_buffer, "") == 0)
				MessageBox(hwnd, "Строка пустая", "Info!", MB_OK | MB_ICONINFORMATION);

			else if (SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == LB_ERR)
			{
				SendMessageA(hListBox,LB_DELETESTRING,i,0);
				SendMessageA(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			}
			else
				MessageBox(hwnd, "Такой элемент уже существует!", "ERROR!", MB_OK | MB_ICONERROR);

		}		
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);

		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
