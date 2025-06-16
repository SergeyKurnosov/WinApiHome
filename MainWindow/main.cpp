#include<Windows.h>
#include<cstdio>

CONST CHAR g_sz_CLASS_NAME[] = "My First Window";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1/////////////////////////////////////////////////////////
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;    // стиль
	wClass.cbSize = sizeof(wClass); // размер
	wClass.cbWndExtra = 0; // доп байты после экзэмрляра окна 
	wClass.cbClsExtra = 0; // доп байты после структуры 

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // дескриптор значка
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // дескриптор небольшого значка
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора 
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW; // дескрипотр фоновой кисти 

	wClass.hInstance = hInstance; // дескриптор экземпляра
	wClass.lpfnWndProc = (WNDPROC)WndProc; // указатель на процедуру окна
	wClass.lpszMenuName = NULL; // указательна имя ресурса меню класса
	wClass.lpszClassName = g_sz_CLASS_NAME; // указатель на имя класса окна

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "CLass registration", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2///////////////////////////////////////////////////////////

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	int screenWidth = rect.right - rect.left;
	int screenHeight = rect.bottom - rect.top;

	int windowWidth = (int)screenWidth * 0.75;
	int windowHeight = (int)screenHeight * 0.75;

	int xPos = rect.left + (screenWidth - windowWidth) / 2;
	int yPos = rect.top + (screenHeight - windowHeight) / 2;

	CONST INT SIZE = 256;

	CHAR sz_message[256] = {};
	sprintf(sz_message, "Размер: %dx%d, Положение: (%d,%d)", windowWidth, windowHeight, xPos, yPos);

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		sz_message,
		WS_OVERLAPPEDWINDOW,
		xPos, 
		yPos,
		windowWidth,
		windowHeight,
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