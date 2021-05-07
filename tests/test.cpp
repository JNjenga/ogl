#pragma comment(lib, "User32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "msvcrt")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "lib/glfw3")

#define GLFW_STATIC
#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

#define OGL_FRAMEWORK_IMPLEMENTATION
#include "ogl_framework.h"

int main()
{
	GLFWwindow * window;

	if(!glfwInit())
		return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(640, 480,"Simple",NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);

	of::init();
	
	glfwSwapInterval(1);

	const char * vsource = 
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;\n"
		"void main()\n"
		"{ gl_Position = vec4(vPosition, 0.0f); }";
	const char * fsource = 
		"#version 330 core\n"
		"out vec4 fColor; \n"
		"void main() { fColor = vec4(0.0, 0.0, 1.0, 1.0);}\n";

	
    float vertices[] = {
        -0.5f, 0.5f, 0.0f, // Top left
        0.5f, 0.5f, 0.0f, // Top right
        0.5f, -0.5f, 0.0f, // Bottom right
        -0.5f, -0.5f, 0.0f, // Bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

	auto vao = of::create_vao(vertices, 12, indices, 6, nullptr, 0);

	auto shader = of::create_shader(vsource, fsource);

	glUseProgram(shader.id);
	printf("a %d a", shader.id); 
	CHECK_ERROR();

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		of::draw_vao(vao, 6, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
