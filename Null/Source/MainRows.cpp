#include "Headers/MainRows.h"

/* ====== CommandRow ====== */

CommandRow::CommandRow()
{

}

std::vector<Vertex> CommandRow::GetVertices()
{
    std::vector<Vertex> commandContents;
    
    // add '>' to commandContents

    std::vector<Vertex> textContents = text.GetVertices();
    commandContents.insert(commandContents.end(), textContents.begin(), textContents.end());

    return commandContents;
}

/* ====== StatusBar ====== */

StatusBar::StatusBar()
{

}

std::vector<Vertex> StatusBar::GetVertices()
{
    return text.GetVertices(); // some sort of flag to tell shader to colour different
}
