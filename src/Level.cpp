#include "Level.h"

#include "Shader.h"
#include "glad/glad.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Level::Level(const char *pPBMFile)
{
	vector<Vertex> walls_vertices;
	
	string line;
	ifstream myfile(pPBMFile);
	if (myfile.is_open())
	{
		getline(myfile,line);
		getline(myfile,line);
		getline(myfile,line);
		
		istringstream iss(line);
		iss >> mW >> mH;
		
		while (getline(myfile, line))
		{
			mMap += line;
		}
		myfile.close();
	}
	
	for(int y = 0; y < mH; y++)
	{
		for(int x = 0; x < mW; x++)
		{
			char item = mMap[y*(int)mW + x];
			int prev_x = x - 1;
			int prev_y = y - 1;
			
			if (item == '0')
			{
				if (prev_x < 0 || mMap[y*(int)mW + prev_x] == '1')
				{	
					/* add a wall at the left of the current cell */
					walls_vertices.push_back({ (float)x,   0.0f,  (float)y,  1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,   0.0f,    y+1.0f,  1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,  10.0f,    y+1.0f,  1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,  10.0f,  (float)y,  1.0f, 0.0f,  0.0f });
				}
				
				if (prev_y < 0 || mMap[prev_y*(int)mW + x] == '1')
				{
					/* add a wall at the top of the current cell */
					walls_vertices.push_back({ (float)x,   0.0f, (float)y,  0.0f, 0.0f, 1.0f });
					walls_vertices.push_back({   x+1.0f,   0.0f, (float)y,  0.0f, 0.0f, 1.0f });
					walls_vertices.push_back({   x+1.0f,  10.0f, (float)y,  0.0f, 0.0f, 1.0f });
					walls_vertices.push_back({ (float)x,  10.0f, (float)y,  0.0f, 0.0f, 1.0f });
				}
				
				/* add right and bottom walls if need be */
				if (x == mW - 1)
				{
					/* right wall */
					walls_vertices.push_back({ x+1.0f,   0.0f, (float)y,  -1.0f, 0.0f, 0.0f });
					walls_vertices.push_back({ x+1.0f,   0.0f,   y+1.0f,  -1.0f, 0.0f, 0.0f });
					walls_vertices.push_back({ x+1.0f,  10.0f,   y+1.0f,  -1.0f, 0.0f, 0.0f });
					walls_vertices.push_back({ x+1.0f,  10.0f, (float)y,  -1.0f, 0.0f, 0.0f });
				}
				if (y == mH - 1)
				{
					/* bottom face */
					walls_vertices.push_back({ (float)x,   0.0f,     y+1.0f,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({   x+1.0f,   0.0f,     y+1.0f,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({   x+1.0f,  10.0f,     y+1.0f,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({ (float)x,  10.0f,     y+1.0f,  0.0f, 0.0f, -1.0f });
				}
			}
			else
			{
				if (prev_x < 0 || mMap[y*(int)mW + prev_x] == '0')
				{	
					/* add a wall at the left of the current cell */
					walls_vertices.push_back({ (float)x,   0.0f,  (float)y, -1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,   0.0f,    y+1.0f, -1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,  10.0f,    y+1.0f, -1.0f, 0.0f,  0.0f });
					walls_vertices.push_back({ (float)x,  10.0f,  (float)y, -1.0f, 0.0f,  0.0f });
				}
				
				if (prev_y < 0 || mMap[prev_y*(int)mW + x] == '0')
				{
					/* add a wall at the top of the current cell */
					walls_vertices.push_back({ (float)x,   0.0f, (float)y,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({   x+1.0f,   0.0f, (float)y,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({   x+1.0f,  10.0f, (float)y,  0.0f, 0.0f, -1.0f });
					walls_vertices.push_back({ (float)x,  10.0f, (float)y,  0.0f, 0.0f, -1.0f });
				}
			}
		}
	}
	walls = VertexArray(walls_vertices.data(), walls_vertices.size(), NULL, 0);
		
	Vertex ground_vertices[] = {
		/* bottom face */
		{      0.0f,  0.0f,      0.0f, 0.0f, 1.0f, 0.0f },
		{      0.0f,  0.0f, (float)mH, 0.0f, 1.0f, 0.0f },
		{ (float)mW,  0.0f, (float)mH, 0.0f, 1.0f, 0.0f },
		{ (float)mW,  0.0f,      0.0f, 0.0f, 1.0f, 0.0f }
	};
	int nb_vertices = sizeof(ground_vertices) / sizeof(Vertex);
	ground = VertexArray(ground_vertices, nb_vertices, NULL, 0);
	
	Vertex roof_vertices[] = {
		/* bottom face */
		{       0.0f, 10.0f,     0.0f, 0.0f, -1.0f, 0.0f },
		{       0.0f, 10.0f, (float)mH, 0.0f, -1.0f, 0.0f },
		{  (float)mW, 10.0f, (float)mH, 0.0f, -1.0f, 0.0f },
		{  (float)mW, 10.0f,     0.0f, 0.0f, -1.0f, 0.0f }
	};
	nb_vertices = sizeof(roof_vertices) / sizeof(Vertex);
	roof = VertexArray(roof_vertices, nb_vertices, NULL, 0);
}

void Level::draw(Shader &pShader)
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(-mW / 2.0f, 0.0f, -mH / 2.0f));
	
	glUniform3f(glGetUniformLocation(pShader.id(), "color"), 0.3f, 0.2f, 0.1f);
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	ground.draw(GL_QUADS);
	
	glUniform3f(glGetUniformLocation(pShader.id(), "color"), 0.15f, 0.15f, 0.1f);
	walls.draw(GL_QUADS);
	roof.draw(GL_QUADS);
}