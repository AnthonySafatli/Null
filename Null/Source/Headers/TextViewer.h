#pragma once

#include "TextArea.h"

class TextViewer : public TextArea
{
public:
	TextViewer(const std::string text, const std::string pageName);

	/* Viewer Methods */
	void UpdateText(const std::string text);
};