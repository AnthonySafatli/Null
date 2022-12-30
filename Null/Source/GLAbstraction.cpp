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

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) // compile shaders at start or before | maybe shader class or method?
{
	handle = glCreateProgram();

	Shaders shaders(vertexPath, fragmentPath);

	glAttachShader(handle, shaders.vertexHandle);
	glAttachShader(handle, shaders.fragmentHandle);

	glLinkProgram(handle);

	glDetachShader(handle, shaders.vertexHandle);
	glDetachShader(handle, shaders.fragmentHandle);

	SetUniforms();
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

/* ====== Shaders ====== */

Shaders::Shaders(const char* vertexPath, const char* fragmentPath)
{
	// TODO: Implement Constructor

	// load into string

	// compile

	// error checking

	// save handles
}

Shaders::~Shaders()
{
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
}

/* ====== Misc ====== */

void SetUniforms()
{
	// TODO: Implement Method

	// TODO: Also find uniform locations for caching
}