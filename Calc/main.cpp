#include<Windows.h>
#include"resource.h"
#include<cstring>
#include<string>
#include<cstdio>
#include<sstream>

CONST CHAR g_sz_CLASS_NAME[] = "MyCalc";

CONST CHAR* g_sz_OPERATIONS[] = { "+","-","*","/" };
CONST CHAR* g_sz_EDIT[] = { "<-","C","=" };

//g_i_ - Global Integer
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 1;
CONST INT g_i_BUTTON_SPACE = g_i_BUTTON_SIZE + g_i_INTERVAL;
//CONST INT g_i_BUTTON_SPACE_DOUBLE = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 2;

CONST INT g_i_BUTTON_SIZE_DOUBLE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_DISPLAY_HEIGHT = 22;
CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + 2 * g_i_START_X + 16;
CONST INT g_i_WINDOW_HEIGHT = (g_i_DISPLAY_HEIGHT + g_i_INTERVAL) + g_i_BUTTON_SPACE * 4 + 2 * g_i_START_Y + 24 + 16;

CONST INT g_SIZE = 256;


INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CHECK_STR_BY_OPERATION(CHAR* str);
DOUBLE GET_RESULT_OPERATION(CHAR* str);

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
		//	WS_OVERLAPPED | WS_SYSMENU /*| WS_THICKFRAME*/  | WS_MINIMIZEBOX | /*WS_MAXIMIZEBOX*/,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
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
		HWND hEditDisplay = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_BUTTON_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		INT iDigit = IDC_BUTTON_1;
		CHAR szDigit[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				szDigit[0] = iDigit - IDC_BUTTON_0 + '0';
				CreateWindowEx
				(
					NULL, "Button", szDigit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i / 3,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)iDigit,
					GetModuleHandle(NULL),
					NULL
				);
				iDigit++;
			}
		}

		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SPACE) * 3,
			g_i_BUTTON_SIZE_DOUBLE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X + g_i_BUTTON_SPACE * 2,
			g_i_BUTTON_START_Y + g_i_BUTTON_SPACE * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		/////////////////////////////////////////////////////////////////
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_sz_OPERATIONS[i],
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				g_i_BUTTON_START_X + g_i_BUTTON_SPACE * 3, g_i_BUTTON_START_Y + g_i_BUTTON_SPACE * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}

		for (int i = 0; i < 3; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_sz_EDIT[i],
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				g_i_BUTTON_START_X + g_i_BUTTON_SPACE * 4,
				g_i_BUTTON_START_Y + g_i_BUTTON_SPACE * i,
				g_i_BUTTON_SIZE, i < 2 ? g_i_BUTTON_SIZE : g_i_BUTTON_SIZE_DOUBLE,
				hwnd,
				(HMENU)(IDC_BUTTON_BSP + i),
				GetModuleHandle(NULL),
				NULL
			);
		}

	}
	break;
	case WM_COMMAND:
	{
		CHAR szDisplay[g_SIZE] = {};
		CHAR szDigit[2] = {};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);

		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_POINT)
		{
			if (LOWORD(wParam) == IDC_BUTTON_POINT)
				szDigit[0] = '.';
			else
				szDigit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);
			if (szDisplay[0] == '0' && szDisplay[1] != '.')szDisplay[0] = 0;
			//	if (szDigit[0] == '.' && strchr(szDisplay, '.'))break;
			strcat(szDisplay, szDigit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)szDisplay);
		}
		//////////////////////////////////////////////////////////////////////
		if (LOWORD(wParam) == IDC_BUTTON_PLUS)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);

			if (CHECK_STR_BY_OPERATION(szDisplay))
			{
				char buffer[g_SIZE];
				double result = GET_RESULT_OPERATION(szDisplay);
				result - static_cast<int>(result) > 0 ?
					snprintf(buffer, sizeof(buffer), "%.1f", result) :
					sprintf(buffer, "%d", static_cast<int>(result));
				strcat(buffer, "+");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)buffer);
				break;
			}
			else
			{
				strcat(szDisplay, "+");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)szDisplay);
			}
		}

		if (LOWORD(wParam) == IDC_BUTTON_MINUS)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);

			if (CHECK_STR_BY_OPERATION(szDisplay))
			{
				char buffer[g_SIZE];
				double result = GET_RESULT_OPERATION(szDisplay);
				result - static_cast<int>(result) > 0 ?
					snprintf(buffer, sizeof(buffer), "%.1f", result) :
					sprintf(buffer, "%d", static_cast<int>(result));
				strcat(buffer, "-");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)buffer);
				break;
			}
			else
			{
				strcat(szDisplay, "-");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)szDisplay);
			}
		}

		if (LOWORD(wParam) == IDC_BUTTON_ASTER)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);

			if (CHECK_STR_BY_OPERATION(szDisplay))
			{
				char buffer[g_SIZE];
				double result = GET_RESULT_OPERATION(szDisplay);
				result - static_cast<int>(result) > 0 ?
					snprintf(buffer, sizeof(buffer), "%.1f", result) :
					sprintf(buffer, "%d", static_cast<int>(result));
				strcat(buffer, "*");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)buffer);
				break;
			}
			else
			{
				strcat(szDisplay, "*");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)szDisplay);
			}
		}

		if (LOWORD(wParam) == IDC_BUTTON_SLASH)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);

			if (CHECK_STR_BY_OPERATION(szDisplay))
			{
				char buffer[g_SIZE];
				double result = GET_RESULT_OPERATION(szDisplay);
				result - static_cast<int>(result) > 0 ?
					snprintf(buffer, sizeof(buffer), "%.1f", result) :
					sprintf(buffer, "%d", static_cast<int>(result));
				strcat(buffer, "/");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)buffer);
				break;
			}
			else
			{
				strcat(szDisplay, "/");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)szDisplay);
			}
		}

		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);
			if (strlen(szDisplay) >= 2)szDisplay[strlen(szDisplay) - 1] = '\0';
			else wParam = IDC_BUTTON_CLR;

			SendMessage(hEditDisplay, WM_SETTEXT, strlen(szDisplay) - 1, (LPARAM)szDisplay);
		}

		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}

		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, g_SIZE, (LPARAM)szDisplay);
			if (CHECK_STR_BY_OPERATION(szDisplay))
			{
				char buffer[g_SIZE];
				double result = GET_RESULT_OPERATION(szDisplay);
				result - static_cast<int>(result) > 0 ?
					snprintf(buffer, sizeof(buffer), "%.1f", result) :
					sprintf(buffer, "%d", static_cast<int>(result));
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)buffer);
			}
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


BOOL CHECK_STR_BY_OPERATION(CHAR* str)
{
	if (strchr(str, '+') || strchr(str, '-') || strchr(str, '*') || strchr(str, '/'))
		return TRUE;
	else
		return FALSE;
}

DOUBLE GET_RESULT_OPERATION(CHAR* str)
{
	CHAR* str_copy = (CHAR*)malloc(strlen(str) + 1);
	DOUBLE left = 0, right = 0, result = 0;
	try
	{
		if (str_copy != NULL)
		{
			strncpy(str_copy, str, strlen(str) + 1);
			strtok(str_copy, "+-*/");
			left = std::stod(str_copy);
			str_copy = strtok(NULL, "+-*/");
			if (str_copy != NULL)
			{
				right = std::stod(str_copy);
			}
		
			if (strchr(str, '+'))result = left + right;
			else if (strchr(str, '-'))result = left - right;
			else if (strchr(str, '*'))result = left * right;
			else if (strchr(str, '/') && right != 0)result = left / right;
		}

	}
	catch (...)
	{
		result = 0;
	}
	return result;
}