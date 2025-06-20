#include<Windows.h>
#include"resource.h"
#include<cstdio>
#include<WinUser.h>

CONST CHAR g_sz_CLASS_NAME[] = "MyCalc";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CreateButton(CONST CHAR* NAME_BUTTON, INT ID_BUTTON, HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1 Регистрация класса окна 
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "CLass registration faild", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2 Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3 Запуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
};

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		CreateWindowEx(
			NULL,
			TEXT("Edit"),
			TEXT("0"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			0, 0, 0, 0,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		CreateButton("0", IDC_BUTTON_0, hwnd);
		CreateButton("1", IDC_BUTTON_1, hwnd);
		CreateButton("2", IDC_BUTTON_2, hwnd);
		CreateButton("3", IDC_BUTTON_3, hwnd);
		CreateButton("4", IDC_BUTTON_4, hwnd);
		CreateButton("5", IDC_BUTTON_5, hwnd);
		CreateButton("6", IDC_BUTTON_6, hwnd);
		CreateButton("7", IDC_BUTTON_7, hwnd);
		CreateButton("8", IDC_BUTTON_8, hwnd);
		CreateButton("9", IDC_BUTTON_9, hwnd);

		CreateButton(".", IDC_BUTTON_POINT, hwnd);
		CreateButton("+", IDC_BUTTON_PLUS, hwnd);
		CreateButton("-", IDC_BUTTON_MINUS, hwnd);
		CreateButton("*", IDC_BUTTON_ASTER, hwnd);
		CreateButton("/", IDC_BUTTON_SLASH, hwnd);
		CreateButton("=", IDC_BUTTON_EQUAL, hwnd);
	}
	break;

	case WM_SIZE:
	{
		HFONT hFontBig = CreateFont(
			24,                        // Высота шрифта в логических единицах
			0,                         // Ширина (0 — автоматическая)
			0,                         // Угол наклона
			0,                         // Угол наклона
			FW_EXTRABOLD,                 // Толщина шрифта
			FALSE,                     // Курсив
			FALSE,                     // Подчеркивание
			FALSE,                     // Зачеркивание
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			TEXT("Arial")              // Имя шрифта
		);
		INT width_calc = LOWORD(lParam);
		INT height_calc = HIWORD(lParam);

		INT height_field_for_buttons = height_calc - 50;
		INT width_field_for_buttons = width_calc;

		INT height_button = height_field_for_buttons / 4;
		INT width_button = width_field_for_buttons / 4;

		INT array_buttons[4][4] =
		{
			{IDC_BUTTON_7 , IDC_BUTTON_8 , IDC_BUTTON_9 , IDC_BUTTON_MINUS},
			{IDC_BUTTON_4 , IDC_BUTTON_5 , IDC_BUTTON_6 , IDC_BUTTON_SLASH},
			{IDC_BUTTON_1 , IDC_BUTTON_2 , IDC_BUTTON_3 , IDC_BUTTON_ASTER},
			{IDC_BUTTON_0 , IDC_BUTTON_POINT , IDC_BUTTON_EQUAL , IDC_BUTTON_PLUS}
		};

		for (size_t y = 0; y < 4; ++y)
		{
			for (size_t x = 0; x < 4; ++x)
			{
				MoveWindow(GetDlgItem(hwnd, array_buttons[y][x]), width_button * x, height_button * y + 50, width_button, height_button, TRUE);
				SendMessage(GetDlgItem(hwnd, array_buttons[y][x]), WM_SETFONT, (WPARAM)hFontBig, TRUE);
			}
		}

		HWND hwndEdit = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		MoveWindow(hwndEdit, 10, 10, width_calc - 20, 30, TRUE);
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFontBig, TRUE);
	}
	break;

	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_buffer_edit[SIZE];
		CHAR sz_buffer_button[SIZE];
		if (LOWORD(wParam) >= 1000 && LOWORD(wParam) <= 1014)
		{
			HWND hwndEdit = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			HWND hwndButton = GetDlgItem(hwnd, LOWORD(wParam));

			SendMessage(hwndButton, WM_GETTEXT, SIZE, (LPARAM)sz_buffer_button);
			SendMessage(hwndEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer_edit);
			if (strcmp(sz_buffer_edit, "0") == 0)
				sprintf(sz_buffer_edit, "");
			strcat(sz_buffer_edit, sz_buffer_button);
			SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer_edit);
		}
	}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

VOID CreateButton(CONST CHAR* NAME_BUTTON, INT ID_BUTTON, HWND hwnd)
{
	if (ID_BUTTON != NULL && NAME_BUTTON != NULL)
	{
		CreateWindowEx(
			NULL,
			TEXT("BUTTON"),
			TEXT(NAME_BUTTON),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd,
			(HMENU)ID_BUTTON,
			GetModuleHandle(NULL),
			NULL
		);
	}
}
