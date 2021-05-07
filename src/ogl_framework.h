#ifndef _OGL_FRAMEWORK_H
#define _OGL_FRAMEWORK_H

#define CHECK_ERROR() of::_check_error(__FILE__, __LINE__)
#define GLEW_STATIC

#include "GL/glew.h"
#include <unordered_map>
#include <cstdlib>
#include <iostream>
#include <string>

typedef unsigned int uint_t;
typedef unsigned char uchar_t;

namespace of
{
	struct shader_t
	{
		uint_t id;
		std::unordered_map<const char *,int> uniforms;
	};

	int init();
	shader_t create_shader(const char * vsource, const char * fsource);
    shader_t reload_shader(uint_t id, const char * vsource, const char * fsource);

	int get_uniform_loc(of::shader_t & shader, const char * name);
	int setu_float(of::shader_t & shader, const char * name, float value);
	int setu_int(of::shader_t & shader, const char * name, int value);
	int setu_mat4(of::shader_t & shader, const char * name, void * value);
	int setu_vec3(of::shader_t & shader, const char * name, void * value);
	
	uint_t create_vao(float * v, int vsize, uint_t * indices, int isize, float * t, int tsize);
	uint_t create_texture2d(uchar_t * data,int depth, int width, int height);

	void draw_vao(int id, int vcount, int tex_id);

	void _check_error(const char * file, int line);
}

#ifdef OGL_FRAMEWORK_IMPLEMENTATION

int of::init()
{
	// Setup opengl state
	int is_ok = 0;

	if(glewInit() != GLEW_OK) is_ok = 0;;

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	return is_ok; 
}

of::shader_t of::create_shader(const char * vsource, const char * fsource)
{
	of::shader_t shader;

	uint_t vid, fid, sid;

	vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vsource, NULL);
	glCompileShader(vid);

	fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &fsource, NULL);
	glCompileShader(fid);

	sid = glCreateProgram();
	glAttachShader(sid, vid);
	glAttachShader(sid, fid);

	glLinkProgram(sid);
	CHECK_ERROR();

	shader.id = sid;
	
	int success;
	char infoLog[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	
	glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	glDeleteShader(vid);
	glDeleteShader(fid);

	return shader;
}

of::shader_t of::reload_shader(uint_t id, const char * vsource, const char * fsource)
{
    glDeleteProgram(id);
    of::shader_t shader = of::create_shader(vsource, fsource);
    return shader;
}

int of::get_uniform_loc(of::shader_t & shader, const char * name)
{
	auto it = shader.uniforms.find(name);
	if(it != shader.uniforms.end())
		return it->second;

	int loc = glGetUniformLocation(shader.id, name);

	if(loc != -1)
		shader.uniforms.insert({name, loc});

	return loc;
}

int of::setu_float(of::shader_t & shader, const char * name, float value)
{
	int loc = of::get_uniform_loc(shader, name);
	glUniform1f(loc, value);
	return 1;
}

int of::setu_int(of::shader_t & shader, const char * name, int value)
{
	int loc = of::get_uniform_loc(shader, name);
	glUniform1i(loc, value);
	return 1;
}

int of::setu_mat4(of::shader_t & shader, const char * name, void * value)
{
	int loc = of::get_uniform_loc(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<GLfloat *>(value));
	return 1;
}

int of::setu_vec3(of::shader_t & shader, const char * name, void * value)
{
	int loc = of::get_uniform_loc(shader, name);
	glUniform3fv(loc, 1, static_cast<GLfloat *>(value));

	return 1;
}

uint_t of::create_vao(float * v, int vsize, uint_t * i, int isize, float * t, int tsize)
{
	uint_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_ERROR();

	uint_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vsize, v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	CHECK_ERROR();
    
    if(tsize != 0 && t != NULL)
    {
        uint_t tbo;
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tsize, t, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        CHECK_ERROR();
    }

	uint_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint_t) * isize, i, GL_STATIC_DRAW);
	CHECK_ERROR();

	return vao;
}

uint_t of::create_texture2d(uchar_t * data, int depth, int width, int height)
{
    uint_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

void of::draw_vao(int vid, int vcount, int tex_id)
{
	glBindVertexArray(vid);
	glDrawElements(GL_TRIANGLES, vcount, GL_UNSIGNED_INT, 0);
}

void of::_check_error(const char * file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:
				error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:
				error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:
									   error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:
									   error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:
									   error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
									   error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		printf("%s | %s (%d) \n", error.c_str(), file, line);

	}
}

#endif
#endif

