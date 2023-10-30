#pragma once

#include <string>
#include <filesystem>
#include <Windows.h>

class Platform 
{
public:
	static void HideConsole();
	static std::string LoadShader(const char* shaderPath, int shaderType);
	static std::filesystem::path GetDocumentsFolder();
	static std::string LoadSettingsFile();
	static bool SaveSettingsFile();

private:
	Platform();

	static void HideConsoleWindows();
	static void HideConsoleUnix();

	static std::string LoadShaderWindows(int shaderType);
	static std::string LoadShaderUnix(const char* shaderPath);
	
	static std::filesystem::path GetDocumentsFolderWindows();
	static std::filesystem::path GetDocumentsFolderUnix();

	static std::string LoadSettingsFileWindows();
	static std::string LoadSettingsFileUnix();

	static bool SaveSettingsFileWindows();
	static bool SaveSettingsFileUnix();

	static HMODULE GCM();
};