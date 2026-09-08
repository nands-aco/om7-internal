#include "om7-hgl.hpp"
#include <cstdint>
#include <glad/glad.h>

namespace om7
{
	GLuint Texture;
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint Program;
	GLuint VertexArray;
	GLuint VerticesBuffer;
	GLuint IndicesBuffer;

	const GLchar *VertexShaderSource =
		"#version 300 es\n"
		// "in highp vec2 position;\n"
		// "in mediump vec2 texCoords;\n"
		"layout(location = 0) in highp vec2 position;\n"
		"layout(location = 1) in mediump vec2 texCoords;\n"
		"out mediump vec2 TexCoords;\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = vec4(position, 0.0, 1.0);\n"
		"\tTexCoords = texCoords;\n"
		"}\n";
	//
	//
	//
	const GLchar *FragmentShaderSource = 
		"#version 300 es\n"
		"in mediump vec2 TexCoords;\n"
		"out mediump vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"\tcolor = texture(ourTexture, TexCoords);\n"
		"}\n";
	//
	//
	//
	const float Vertices[] =
	{
		/* 左上 */ -1.0f, +1.0f, +0.0f, +0.0f,
		/* 左下 */ -1.0f, -1.0f, +0.0f, +1.0f,
		/* 右下 */ +1.0f, -1.0f, +1.0f, +1.0f,
		/* 右上 */ +1.0f, +1.0f, +1.0f, +0.0f,
	};
	//
	//
	//
	const GLuint Indices[] =
	{
		/* １つめ */ 0, 1, 2,
		/* ２つめ */ 0, 2, 3,
	};
	//
	//
	//
	void GlInit()
	{
		//
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glDisable(GL_DITHER);
		//
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, nullptr);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//
		VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader, 1, &VertexShaderSource, nullptr);
		glCompileShader(VertexShader);
		//
		FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
		glCompileShader(FragmentShader);
		//
		Program = glCreateProgram();
		glAttachShader(Program, VertexShader);
		glAttachShader(Program, FragmentShader);
		glLinkProgram(Program);
		//
		glUseProgram(Program);
		//
		glDetachShader(Program, VertexShader);
		glDetachShader(Program, FragmentShader);
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		//
		glGenVertexArrays(1, &VertexArray);
		glGenBuffers(1, &VerticesBuffer);
		glGenBuffers(1, &IndicesBuffer);
		//
		glBindVertexArray(VertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, VerticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		//
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	}
	//
	//
	//
	void GlTerm()
	{
		//
		glDeleteBuffers(1, &IndicesBuffer);
		glDeleteBuffers(1, &VerticesBuffer);
		glDeleteVertexArrays(1, &VertexArray);
		//
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &Texture);
		//
		glUseProgram(0);
		glDeleteProgram(Program);
	}
	//
	//
	//
	void GlRender()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, ScreenBuffers[ScreenBufferIndex]);
		// glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, ScreenBuffers[ScreenBufferIndex]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ScreenBufferIndex = (ScreenBufferIndex + 1) & 1;
	}
}
