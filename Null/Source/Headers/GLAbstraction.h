#pragma once

#include <vector>

#define MAX_SQAURES_TO_RENDER 10000

#define INVISIBLE        0.0
#define NORMAL           1.0
#define CURSOR           2.0
#define CURSOR_COMMAND   3.0
#define CURSOR_INVISIBLE 4.0
#define STATUS           5.0
#define COMMAND          6.0
#define MARGIN           7.0
#define MARGIN_SELECTED  8.0

struct Vertex
{
	Vertex();
	Vertex(const float x, const float y, const float u, const float v, const float row, const float column, const float type);

	void Print();

	float reletivePosition[2];
	float texCoords[2];
	float row;
	float column;
	float type;
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

