#pragma once

#include <string>
#include <vector>

class TextArea
{
public:
    /* Callback Methods */
    virtual void ProcessKey(int key, int action, int mods) = 0;
    virtual void ProcessChar(unsigned int codepoint) = 0;
    virtual void OnResize(int width, int height) = 0;

    /* Virtual Methods */
    virtual void AddLeftMargin() = 0;

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

    /* Margin Methods */
    void AddCharacterToMargin(const char ch, const int index);

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