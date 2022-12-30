#pragma once

struct Vertex
{
	Vertex(const float x, const float y, const float u, const float v);

	float position[2];
	float texCoords[2];
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	unsigned int handle; 
};

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	unsigned int handle;
};

class VertexArray
{
public:
	VertexArray(unsigned int vertexBufferHandle);
	~VertexArray();

	unsigned int handle;
};

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();

	unsigned int handle;
};

class Texture
{
public:
	Texture(const char* filename);
	~Texture();

	unsigned int handle;
};

class Shaders
{
public: 
	Shaders(const char* vertexPath, const char* fragmentPath);
	~Shaders();

	unsigned int vertexHandle;
	unsigned int fragmentHandle;
};

void SetUniforms();