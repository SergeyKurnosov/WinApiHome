#include<Windows.h>
#include<cstdio>
#include"resource.h"


CONST CHAR* G_SZ_VALUES[] = { "This" , "is" , "my" , "First" , "Combo" , "Box", "Хорошо", "Живет", "На" , "Свете" , "Винни" , "Пух" , "Кактотак" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgCHANGE(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		HWND hList = GetDlgItem(hwnd, ID_LIST);
		for (int i = 0; i < sizeof(G_SZ_VALUES) / sizeof(G_SZ_VALUES[0]); i++)
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)G_SZ_VALUES[i]);
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_LIST && HIWORD(wParam) == LBN_DBLCLK)
		{
			HWND hList = GetDlgItem(hwnd, ID_LIST);
			int index = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index != -1)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), NULL, (DLGPROC)DlgCHANGE, 0);
			}
		}
	}


	switch (LOWORD(wParam))
	{
	case IDOK:
	{
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		CHAR sz_message[SIZE] = {};
		HWND hList = GetDlgItem(hwnd, ID_LIST);

		INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
		SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);

		if (i == -1)strcpy(sz_message, "Выберите ваш вариант");
		else
			sprintf(sz_message, "Вы выбрали пункт %i со значением %s", i, sz_buffer);
		MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
	}
	break;
	case IDADD:
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), NULL, (DLGPROC)DlgADD, 0);
		break;
	case IDDELETE:
	{
		CONST INT SIZE = 256;
		HWND hList = GetDlgItem(hwnd, ID_LIST);
		INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
		SendMessage(hList, LB_DELETESTRING, i, 0);
	}
	break;

	case IDCANCEL:
		EndDialog(hwnd, 0);
		break;
	}

	break;


	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}


BOOL CALLBACK DlgADD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND mainHWND = FindWindow(NULL, "ListBox");
			HWND hList = GetDlgItem(mainHWND, ID_LIST);
			HWND hEditADD = GetDlgItem(hwnd, IDC_EDIT1);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditADD, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			EndDialog(hwnd, 0);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	}
	return FALSE;
}

BOOL CALLBACK DlgCHANGE(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND mainHWND = FindWindow(NULL, "ListBox");
			HWND hList = GetDlgItem(mainHWND, ID_LIST);
			HWND hEditCHANGE = GetDlgItem(hwnd, IDC_EDIT1);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditCHANGE, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			INT i = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, i, 0);
			SendMessage(hList, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
			EndDialog(hwnd, 0);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return FALSE;
}
