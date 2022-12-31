#include "Headers/Contents.h"

/* ====== Contents ====== */

Contents::Contents() : textSize(10)
{
	// Only run once at beginning of program
}

void Contents::AddCharacter(char ch)
{

}

std::vector<Vertex> Contents::GetVertices() // make pointer or something so no copy data
{
	// optimize vectors

	std::vector<Vertex> contents;

	std::vector<Vertex> commandContents = command.GetVertices();
	contents.insert(contents.end(), commandContents.begin(), commandContents.end());

	std::vector<Vertex> statusContents = status.GetVertices();
	contents.insert(contents.end(), statusContents.begin(), statusContents.end());

	std::vector<Vertex> sceneContents = status.GetVertices();
	contents.insert(contents.end(), sceneContents.begin(), sceneContents.end());

	return contents;
}

std::vector<unsigned int> Contents::GetIndices()
{
	return std::vector<unsigned int>();
}
