#include<Windows.h>
#include<stdio.h>

CONST CHAR g_sz_CLASS_NAME[] = "My First Window";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1/////////////////////////////////////////////////////////
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;    // �����
	wClass.cbSize = sizeof(wClass); // ������
	wClass.cbWndExtra = 0; // ��� ����� ����� ���������� ���� 
	wClass.cbClsExtra = 0; // ��� ����� ����� ��������� 

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ���������� ������
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // ���������� ���������� ������
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� ������� 
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW; // ���������� ������� ����� 

	wClass.hInstance = hInstance; // ���������� ����������
	wClass.lpfnWndProc = (WNDPROC)WndProc; // ��������� �� ��������� ����
	wClass.lpszMenuName = NULL; // ����������� ��� ������� ���� ������
	wClass.lpszClassName = g_sz_CLASS_NAME; // ��������� �� ��� ������ ����

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "CLass registration", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2///////////////////////////////////////////////////////////
	//RECT rect;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	//int screenWidth = rect.right - rect.left;
	//int screenHeight = rect.bottom - rect.top;

	//int windowWidth = (int)screenWidth * 0.75;
	//int windowHeight = (int)screenHeight * 0.75;

	//int xPos = rect.left + (screenWidth - windowWidth) / 2;
	//int yPos = rect.top + (screenHeight - windowHeight) / 2;

	//CONST INT SIZE = 256;

	//CHAR sz_message[256] = {};
	//sprintf(sz_message, "������: %dx%d, ���������: (%d,%d)", windowWidth, windowHeight, xPos, yPos);


	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);
	INT window_width = screen_width * .75;
	INT window_height = screen_height * 3 / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;



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
	//3///////////////////////////////////////////////////////////
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message;
}




INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
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
	}
	break;
	case WM_COMMAND:
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