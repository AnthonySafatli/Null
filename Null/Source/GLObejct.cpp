#include "Headers/GLObejct.h"

GLObject::GLObject() : init(false)
{
}

void GLObject::Init()
{
	vertexBuffer.Init();

	vertexArray.Init();
	VertexArray::EnableAttributes();

	indexBuffer.Init();

	// END: Fix paths / resource management before deployment
	shaderProgram.Init(
		"res\\vertex.glsl",
		"res\\fragment.glsl"
	);

	texture.Init("res\\font texture.png");

	u_idealRatio.Init(shaderProgram.handle, "idealRatio");
	u_size.Init(shaderProgram.handle, "size");
	u_leftMargin.Init(shaderProgram.handle, "leftMargin");
	u_rowIndex.Init(shaderProgram.handle, "rowIndex");
	u_columnIndex.Init(shaderProgram.handle, "columnIndex");
	u_maxHeight.Init(shaderProgram.handle, "maxHeight");
	u_maxWidth.Init(shaderProgram.handle, "maxWidth");
	
	u_tex.Init(shaderProgram.handle, "tex");
	u_foreground.Init(shaderProgram.handle, "foreground");
	u_cursorRow.Init(shaderProgram.handle, "cursorRow");

	init = true;
}


