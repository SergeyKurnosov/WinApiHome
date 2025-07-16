#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include <commdlg.h>
#include <fstream>
#include <string>
#include "resource.h"


#define IDC_STATIC     1000
#define IDC_EDIT       1001
#define IDC_BUTTON     1002
#define IDC_BUTTON_C   1003

INT screen_width = GetSystemMetrics(SM_CXSCREEN);
INT screen_height = GetSystemMetrics(SM_CYSCREEN);

INT window_width = screen_width * .75;
INT window_height = screen_height * 3 / 4;
INT window_start_x = screen_width / 8;
INT window_start_y = screen_height / 8;

CONST CHAR g_sz_CLASS_NAME[] = "My First Window";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::string GetEditText(HWND hwndEdit);

void SaveTextFromEdit(HWND hwndEdit, OPENFILENAME ofn);
void SaveTextFromFile(HWND hwndEdit, OPENFILENAME ofn);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_BITCOIN));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_ATOM));
	wClass.hCursor = (HCURSOR)LoadImage(
		hInstance,
		"starcraft-original\\Working In Background.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_LOADFROMFILE
	);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "CLass registration", "", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		window_start_x, window_start_y,
		window_width, window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:
		CreateWindowEx
		(
			NULL,
			"Static",
			"Текстовый редактор",
			WS_CHILD | WS_VISIBLE,
			10, 5,
			550, 25,
			hwnd,
			(HMENU)IDC_STATIC,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
			10, 50,
			window_width-35, window_height - 100,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Button", "Сохранить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 30,
			110, 20,
			hwnd,
			(HMENU)IDC_BUTTON,
			GetModuleHandle(NULL),
			NULL
		);

		CreateWindowEx
		(
			NULL,
			"Button", "Открыть",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			130, 30,
			110, 20,
			hwnd,
			(HMENU)IDC_BUTTON_C,
			GetModuleHandle(NULL),
			NULL
		);

		break;
	case WM_MOVE:
	case WM_SIZE:
	{
		RECT window_rect;
		GetWindowRect(hwnd, &window_rect);
		INT window_width = window_rect.right - window_rect.left;
		INT window_height = window_rect.bottom - window_rect.top;
		CONST INT SIZE = 256;
		CHAR sz_title[SIZE] = {};
		sprintf
		(
			sz_title,
			"%s - Position:%ix%i, Size:%ix%i",
			g_sz_CLASS_NAME,
			window_rect.left, window_rect.top,
			window_width, window_height
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);

		MoveWindow(GetDlgItem(hwnd, IDC_EDIT), 10, 50, window_width - 35, window_height - 100, TRUE);
	}
	break;
	case WM_COMMAND:
	{
		OPENFILENAME ofn;
		char szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = "txt";

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SaveTextFromEdit(hEdit, ofn);
		}
		break;
		case IDC_BUTTON_C:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SaveTextFromFile(hEdit, ofn);
			SendMessage(GetDlgItem(hwnd, IDC_STATIC), WM_SETTEXT, 0, (LPARAM)ofn.lpstrFile);


		}
		break;
		}
	}

	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

std::string GetEditText(HWND hwndEdit) {
	int length = GetWindowTextLength(hwndEdit);
	if (length == 0) return "";

	char* buffer = new char[length + 1];
	GetWindowText(hwndEdit, buffer, length + 1);
	std::string text(buffer);
	delete[] buffer;
	return text;
}


void SaveTextFromEdit(HWND hwndEdit, OPENFILENAME ofn)
{
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn) == TRUE) {
		std::string textToSave = GetEditText(hwndEdit);

		std::ofstream outFile(ofn.lpstrFile);
		if (outFile.is_open()) {
			outFile << textToSave;
			outFile.close();
		}
		else {
			MessageBox(NULL, "Не удалось открыть файл для записи.", "Ошибка", MB_OK | MB_ICONERROR);
		}
	}
}

void SaveTextFromFile(HWND hwndEdit, OPENFILENAME ofn)
{
	if (GetSaveFileName(&ofn) == TRUE) {
		std::ifstream outFile(ofn.lpstrFile);
		if (outFile.is_open()) {
			std::string file_buffer;

			while (std::getline(outFile, file_buffer, '\n'))
			{
				file_buffer += '\n';
				CONST CHAR* sz_buffer = file_buffer.c_str();
				SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
				SendMessage(hwndEdit, EM_REPLACESEL, TRUE, (LPARAM)sz_buffer);

			}

			outFile.close();
		}
		else {
			MessageBox(NULL, "Не удалось открыть файл для записи.", "Ошибка", MB_OK | MB_ICONERROR);
		}
	}

}
