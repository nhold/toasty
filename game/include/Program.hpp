#pragma once

#include <String.hpp>
#include <glad/glad_wgl.h>

class Program
{
public:
	Program();
	~Program();

	bool Load(String vertexFile, String fragmentFile);
	void Bind();
	GLuint programId;
private:
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	bool isValid;

	GLuint Compile(String code);
	String GetFileAsText(const char* filename);
	
};