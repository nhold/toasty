#pragma once

#include <String.hpp>
#include <glad/glad_wgl.h>
#include <Matrix4.hpp>

class Program
{
public:
	Program();
	~Program();

	bool Load(String vertexFile, String fragmentFile);
	void SetMatrix4Uniform(String name, Matrix4 matrix);
	void Bind();
	GLuint programId;
private:
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	bool isValid;

	GLuint Compile(String code);
	String GetFileAsText(const char* filename);
	
};