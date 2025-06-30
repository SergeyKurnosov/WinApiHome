#pragma once

#include<Windows.h>
#include<iostream>
#include <atlstr.h>
#include <string>

namespace Errors
{
	class LastError
	{
	public:
		static std::string FormatLastError(DWORD dwErrorID);
		static VOID PrintLastError(DWORD dwErrorID);
	};
}
