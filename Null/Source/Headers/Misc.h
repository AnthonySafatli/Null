#pragma once

#include <string>
#include <map>
#include <vector>

#include "Command.h"
#include "TextEditor.h"

std::vector<std::string> Split(const std::string str, const char separator);
Colour ParseColour(const std::vector<std::string> args, const std::string commandName, const Colour defaultColour, const float defaultA);
std::map<std::string, Colour> GenerateColourMap();
void SaveFile(TextEditor** editor);
bool SavedSuccessfully(const std::string path);
bool isFloat(const std::string number);
void PrintColour(const std::string commandName, const Colour colour);