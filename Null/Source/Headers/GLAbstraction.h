#pragma once

struct Vertex
{
	Vertex(float x, float y, float u, float v);

	float position[2];
	float texCoords[2];
};

class VertexBuffer
{
private:

public:
	VertexBuffer();
	~VertexBuffer();

	unsigned int handle; // maybe unsigned int?
};

class IndexBuffer
{
private:

public:
	IndexBuffer();
	~IndexBuffer();

	unsigned int handle;
};

class VertexArray
{
private:

public:
	VertexArray(unsigned int vertexBufferHandle);
	~VertexArray();

	unsigned int handle;
};

class ShaderProgram
{
private:

public:
	ShaderProgram();
	~ShaderProgram();

	unsigned int handle;
};

void CompileShaders();

void SetUniforms();