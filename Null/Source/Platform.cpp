#include "Headers/Platform.h"

#include <sstream>
#include <fstream>
#include <string>
#include <Windows.h>

#include "resource.h"

void Platform::HideConsole()
{
#ifdef WIN32
	HideConsoleWindows();
#else
	HideConsoleUnix();
#endif 
}

std::string Platform::LoadShader(const char* shaderPath, int shaderType)
{
#ifdef WIN32
	return LoadShaderWindows(shaderType);
#else
	return LoadShaderUnix(shaderPath);
#endif 
}



void Platform::HideConsoleWindows()
{
#ifdef _WIN32
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	FreeConsole();
#endif
}

void Platform::HideConsoleUnix()
{
}


std::string Platform::LoadShaderWindows(int shaderType)
{
#ifdef _WIN32
	HRSRC hRes = FindResource(GCM(), MAKEINTRESOURCE(shaderType), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL hData = LoadResource(GCM(), hRes);
	DWORD hSize = SizeofResource(GCM(), hRes);
	char* hFinal = (char*)LockResource(hData);

	std::string sourceString;
	sourceString.assign(hFinal, hSize);

	return sourceString;
#else
	return NULL;
#endif
}

std::string Platform::LoadShaderUnix(const char* shaderPath)
{
#ifdef _WIN32
	return NULL;
#else
	std::string line;

	std::ifstream vertexStream(shaderPath);
	std::stringstream vertexStringStream;
	while (getline(vertexStream, line))
		vertexStringStream << line << '\n';

	std::string sourceString = vertexStringStream.str();

	return sourceString;
#endif
}


HMODULE Platform::GCM()
{
#ifdef _WIN32
	HMODULE hModue = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModue
	);

	return hModue;
#else
	return NULL;
#endif
}