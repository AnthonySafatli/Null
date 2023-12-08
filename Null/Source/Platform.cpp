#include "Headers/Platform.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <shlobj.h>

#include "resource.h"
#include "Headers/Program.h"

extern Program program;

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

std::string Platform::LoadSettingsFile()
{
#ifdef _WIN32
	return LoadSettingsFileWindows();
#else
	return LoadSettingsFileUnix();
#endif
}

bool Platform::SaveSettingsFile()
{
#ifdef _WIN32
	return SaveSettingsFileWindows();
#else
	return SaveSettingsFileUnix();
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

std::filesystem::path Platform::GetSettingsFolderWindows()
{
#ifdef _WIN32
	std::filesystem::path appDataPath;

	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		appDataPath = path;
		return appDataPath.parent_path() / "Local" / "Null";
	}
	else
		return std::filesystem::path();
#else
	return std::filesystem::path();
#endif
}

std::filesystem::path Platform::GetSettingsFolderUnix()
{
	return std::filesystem::path();
}

std::string Platform::LoadSettingsFileWindows()
{
	std::filesystem::path settingsPath = GetSettingsFolderWindows() / "settings";
	if (settingsPath.empty() || !std::filesystem::exists(settingsPath))
		return std::string();

	std::ifstream file(settingsPath);

	if (!file.is_open() || !file.good() || file.bad())
		return std::string();

	std::string settings;
	std::string settingsLine;
	try 
	{
		while (file >> settingsLine) 
		{
			settings.append(settingsLine + "\n");
		}
	} 
	catch (std::exception e) 
	{
		return std::string();
	}

	file.close();

	return settings;
}

std::string Platform::LoadSettingsFileUnix()
{
	return std::string();
}


bool Platform::SaveSettingsFileWindows()
{
	std::stringstream settings;
	settings << program.textSize;
	settings << "\n" << program.background.r << "\n" << program.background.g << "\n" << program.background.b << "\n" << program.background.a;
	settings << "\n" << program.foreground.r << "\n" << program.foreground.g << "\n" << program.foreground.b << "\n" << program.foreground.a;
	settings << "\n" << program.cursorSpeed;

	std::filesystem::path settingsPath = GetSettingsFolderWindows();
	if (!std::filesystem::exists(settingsPath))
	{
		if (!std::filesystem::create_directory(settingsPath))
			return false;
	}

	std::ofstream file(settingsPath / "settings");

	if (!file.is_open() || !file.good() || file.bad()) 
		return false;

	try 
	{
		file << settings.str();
	}
	catch (const std::exception& e) 
	{
		return false;
	}

	file.close();

	if (!file.good() || file.bad())
		return false;

	return true;
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