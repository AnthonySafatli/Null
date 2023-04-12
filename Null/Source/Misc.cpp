#include "Headers/Misc.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "NFD/nfd.h"

#include "Headers/Program.h"

extern Program program;

Colour ParseColour(const std::vector<std::string> args, const std::string commandName, const Colour defaultColour, const float defaultA)
{
	Colour colour;

	if (args.size() > 4 || args.size() < 1)
	{
		colour.error = true;
		program.RenderStatus("Command '" + commandName + "' can only take 1-4 arguments");
		return colour;
	}

	if (!isFloat(args[0]))
	{
		if (args.size() == 1 && args[0] == "default")
		{
			PrintColour(commandName, defaultColour);
			return defaultColour;
		}

		std::map<std::string, Colour> colourMap = GenerateColourMap();
		if (colourMap.find(args[0]) != colourMap.end())
		{
			colour = colourMap[args[0]];
			colour.a = defaultA;

			if (args.size() == 2)
			{
				if (!isFloat(args[1]))
				{
					colour.error = true;
					program.RenderStatus("Error invalid argument after colour value");
					return colour;
				}

				colour.a = std::stof(args[1]);
			}

			PrintColour(commandName, colour);
			return colour;
		}

		colour.error = true;
		program.RenderStatus("Colour " + args[0] + " not found");
		return colour;
	}

	colour.error = true;
	try
	{
		if (args.size() == 1)      // x
		{
			colour.error = false;
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[0]);
			colour.b = std::stof(args[0]);
			colour.a = 1.0f;
		}
		else if (args.size() >= 3) // r g b
		{
			colour.error = false;
			colour.r = std::stof(args[0]);
			colour.g = std::stof(args[1]);
			colour.b = std::stof(args[2]);
			colour.a = 1.0f;
		}

		if (args.size() == 4)      // r g b a
		{
			colour.a = std::stof(args[3]);
		}

		if (colour.error)
		{
			program.RenderStatus("Error Invalid Argument(s)"); // invalid amount of numbers
			return colour;
		}
	}
	catch (const std::exception& e)
	{
		program.RenderStatus("Error Invalid Argument(s)");     // not all args are numbers
		return colour;
	}

	colour.error = false;
	PrintColour(commandName, colour);
	return colour;
}

std::vector<std::string> Split(const std::string str, const char separator)
{
	std::vector<std::string> strings;

	std::stringstream ss;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != separator)
		{
			ss << str[i];
			continue;
		}

		if (ss.str().size() > 0)
		{
			strings.push_back(ss.str());
			ss.clear();
			ss.str("");
		}
	}

	if (ss.str().size() > 0)
		strings.push_back(ss.str());

	return strings;
}

