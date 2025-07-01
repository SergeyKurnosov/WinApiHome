#include "MathLibrary.h"

std::string Errors::LastError::FormatLastError(DWORD dwErrorID)
{

	LPSTR lpszMessage = nullptr;
	size_t size = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA),
		(LPSTR)&lpszMessage,
		0,
		NULL
	);

	std::cout << lpszMessage << std::endl;

	std::string message(lpszMessage , size);
	if (size && lpszMessage) {
		LocalFree(lpszMessage);
	}
	else {
		message = "Не удалось получить описание ошибки.";
	}

	return message;
}

VOID Errors::LastError::PrintLastError(DWORD dwErrorID)
{
	std::cout << FormatLastError(dwErrorID) << std::endl;
}
