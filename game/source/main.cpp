#include <Window.hpp>
#include <glad/glad_wgl.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Program.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG // Custom usage. Only png support.
#include <stb_image.h>

void CheckGlError()
{
	int i = glGetError();
	if(i != GL_NO_ERROR)
		printf("Error: %d\n", i);
}

/*static const GLfloat g_vertex_buffer_data[] = {
   -0.5f, 0.5f, 0.0f,
   0.0f, 0.5f, 0.0f,
   -0.5f,  0.0f, 0.0f,
   0.0f,  0.0f, 0.0f
};*/

static const GLfloat g_vertex_buffer_data[] = {
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

bool LoadPNGFromFile(const std::string& filename, std::vector<uint8_t>& pixels, Vector2ui& size)
{
	// Clear the array (just in case)
	pixels.clear();

	// Load the image and get a pointer to the pixels in memory
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (ptr)
	{
		// Assign the image properties
		size.x = width;
		size.y = height;

		if (width && height)
		{
			// Copy the loaded pixels to the pixel buffer
			pixels.resize(width * height * 4);
			memcpy(&pixels[0], ptr, pixels.size());
		}

		// Free the loaded pixels (they are now in our own pixel buffer)
		stbi_image_free(ptr);

		return true;
	}
	else
	{
		// Error, failed to load the image
		printf("Failed to load image %s. Reason: %s", filename.c_str(), stbi_failure_reason());

		return false;
	}
}
int main()
{
	Window window;
	window.Create(800, 600, "The ultimate test");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, g_vertex_buffer_data, GL_STATIC_DRAW);

	Program simpleProgram;
	simpleProgram.Load("assets/SimpleVertexShader.vertexshader", "assets/SimpleFragmentShader.fragmentshader");
	
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glViewport(0, 0, 800, 800);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	while (window.IsOpen())
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simpleProgram.Bind();
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);


		window.Display();
	}

	window.Destroy();

	return 0;
}
