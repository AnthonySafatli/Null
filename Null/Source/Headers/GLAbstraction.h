#pragma once

#include <vector>

#define MAX_SQAURES_TO_RENDER 10000

struct Vertex
{
	Vertex(const float x, const float y, const float u, const float v, const float row, const float column, const unsigned int highlight);

	float reletivePosition[2];
	float texCoords[2];
	float row;
	float column;
	unsigned int highlight;
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Init();
	void SetData(std::vector<Vertex> vertices);

	unsigned int handle; 
};

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Init();
	void SetData(std::vector<unsigned int> indices);

	unsigned int handle;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Init();

	static void EnableAttributes();

	unsigned int handle;
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void Init(const char* vertexPath, const char* fragmentPath);

	unsigned int handle;
};

class Texture
{
private:
	void SetTextureSettings();

public:
	Texture();
	~Texture();

	void Init(const char* filename);

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
	Uniform();

	void Init(const unsigned int shaderProgramHandle, const char* name);

	int location;
};

void GetErrors();

