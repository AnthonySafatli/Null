#pragma once

class TextArea
{
public:
	virtual void OnKey() = 0;
	virtual void OnChar() = 0;
	virtual void OnResize() = 0;
};