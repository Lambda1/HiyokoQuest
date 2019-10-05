#ifndef __MY_SHADER_HPP__
#define __MY_SHADER_HPP__

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

class MyShader{
	GLint compiled, linked;
	/* Vertex-shader and Fragment-shader: variable */
	GLuint vertShader, fragShader;
	GLuint gl2Program;

	bool VCompile(std::string&);
	bool FCompile(std::string&);
	bool Link();
	
	void PrintCompileError(const GLuint);
	
	public:
	MyShader(){}
	MyShader(const std::string, const std::string);
	~MyShader();
	bool Set(const std::string, const std::string);
	void UseProgram();
	void UnUseProgram();
	
	void EnableVAArray(const int value){ glEnableVertexAttribArray(value); }
	void Uniform1i(const int tex_value,const int unit_number){ glUniform1i(tex_value, unit_number); }

	/* Getter */
	inline GLuint GetProgramID(){ return gl2Program; }
	inline int GetAttLocationValue(const std::string var){ return glGetAttribLocation(gl2Program, var.c_str()); }
	inline int GetUniLocationValue(const std::string var){ return glGetUniformLocation(gl2Program, var.c_str()); }
};

#endif
