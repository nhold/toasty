#include <Program.hpp>
#include <fstream>
#include <vector>

Program::Program()
{
}

Program::~Program()
{
}


GLuint Program::Compile(String filePath)
{
	// Get code from file.
	String code = GetFileAsText(filePath.c_str());
	char const* codePointer = code.c_str();

	GLenum shaderType = -1;

	// Determine shader type
	if (filePath.find("vertex") != std::string::npos)
	{
		shaderType = GL_VERTEX_SHADER;
	}

	if (filePath.find("fragment") != std::string::npos)
	{
		shaderType = GL_FRAGMENT_SHADER;
	}

	GLuint shaderId = glCreateShader(shaderType);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Shader
	glShaderSource(shaderId, 1, &codePointer, NULL);
	glCompileShader(shaderId);

	// Check Vertex Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	return shaderId;
}

bool Program::Load(String vertexFile, String fragmentFile)
{
	isValid = true;

	GLint Result = GL_FALSE;
	int InfoLogLength;

	GLuint compiledVertexShaderId = Compile(vertexFile);
	GLuint compiledFragmentShaderId = Compile(fragmentFile);

	// Link the program
	printf("Linking program\n");
	programId = glCreateProgram();
	glAttachShader(programId, compiledVertexShaderId);
	glAttachShader(programId, compiledFragmentShaderId);
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programId, compiledVertexShaderId);
	glDetachShader(programId, compiledFragmentShaderId);

	glDeleteShader(compiledVertexShaderId);
	glDeleteShader(compiledFragmentShaderId);

	return isValid;
}

void Program::Bind()
{
	glUseProgram(programId);
}

String Program::GetFileAsText(const char* filename)
{
	std::ifstream in(filename, std::ios::in);

	if (in)
	{
		String contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		return contents;
	}

	return String();
}