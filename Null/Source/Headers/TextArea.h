#pragma once

#include <string>
#include <vector>

class TextArea
{
public:
    /* Callback Methods */
    virtual void ProcessKey(int key, int action, int mods);
    virtual void ProcessChar(unsigned int codepoint);
    virtual void OnResize(int width, int height);

    /* Virtual Methods */
    virtual void AddLeftMargin();
    virtual void RemoveLeftMargin();

    /* Constructor Helper Methods */
    void ConstructorStart(int leftMargin, bool showCursor);
    void ConstructorEnd();

    /* Text Data */
    std::vector<std::string> rows;

    /* Text Methods */
    void SetLeftMargin(const int margin);
    int GetCharIndex();
    int GetLastIndexInRow();
    void AddCharacter(const char ch);
    void RemoveCharacterFromLeft();
    void RemoveCharacterFromRight();
    void AddTab();
    void Return();

    /* Char Methods */
    int LongestRowSize();

    /* Margin Methods */
    void AddCharacterToMargin(const char ch, const int index, const float type);

    /* Cursor Methods */
    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();
    void MoveHome();
    void MoveEnd();

    /* Read-only */
    int leftMargin;
};