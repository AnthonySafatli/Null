#include "Headers/GLObejct.h"

GLObject::GLObject() 
	: vertexBuffer(), indexBuffer(), vertexArray(),
	  shaderProgram(), texture(), 
	  u_idealRatio(), u_leftMargin(), u_size(), u_sceneRowIndex(), u_sceneColumnIndex(), u_tex()
{
}

void GLObject::Init()
{
	vertexBuffer.Init();

	vertexArray.Init();
	VertexArray::EnableAttributes();

	indexBuffer.Init();

	shaderProgram.Init(
		"C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\vertex.glsl",
		"C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\fragment.glsl"
	);

	texture.Init("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Textures\\font texture.png");

	u_idealRatio.Init(shaderProgram.handle, "idealRatio");
	u_leftMargin.Init(shaderProgram.handle, "leftMargin");
	u_size.Init(shaderProgram.handle, "size");
	u_sceneRowIndex.Init(shaderProgram.handle, "sceneRowIndex");
	u_sceneColumnIndex.Init(shaderProgram.handle, "sceneColumnIndex");
	u_tex.Init(shaderProgram.handle, "tex");
}


