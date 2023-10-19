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

private:
	Platform();

	static void HideConsoleWindows();
	static void HideConsoleUnix();

	static std::string LoadShaderWindows(int shaderType);
	static std::string LoadShaderUnix(const char* shaderPath);
	
	static std::filesystem::path GetDocumentsFolderWindows();
	static std::filesystem::path GetDocumentsFolderUnix();

	static HMODULE GCM();
};