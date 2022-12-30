#pragma once

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