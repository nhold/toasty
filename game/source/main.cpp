#include <Window.hpp>
#include <glad/glad_wgl.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Program.hpp>
#include <Matrix4.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG // Custom usage. Only png support.
#include <stb_image.h>

void CheckGlError()
{
	int i = glGetError();
	if(i != GL_NO_ERROR)
		printf("Error: %d\n", i);
}

class Image
{
public:
	Image(const std::string& filename)
	{
		Load(filename);
	}

	Image()
	{
	}

	bool Load(const std::string& filename)
	{
		int width = 0;
		int height = 0;

		unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (ptr)
		{
			size.x = width;
			size.y = height;
			if (width && height)
			{
				pixels = new unsigned char[width * height * 4];
				memcpy(&pixels[0], ptr, width * height * 4);
			}

			stbi_image_free(ptr);

			return true;
		}
		else
		{
			printf("Failed to load image %s. Reason: %s", filename.c_str(), stbi_failure_reason());
			return false;
		}
	}

	int GetWidth()
	{
		return size.x;
	}

	int GetHeight()
	{
		return size.y;
	}

	unsigned char* GetData()
	{
		return pixels;
	}

private:
	unsigned char* pixels;
	Vector2ui size;
	int channels;
};


class Texture2D
{
public:
	Texture2D(Image& const aImage) : image(aImage)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	unsigned int texture;
	Image& const image;

};
class SpriteRenderer
{
public:

	GLuint quadVAO = 0;
	Program program;
	Matrix4 orthoView;

	SpriteRenderer()
	{
		GLuint VBO;
		
		GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.0f, 1.0f, 0.0f
		};

		/*GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 16.0f, 0.0f, 1.0f,
			16.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 16.0f, 0.0f, 1.0f,
			16.0f, 16.0f, 1.0f, 1.0f,
			16.0f, 0.0f, 1.0f, 0.0f
		};*/

		/*GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};*/

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(quadVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		program.Load("assets/SimpleVertexShader.vertexshader", "assets/SimpleFragmentShader.fragmentshader");
		orthoView = orthoView.CreateOrthographic(800, 600);
		program.SetMatrix4Uniform("projection", orthoView);
	}

	void DrawSprite(Texture2D& texture, Vector2f position, Vector2f size, GLfloat rotation)
	{
		program.Bind();

		Matrix4 model;
		//model.matrixData[12] = 0.5f;
		//model.matrixData[13] = 0.5f;
		//model = model.Translate(position);
		//model = model.Scale(size.x, size.y);

		program.SetMatrix4Uniform("model", model);

		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
};

int main()
{
	Window window;
	window.Create(800, 600, "Toasty :)");
	
	glViewport(0, 0, 800, 800);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	SpriteRenderer renderer;
	Image image("assets/enemy1.png");
	Texture2D tex(image);
	
	while (window.IsOpen())
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.DrawSprite(tex, Vector2f(0, 0), Vector2f(16, 16), 0);

		window.Display();
	}

	window.Destroy();

	return 0;
}
