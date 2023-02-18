#pragma once

class TextArea
{
public:
    /* Callback Methods */
    virtual void ProcessKey(int key, int action, int mods) = 0;
    virtual void ProcessChar(unsigned int codepoint) = 0;
    virtual void OnResize(int width, int height) = 0;

    /* Text Methods */
    void IncrementBarrier();
    void AddCharacter(const char ch);
    void RemoveCharacterFromLeft();
    void RemoveCharacterFromRight();
    void AddTab();
    void Return();
};