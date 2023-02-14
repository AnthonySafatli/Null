#pragma once

class TextArea
{
public:
    virtual void ProcessKey(int key, int action, int mods) = 0;
    virtual void ProcessChar(unsigned int codepoint) = 0;
    virtual void OnResize(int width, int height) = 0;
};