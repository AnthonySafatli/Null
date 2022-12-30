#pragma once

struct Vertex
{
	Vertex(const float x, const float y, const float u, const float v);

	float position[2];
	float texCoords[2];
};

class VertexBuffer
{
private:

public:
	VertexBuffer();
	~VertexBuffer();

	unsigned int handle; 
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

class Texture
{
private:

public:
	Texture(const char* filename);
	~Texture();

	unsigned int handle;
};

void CompileShaders();

void SetUniforms();