// Colours list https://www.w3schools.com/cssref/css_colors.php
std::map<std::string, Colour> GenerateColourMap()
{
	std::map<std::string, Colour> colours;

	colours["aliceblue"] = Colour(240.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0);
	colours["antiquewhite"] = Colour(250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0);
	colours["aqua"] = Colour(000.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["aquamarine"] = Colour(127.0 / 255.0, 255.0 / 255.0, 212.0 / 255.0);
	colours["azure"] = Colour(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["beige"] = Colour(245.0 / 255.0, 245.0 / 255.0, 220.0 / 255.0);
	colours["bisque"] = Colour(255.0 / 255.0, 228.0 / 255.0, 196.0 / 255.0);
	colours["black"] = Colour(000.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["blanchedalmond"] = Colour(255.0 / 255.0, 235.0 / 255.0, 205.0 / 255.0);
	colours["blue"] = Colour(000.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["blueviolet"] = Colour(135.0 / 255.0, 043.0 / 255.0, 226.0 / 255.0);
	colours["brown"] = Colour(165.0 / 255.0, 042.0 / 255.0, 042.0 / 255.0);
	colours["burlywood"] = Colour(222.0 / 255.0, 184.0 / 255.0, 135.0 / 255.0);
	colours["cadetblue"] = Colour(095.0 / 255.0, 158.0 / 255.0, 160.0 / 255.0);
	colours["chartreuse"] = Colour(127.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["chocolate"] = Colour(210.0 / 255.0, 105.0 / 255.0, 030.0 / 255.0);
	colours["coral"] = Colour(255.0 / 255.0, 127.0 / 255.0, 080.0 / 255.0);
	colours["cornflowerblue"] = Colour(100.0 / 255.0, 219.0 / 255.0, 237.0 / 255.0);
	colours["cornsilk"] = Colour(250.0 / 255.0, 248.0 / 255.0, 220.0 / 255.0);
	colours["crimson"] = Colour(220.0 / 255.0, 020.0 / 255.0, 060.0 / 255.0);
	colours["cyan"] = Colour(000.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["darkblue"] = Colour(000.0 / 255.0, 000.0 / 255.0, 139.0 / 255.0);
	colours["darkcyan"] = Colour(000.0 / 255.0, 139.0 / 255.0, 139.0 / 255.0);
	colours["darkgoldenrod"] = Colour(184.0 / 255.0, 134.0 / 255.0, 110.0 / 255.0);
	colours["darkgrey"] = Colour(169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0);
	colours["darkgray"] = Colour(169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0);
	colours["darkgreen"] = Colour(000.0 / 255.0, 100.0 / 255.0, 000.0 / 255.0);
	colours["darkkhaki"] = Colour(189.0 / 255.0, 183.0 / 255.0, 107.0 / 255.0);
	colours["darkmagenta"] = Colour(139.0 / 255.0, 000.0 / 255.0, 139.0 / 255.0);
	colours["darkolivegreen"] = Colour(085.0 / 255.0, 107.0 / 255.0, 047.0 / 255.0);
	colours["darkorange"] = Colour(255.0 / 255.0, 140.0 / 255.0, 000.0 / 255.0);
	colours["darkorchid"] = Colour(153.0 / 255.0, 050.0 / 255.0, 204.0 / 255.0);
	colours["darkred"] = Colour(139.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["darksalmon"] = Colour(233.0 / 255.0, 150.0 / 255.0, 122.0 / 255.0);
	colours["darkseagreen"] = Colour(143.0 / 255.0, 188.0 / 255.0, 143.0 / 255.0);
	colours["darkslateblue"] = Colour(072.0 / 255.0, 061.0 / 255.0, 139.0 / 255.0);
	colours["darkslategrey"] = Colour(047.0 / 255.0, 079.0 / 255.0, 079.0 / 255.0);
	colours["darkslategray"] = Colour(047.0 / 255.0, 079.0 / 255.0, 079.0 / 255.0);
	colours["darkturqoise"] = Colour(000.0 / 255.0, 206.0 / 255.0, 209.0 / 255.0);
	colours["darkviolet"] = Colour(148.0 / 255.0, 000.0 / 255.0, 211.0 / 255.0);
	colours["deeppink"] = Colour(255.0 / 255.0, 020.0 / 255.0, 147.0 / 255.0);
	colours["deepskyblue"] = Colour(000.0 / 255.0, 191.0 / 255.0, 255.0 / 255.0);
	colours["dimgray"] = Colour(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
	colours["dimgrey"] = Colour(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
	colours["dodgerblue"] = Colour(030.0 / 255.0, 144.0 / 255.0, 255.0 / 255.0);
	colours["firebrick"] = Colour(178.0 / 255.0, 034.0 / 255.0, 034.0 / 255.0);
	colours["floralwhite"] = Colour(255.0 / 255.0, 250.0 / 255.0, 240.0 / 255.0);
	colours["forestgreen"] = Colour(034.0 / 255.0, 139.0 / 255.0, 034.0 / 255.0);
	colours["fuchsia"] = Colour(255.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["gainsboro"] = Colour(220.0 / 255.0, 220.0 / 255.0, 220.0 / 255.0);
	colours["ghostwhite"] = Colour(248.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0);
	colours["gold"] = Colour(255.0 / 255.0, 215.0 / 255.0, 000.0 / 255.0);
	colours["goldenrod"] = Colour(218.0 / 255.0, 165.0 / 255.0, 032.0 / 255.0);
	colours["gray"] = Colour(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["grey"] = Colour(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["green"] = Colour(000.0 / 255.0, 128.0 / 255.0, 000.0 / 255.0);
	colours["greenyellow"] = Colour(173.0 / 255.0, 255.0 / 255.0, 047.0 / 255.0);
	colours["honeydew"] = Colour(240.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0);
	colours["hotpink"] = Colour(255.0 / 255.0, 105.0 / 255.0, 180.0 / 255.0);
	colours["indianred"] = Colour(205.0 / 255.0, 092.0 / 255.0, 092.0 / 255.0);
	colours["indigo"] = Colour(075.0 / 255.0, 000.0 / 255.0, 130.0 / 255.0);
	colours["ivory"] = Colour(255.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0);
	colours["khaki"] = Colour(240.0 / 255.0, 230.0 / 255.0, 140.0 / 255.0);
	colours["lavender"] = Colour(230.0 / 255.0, 230.0 / 255.0, 250.0 / 255.0);
	colours["lavenderblush"] = Colour(255.0 / 255.0, 240.0 / 255.0, 245.0 / 255.0);
	colours["lawngreen"] = Colour(124.0 / 255.0, 252.0 / 255.0, 000.0 / 255.0);
	colours["lemonchiffon"] = Colour(255.0 / 255.0, 250.0 / 255.0, 205.0 / 255.0);
	colours["lightblue"] = Colour(173.0 / 255.0, 216.0 / 255.0, 230.0 / 255.0);
	colours["lightcoral"] = Colour(240.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["lightcyan"] = Colour(224.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["lightgoldenrodyellow"] = Colour(250.0 / 255.0, 250.0 / 255.0, 210.0 / 255.0);
	colours["lightgrey"] = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightgray"] = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightgreen"] = Colour(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);
	colours["lightpink"] = Colour(255.0 / 255.0, 182.0 / 255.0, 193.0 / 255.0);
	colours["lightsalmon"] = Colour(255.0 / 255.0, 160.0 / 255.0, 122.0 / 255.0);
	colours["lightseagreen"] = Colour(032.0 / 255.0, 178.0 / 255.0, 170.0 / 255.0);
	colours["lightskyblue"] = Colour(135.0 / 255.0, 206.0 / 255.0, 250.0 / 255.0);
	colours["lightslategray"] = Colour(119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0);
	colours["lightslategray"] = Colour(119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0);
	colours["lightsleelblue"] = Colour(176.0 / 255.0, 196.0 / 255.0, 222.0 / 255.0);
	colours["lightyellow"] = Colour(255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0);
	colours["lime"] = Colour(000.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["limegreen"] = Colour(050.0 / 255.0, 205.0 / 255.0, 050.0 / 255.0);
	colours["linen"] = Colour(250.0 / 255.0, 240.0 / 255.0, 230.0 / 255.0);
	colours["megenta"] = Colour(255.0 / 255.0, 000.0 / 255.0, 255.0 / 255.0);
	colours["maroon"] = Colour(128.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["mediumaquamarine"] = Colour(102.0 / 255.0, 205.0 / 255.0, 170.0 / 255.0);
	colours["mediumblue"] = Colour(000.0 / 255.0, 000.0 / 255.0, 205.0 / 255.0);
	colours["mediumorchid"] = Colour(186.0 / 255.0, 085.0 / 255.0, 211.0 / 255.0);
	colours["mediumpurple"] = Colour(147.0 / 255.0, 112.0 / 255.0, 219.0 / 255.0);
	colours["mediumseagreen"] = Colour(060.0 / 255.0, 179.0 / 255.0, 113.0 / 255.0);
	colours["mediumslateblue"] = Colour(123.0 / 255.0, 104.0 / 255.0, 238.0 / 255.0);
	colours["mediumspringgreen"] = Colour(000.0 / 255.0, 250.0 / 255.0, 154.0 / 255.0);
	colours["mediumturquoise"] = Colour(072.0 / 255.0, 209.0 / 255.0, 204.0 / 255.0);
	colours["mediumvioletred"] = Colour(199.0 / 255.0, 021.0 / 255.0, 133.0 / 255.0);
	colours["midnightblue"] = Colour(025.0 / 255.0, 025.0 / 255.0, 112.0 / 255.0);
	colours["mintcream"] = Colour(245.0 / 255.0, 255.0 / 255.0, 250.0 / 255.0);
	colours["mistyrose"] = Colour(255.0 / 255.0, 228.0 / 255.0, 225.0 / 255.0);
	colours["moccasin"] = Colour(255.0 / 255.0, 228.0 / 255.0, 181.0 / 255.0);
	colours["novajowhite"] = Colour(255.0 / 255.0, 222.0 / 255.0, 173.0 / 255.0);
	colours["navy"] = Colour(000.0 / 255.0, 000.0 / 255.0, 128.0 / 255.0);
	colours["oldlace"] = Colour(253.0 / 255.0, 245.0 / 255.0, 230.0 / 255.0);
	colours["olive"] = Colour(128.0 / 255.0, 128.0 / 255.0, 000.0 / 255.0);
	colours["olivedrap"] = Colour(107.0 / 255.0, 142.0 / 255.0, 035.0 / 255.0);
	colours["orange"] = Colour(255.0 / 255.0, 165.0 / 255.0, 000.0 / 255.0);
	colours["orangered"] = Colour(255.0 / 255.0, 069.0 / 255.0, 000.0 / 255.0);
	colours["orchid"] = Colour(218.0 / 255.0, 112.0 / 255.0, 214.0 / 255.0);
	colours["palegoldenrod"] = Colour(238.0 / 255.0, 232.0 / 255.0, 170.0 / 255.0);
	colours["palegreen"] = Colour(152.0 / 255.0, 251.0 / 255.0, 152.0 / 255.0);
	colours["paleturquoise"] = Colour(175.0 / 255.0, 238.0 / 255.0, 238.0 / 255.0);
	colours["palevioletred"] = Colour(219.0 / 255.0, 112.0 / 255.0, 147.0 / 255.0);
	colours["papayawhip"] = Colour(255.0 / 255.0, 239.0 / 255.0, 213.0 / 255.0);
	colours["peachpuff"] = Colour(255.0 / 255.0, 218.0 / 255.0, 185.0 / 255.0);
	colours["peru"] = Colour(205.0 / 255.0, 133.0 / 255.0, 063.0 / 255.0);
	colours["pink"] = Colour(255.0 / 255.0, 192.0 / 255.0, 203.0 / 255.0);
	colours["plum"] = Colour(221.0 / 255.0, 160.0 / 255.0, 221.0 / 255.0);
	colours["powderblue"] = Colour(176.0 / 255.0, 224.0 / 255.0, 230.0 / 255.0);
	colours["purple"] = Colour(128.0 / 255.0, 000.0 / 255.0, 128.0 / 255.0);
	colours["rebeccapurple"] = Colour(102.0 / 255.0, 051.0 / 255.0, 153.0 / 255.0);
	colours["red"] = Colour(255.0 / 255.0, 000.0 / 255.0, 000.0 / 255.0);
	colours["rosybrown"] = Colour(188.0 / 255.0, 143.0 / 255.0, 143.0 / 255.0);
	colours["royalblue"] = Colour(065.0 / 255.0, 105.0 / 255.0, 225.0 / 255.0);
	colours["saddlebrown"] = Colour(139.0 / 255.0, 069.0 / 255.0, 019.0 / 255.0);
	colours["salmon"] = Colour(250.0 / 255.0, 128.0 / 255.0, 114.0 / 255.0);
	colours["sandybrown"] = Colour(244.0 / 255.0, 164.0 / 255.0, 096.0 / 255.0);
	colours["seagreen"] = Colour(046.0 / 255.0, 139.0 / 255.0, 087.0 / 255.0);
	colours["seashell"] = Colour(255.0 / 255.0, 245.0 / 255.0, 238.0 / 255.0);
	colours["sienna"] = Colour(160.0 / 255.0, 082.0 / 255.0, 045.0 / 255.0);
	colours["silver"] = Colour(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
	colours["skyblue"] = Colour(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0);
	colours["slateblue"] = Colour(106.0 / 255.0, 090.0 / 255.0, 205.0 / 255.0);
	colours["slategrey"] = Colour(112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0);
	colours["slategray"] = Colour(112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0);
	colours["snow"] = Colour(255.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0);
	colours["springgreen"] = Colour(000.0 / 255.0, 255.0 / 255.0, 127.0 / 255.0);
	colours["steelblue"] = Colour(070.0 / 255.0, 130.0 / 255.0, 180.0 / 255.0);
	colours["tan"] = Colour(210.0 / 255.0, 180.0 / 255.0, 140.0 / 255.0);
	colours["teal"] = Colour(000.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	colours["thistle"] = Colour(216.0 / 255.0, 191.0 / 255.0, 216.0 / 255.0);
	colours["tomato"] = Colour(255.0 / 255.0, 099.0 / 255.0, 071.0 / 255.0);
	colours["turquoise"] = Colour(064.0 / 255.0, 224.0 / 255.0, 208.0 / 255.0);
	colours["violet"] = Colour(238.0 / 255.0, 130.0 / 255.0, 238.0 / 255.0);
	colours["wheat"] = Colour(245.0 / 255.0, 222.0 / 255.0, 179.0 / 255.0);
	colours["white"] = Colour(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
	colours["whitesmoke"] = Colour(245.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0);
	colours["yellow"] = Colour(255.0 / 255.0, 255.0 / 255.0, 000.0 / 255.0);
	colours["yellowgreen"] = Colour(154.0 / 255.0, 205.0 / 255.0, 050.0 / 255.0);

	return colours;
}

void SaveFile(TextEditor** editor)
{
	nfdchar_t* path;
	nfdresult_t result = NFD_SaveDialog(&path, NULL, 0, NULL, NULL);

	if (result != NFD_OKAY)
	{
		if (result != NFD_CANCEL)
			program.RenderStatus("An error occured opening the file dialog");

		return;
	}

	std::ofstream file(path);
	file << (*(*editor)).GetText();

	std::stringstream ss;
	ss << path;
	std::string fileName = std::filesystem::path(path).filename().string();

	if (!SavedSuccessfully(path))
	{
		program.RenderStatus("Error occured while saving " + fileName);
		NFD_FreePath(path);
		return;
	}

	program.RenderStatus(fileName + " saved successfully");

	(*(*editor)).fileName = fileName;
	(*(*editor)).fileDirectory = ss.str();

	NFD_FreePath(path);
}

bool SavedSuccessfully(const std::string path)
{
	std::ofstream outfile(path);

	if (outfile.is_open())
	{
		outfile.close();
		return true;
	}

	return false;
}

bool isFloat(const std::string number)
{
	try
	{
		float f = std::stof(number);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void PrintColour(const std::string commandName, const Colour colour)
{
	std::vector<std::string> nums = { std::to_string(colour.r), std::to_string(colour.g), std::to_string(colour.b), std::to_string(colour.a) };
	std::stringstream ss;
	ss << commandName << " has been changed to ";

	for (std::string s : nums)
	{
		int lastZeroIndex = s.find_last_not_of("0");
		if (lastZeroIndex != std::string::npos) {
			s.erase(lastZeroIndex + 1);
		}
		else {
			s.clear();
		}

		ss << (s.size() == 0 ? "0" : s) << " ";
	}

	program.RenderStatus(ss.str());
}

std::string Trim(const std::string& str, const std::string& whitespace)
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}