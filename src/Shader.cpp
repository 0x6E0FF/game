#include "glad/glad.h"

#include "Shader.h"

#include <fstream>
#include <iostream>
using namespace std;

static string read_file(const char *file)
{
	ifstream ifs(file);
	if (ifs)
	{
		string contents;
		ifs.seekg(0, ios::end);
		contents.resize(ifs.tellg());
		ifs.seekg(0, ios::beg);
		ifs.read(&contents[0], contents.size());
		ifs.close();
		return(contents);
	}
	else
	{
		cout << "cannot open file " << file << endl;
	}
	return string("");	
}

Shader::Shader(const char *vertPath, const char *fragPath)
{
	const char *c_str;
	int success;
	char infoLog[512];
	
	string vert = read_file(vertPath);
	string frag = read_file(fragPath);

	unsigned int vert_id = glCreateShader(GL_VERTEX_SHADER);
	c_str = vert.c_str();
	glShaderSource(vert_id, 1, &c_str, NULL);
	glCompileShader(vert_id);
	glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vert_id, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};
	
	unsigned int frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	c_str = frag.c_str();
	glShaderSource(frag_id, 1, &c_str, NULL);
	glCompileShader(frag_id);
	glGetProgramiv(frag_id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(frag_id, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	
	mId = glCreateProgram();
	glAttachShader(mId, vert_id);
	glAttachShader(mId, frag_id);
	glLinkProgram(mId);
	glGetProgramiv(mId, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(mId, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);
}

void Shader::use()
{
	glUseProgram(mId);
}
