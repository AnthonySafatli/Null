#pragma once

#include "TextArea.h"

class TextViewer : public TextArea
{
public:
	TextViewer(const std::string text, const std::string pageName, AreaType type);

	/* Viewer Methods */
	void UpdateText(const std::string text);
};