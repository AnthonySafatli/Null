#pragma once

#include <vector>

struct Vertex
{
	Vertex(const float x, const float y, const float ax, const float ay, const float u, const float v);

	float position[2];
	float anchor[2];
	float texCoords[2];
};

class VertexBuffer
{
public:
	VertexBuffer(const int maxSquares);
	~VertexBuffer();

	void SetData(std::vector<Vertex> vertices);

	unsigned int handle; 
};

class IndexBuffer
{
public:
	IndexBuffer(const int maxSquares);
	~IndexBuffer();

	void SetData(std::vector<unsigned int> indices);

	unsigned int handle;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	static void EnableAttributes();

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
private:
	void SetTextureSettings();

public:
	Texture(const char* filename);
	~Texture();

	unsigned int handle;
};

class Shaders
{
private:
	void CompileVertex(const char* vertexPath);
	void CompileFragment(const char* fragmentPath);
public: 
	Shaders(const char* vertexPath, const char* fragmentPath);
	~Shaders();

	unsigned int vertexHandle;
	unsigned int fragmentHandle;
};

class Uniform
{
public: 
	Uniform(const unsigned int shaderProgramHandle, const char* name);

	int location;
};

void GetErrors();

