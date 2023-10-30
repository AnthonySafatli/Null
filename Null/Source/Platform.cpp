#include "Headers/Platform.h"

#include <sstream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <shlobj.h>

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

std::filesystem::path Platform::GetDocumentsFolder()
{
#ifdef _WIN32
	return GetDocumentsFolderWindows();
#else
	return GetDocumentsFolderUnix();
#endif
}

// TODO: Implement
std::string Platform::LoadSettingsFile()
{
	return std::string();
}

// TODO: Implement
bool Platform::SaveSettingsFile()
{
	return false;
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
	return std::string();
#endif
}

std::string Platform::LoadShaderUnix(const char* shaderPath)
{
#ifdef _WIN32
	return std::string();
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


std::filesystem::path Platform::GetDocumentsFolderWindows()
{
#ifdef _WIN32
	std::filesystem::path documentsPath;

	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		documentsPath = path;
		return documentsPath;
	}
	else
		return std::filesystem::path();
#else
	return std::filesystem::path();
#endif
}

std::filesystem::path Platform::GetDocumentsFolderUnix()
{
#ifdef _WIN32
	return std::filesystem::path();
#else
	const char* homeDir = std::getenv("HOME");
	if (homeDir == nullptr)
		return std::filesystem::path();
	else
		return std::filesystem::path(homeDir) / "Documents";
#endif
}


std::string Platform::LoadSettingsFileWindows()
{
	return std::string();
}

std::string Platform::LoadSettingsFileUnix()
{
	return std::string();
}


bool Platform::SaveSettingsFileWindows()
{
	return false;
}

bool Platform::SaveSettingsFileUnix()
{
	return false;
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