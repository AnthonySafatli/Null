#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <STB/stb_image.h>
#include <GLEW/glew.h>

#include "Headers/GLAbstraction.h"

/* ====== Vertex Struct ====== */

Vertex::Vertex(const float x, const float y, const float u, const float v)
{
	position[0]  = x;
	position[1]  = y;
	texCoords[0] = u;
	texCoords[1] = v;
}

/* ====== Vertex Buffer ====== */

VertexBuffer::VertexBuffer()
{
	
	/*Vertex verticies[4] = { 
		Vertex(-0.5, -0.5, 0.0, 0.0),
		Vertex( 0.5,  0.5, 0.0, 0.0),
		Vertex( 0.5, -0.5, 0.0, 0.0),
		Vertex(-0.5,  0.5, 0.0, 0.0)
	};*/

	float positions[] = {
			-0.5, -0.5, 0.0, 0.0,
			 0.5,  0.5, 0.0, 1.0,
			 0.5, -0.5, 1.0, 0.0,
			-0.5,  0.5, 1.0, 1.0,
	};

	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
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
		1, 3, 0
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

VertexArray::VertexArray() 
{
	glGenVertexArrays(1, &handle);
	glBindVertexArray(handle);
}

void VertexArray::EnableAttributes()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
}

VertexArray::~VertexArray()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &handle);
}

/* ====== Shader Program ====== */

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) 
{
	handle = glCreateProgram();

	Shaders shaders(vertexPath, fragmentPath);

	glAttachShader(handle, shaders.vertexHandle);
	glAttachShader(handle, shaders.fragmentHandle);

	glLinkProgram(handle);

	glDetachShader(handle, shaders.vertexHandle);
	glDetachShader(handle, shaders.fragmentHandle);

	glUseProgram(handle);
}

ShaderProgram::~ShaderProgram() 
{
	glUseProgram(0);
	glDeleteProgram(handle);
}

/* ====== Texture ====== */

Texture::Texture(const char* filename)
{
	stbi_set_flip_vertically_on_load(1);

	int texWidth, texHeight, numColourChannels;
	unsigned char* bytes = stbi_load(filename, &texWidth, &texHeight, &numColourChannels, 4);

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);

	if (bytes)
		stbi_image_free(bytes);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* ====== Shaders ====== */

Shaders::Shaders(const char* vertexPath, const char* fragmentPath)
{
	CompileVertex(vertexPath);
	CompileFragment(fragmentPath);
}

void Shaders::CompileVertex(const char* vertexPath)
{
	std::string line;

	std::ifstream vertexStream(vertexPath);
	std::stringstream vertexStringStream;
	while (getline(vertexStream, line))
		vertexStringStream << line << '\n';

	std::string sourceString = vertexStringStream.str();

	vertexHandle = glCreateShader(GL_VERTEX_SHADER);
	const char* source = sourceString.c_str();

	glShaderSource(vertexHandle, 1, &source, nullptr);
	glCompileShader(vertexHandle);

	int result;
	glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(vertexHandle, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(vertexHandle, length, &length, message);

		std::cout << "Failed to compile vertex shader:" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(vertexHandle);
	}
}

void Shaders::CompileFragment(const char* fragmentPath)
{
	std::string line;

	std::ifstream fragmentStream(fragmentPath);
	std::stringstream fragmentStringStream;
	while (getline(fragmentStream, line))
		fragmentStringStream << line << '\n';

	std::string sourceString = fragmentStringStream.str();

	fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = sourceString.c_str();

	glShaderSource(fragmentHandle, 1, &source, nullptr);
	glCompileShader(fragmentHandle);

	int result;
	glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(fragmentHandle, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(fragmentHandle, length, &length, message);

		std::cout << "Failed to compile fragment shader:" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(fragmentHandle);
	}
}

Shaders::~Shaders()
{
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
}

/* ====== Uniform ====== */

Uniform::Uniform(const unsigned int shaderProgramHandle, const char* name)
{
	location = glGetUniformLocation(shaderProgramHandle, name);
}

/* ====== Misc ====== */

void GetErrors()
{
	GLenum error = glGetError();
	int errorNum = 0;

	while (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
		errorNum++;

		error = glGetError();
	}
}