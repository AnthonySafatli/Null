#include <GLEW/glew.h>

#include "Headers/GLAbstraction.h"

/* ====== Vertex Buffer ====== */

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &handle);
	
	// TODO: Implement setup for buffer
}

VertexBuffer::~VertexBuffer()
{
	// TODO: Implement Destructor
}

/* ====== Index Buffer ====== */

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &handle);

	// TODO: Implement setup for buffer
}

IndexBuffer::~IndexBuffer()
{
	// TODO: Implement Destructor
}

/* ====== Vertex Array ====== */

VertexArray::VertexArray(unsigned int vertexBufferHandle)
{
	// TODO: Implement Constructor
}

VertexArray::~VertexArray()
{
	// TODO: Implement Destructor
}

/* ====== Shader Program ====== */

ShaderProgram::ShaderProgram()
{
	// TODO: Implement Constructor
}

ShaderProgram::~ShaderProgram() 
{
	// TODO: Implement Destructor
}