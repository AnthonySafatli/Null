// #include "Headers/MainRows.h"
// #include "Headers/Character.h"
// #include "Headers/NullEditor.h"
// 
// #include <string>
// 
// /* ====== CommandRow ====== */
// 
// CommandRow::CommandRow() : row()
// {
// 	TexCoords texCoords = GetCoords('>');
// 
// 	row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, 1, 0));
// 	row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, 1, 0));
// 	row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, 1, 0));
// 	row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, 1, 0));
// }
// 
// /* ====== StatusBar ====== */
// 
// StatusBar::StatusBar() : row(), message()
// {
// }
// 
// void StatusBar::Init(const std::string initialMessage)
// {
// 	message = initialMessage;
// 	Render();
// }
// 
// void StatusBar::UpdateLength()
// {
// 	row.vertices.clear();
// 	Render();
// }
// 
// void StatusBar::UpdateMessage(const std::string message)
// {
// 	row.vertices.clear();
// 	this->message = message;
// 	Render();
// }
// 
// 
// void StatusBar::Render()
// {
// 	//float length = ((1.0 / (0.1 * appContents.textSize)) * ((float)appContents.width / (float)appContents.idealWidth)) + 1;
// 
// 	TexCoords texCoords = GetCoords(' ');
// 
// 	row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u, texCoords.v, -2, 0, 1));
// 	row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, -2, 0, 1));
// 	row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -2, 0, 1));
// 	row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), -2, 0, 1));
// 
// 	//for (int i = 1; i < length; i++)
// 	for (int i = 1; i < 0; i++)
// 	{
// 		if (i - 1 < message.size())
// 		{
// 			TexCoords texCoords = GetCoords(message.at(i - 1));
// 
// 			row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u, texCoords.v, -2, i, 1));
// 			row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, -2, i, 1));
// 			row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -2, i, 1));
// 			row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), -2, i, 1));
// 		}
// 		else
// 		{
// 			TexCoords texCoords = GetCoords(' ');
// 
// 			row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u, texCoords.v, -2, i, 1));
// 			row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, -2, i, 1));
// 			row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -2, i, 1));
// 			row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), -2, i, 1));
// 		}
// 	}
// 
// 	//appContents.AddIndices();
// }