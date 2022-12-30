#include <GLEW/glew.h>

#include "Headers/GLAbstraction.h"

/* ====== Vertex Struct ====== */

Vertex::Vertex(float x, float y, float u, float v)
{
	position[0]  = x;
	position[1]  = y;
	texCoords[0] = u;
	texCoords[1] = v;
}

/* ====== Vertex Buffer ====== */

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &handle);
	
	Vertex verticies[4] = { 
		Vertex(-0.5,  0.5, 0.0, 0.0),
		Vertex( 0.5,  0.5, 0.0, 0.0),
		Vertex(-0.5, -0.5, 0.0, 0.0),
		Vertex( 0.5, -0.5, 0.0, 0.0)
	};

	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), verticies, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &handle);
}

/* ====== Index Buffer ====== */

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &handle);

	unsigned int indices[] = {
		0, 1, 2,
		1, 3, 2
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &handle);
}

/* ====== Vertex Array ====== */

VertexArray::VertexArray(unsigned int vertexBufferHandle) // maybe pass in vertex attribute struct array?
{
	glGenVertexArrays(1, &handle);

	glBindVertexArray(handle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);

	// TODO: Implement vertex attributes
}

VertexArray::~VertexArray()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &handle);
}

/* ====== Shader Program ====== */

ShaderProgram::ShaderProgram() // compile shaders at start or before | maybe shader class or method?
{
	handle = glCreateProgram();

	// TODO: Implement shaders and shader handles

	// glAttachShader(handle, vertexShaderHandle);
	// glAttachShader(handle, fragmentShaderHandle);

	glLinkProgram(handle);

	// glDetachShader(handle, vertexShaderHandle);
	// glDetachShader(handle, fragmentShaderHandle);
}

ShaderProgram::~ShaderProgram() 
{
	// glDeleteShader(vertexShaderHandle);
	// glDeleteShader(fragmentShaderHandle);

	glUseProgram(0);
	glDeleteProgram(handle);
}

/* ====== Misc ====== */

void CompileShaders() 
{

}

void SetUniforms()
{

}