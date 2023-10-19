#pragma once

#include <string>

#include <Windows.h>

class Platform 
{
public:
	static void HideConsole();
	static std::string LoadShader(const char* shaderPath, int shaderType);

private:
	Platform();

	static void HideConsoleWindows();
	static void HideConsoleUnix();

	static std::string LoadShaderWindows(int shaderType);
	static std::string LoadShaderUnix(const char* shaderPath);

	static HMODULE GCM();
};