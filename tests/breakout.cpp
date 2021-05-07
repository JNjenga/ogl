#ifdef _WIN32

#pragma comment(lib, "User32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "msvcrt")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "shell32")

#endif

#define GLFW_STATIC
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define OGL_FRAMEWORK_IMPLEMENTATION
#include "ogl_framework.h"

// Variables
const int width = 800;
const int height = 600;

const float brickw = 40.0f;
const float brickh = 20.0f;

float paddlew = 90.0f;
const float paddleh = 20.0f;

float ballw = 20.0f;
float ballh = 20.0f;

int vid;
int tid;
of::shader_t shader;

// Functions 
GLFWwindow * create_window();
void draw_tiles();
void on_resize(GLFWwindow* window, int w, int h);

int main()
{
	GLFWwindow * window = create_window();

	of::init();

	if(window == NULL)
		return -1;


	const char * vsource = 
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;\n"
		"layout(location = 1) in vec2 tPosition;\n"
		"uniform mat4 m_mat = mat4(1.0f);;\n"
		"uniform mat4 p_mat = mat4(1.0f);\n"
		"out vec3 col;\n"
		"out vec2 texCoords;\n"
		"void main()\n"
		"{\n"
		"gl_Position = p_mat * m_mat * vec4(vPosition, 1.0f);\n  "
		"col = vPosition;\n  "
		"texCoords = tPosition;\n  "
		"}";
	const char * fsource = 
		"#version 330 core\n"
		"out vec4 fColor; \n"
		"uniform vec3 scolor;"
		"in vec3 col;"
		"in vec2 texCoords;"
		"uniform sampler2D tex;"
		"void main() { fColor = texture(tex, texCoords);}\n";
	
    float vertices[] = {
        .0f, .0f, 0.0f, // Top left
        1.0f, .0f, 0.0f, // Top right
        1.0f, 1.0f, 0.0f, // Bottom right
        .0f, 1.0f, 0.0f, // Bottom left
    };

    float texture[] = {
        0.0f, 1.0f,  
        1.0f, 1.0f, 
        1.0f, 0.0f,
        0.0f, 0.0f, 
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    int width, height, depth;
    uchar_t * data = stbi_load("texture.png", &width, &height, &depth, 0);

    if(data)
    {
        tid = of::create_texture2d(data, depth, width, height);
        std::cout << tid;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture\n";
        return 0;
    }


	vid = of::create_vao(vertices, 12, indices, 6, texture, 8);

	shader = of::create_shader(vsource, fsource);
	glUseProgram(shader.id);
	
	glm::mat4 p_mat = glm::ortho(.0f, 800.0f, 600.0f, .0f, -100.0f, 100.0f);
	glm::mat4 m_mat = glm::mat4(1.0f);
	m_mat = glm::scale(m_mat, glm::vec3(40.0f,10.0f, 0.0f));

	of::setu_mat4(shader, "p_mat", glm::value_ptr(p_mat));
	of::setu_mat4(shader, "m_mat", glm::value_ptr(m_mat));

	auto color = glm::vec3(1.0f, 0.0f, 0.0f);

	of::setu_vec3(shader, "scolor", &color);
    CHECK_ERROR();
	while(!glfwWindowShouldClose(window))
	{
		CHECK_ERROR();
		glClear(GL_COLOR_BUFFER_BIT);
		
		// of::draw_vao(vid, 6, 0);
		draw_tiles();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

GLFWwindow * create_window()
{
	GLFWwindow * window;

	if(!glfwInit())
		return nullptr;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(800, 600, "Simple",NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_resize);

	glfwSwapInterval(10);
	return window;
}

void draw_tiles()
{
	glm::mat4 m_mat = glm::mat4(1.0f);
	
	int tileno = width / brickw;

	for(int i = 0; i < 10; i ++)
	{
		for(int j = 0; j < tileno; j ++)
		{
			glm::mat4 m_mat = glm::mat4(1.0f);
			m_mat = glm::translate(m_mat, glm::vec3(brickw * j, brickh * i, 0.0f));
			m_mat = glm::scale(m_mat, glm::vec3(brickw,brickh, 0.0f));
			of::setu_mat4(shader, "m_mat", glm::value_ptr(m_mat));
            glBindTexture(GL_TEXTURE_2D, tid);
			of::draw_vao(vid, 6, 0);
		}

	}
}
void on_resize(GLFWwindow* window, int w, int h)
{
    glViewport(0,0, w, h);
}
