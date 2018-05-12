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

#include <iostream>

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

glm::vec3 Level::intersectWall(glm::vec3 &pStart, glm::vec3 &pEnd)
{
	glm::vec3 ret;
	glm::vec2 p1, p2, p3;
	p1.x = pStart.x;
	p1.y = pStart.z;
	p2.x = pEnd.x;
	p2.y = pEnd.z;
	
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	
	int next_x = (int)(p1.x + ((dx < 0) ? 0.0 : 1.0));
	int next_y = (int)(p1.y + ((dy < 0) ? 0.0 : 1.0));
	
	while ( ((dx < 0 && p1.x > p2.x) || (dx >= 0 && p1.x < p2.x))
       	 || ((dy < 0 && p1.y > p2.y) || (dy >= 0 && p1.y < p2.y)))
	{
		if (dx == 0.0)
		{
			p3.x = p1.x;
			p3.y = next_y;
			next_y += (dy < 0) ? -1 : 1;
		}
		else if (dy == 0.0)
		{
			p3.x = next_x;
			p3.y = p1.y;
			next_x += (dx < 0) ? -1 : 1;
		}
		else 
		{
			/* general case : compute next intersection of line with the grid */
			
			/* first, try fixing x and compute y with line equation : */
			/*            dy
			 *  y - y1 = ---- . (x - x1)
			 *            dx 
			 */
			p3.x = next_x;
			p3.y = (dy / dx) * (p3.x - p1.x) + p1.y;

			/* if computed point doesn't lie on current cell border, retry with fixed y */
			if ((dy <  0 && p3.y <  next_y)
			 || (dy >= 0 && p3.y >= next_y))
			{
				p3.y = next_y;
				p3.x = (dx / dy) * (p3.y - p1.y) + p1.x;
				
				next_y += (dy < 0) ? -1 : 1;
			}
			else
			{
				next_x += (dx < 0) ? -1 : 1;
			}
		}
		
		/* cell coordinates */
		int x = (int)p3.x;
		int y = (int)p3.y;
		
		if (dx < 0 && (float)x == p3.x)
			x--;
		if (dy < 0 && (float)y == p3.y)
			y--;
		
		x += mW / 2;
		y += mH / 2;
		if (x < 0 || y < 0 || x >= mW || y >= mH || mMap[y * mW + x] == '1')
		{
			// if (dx < 0 && dy < 0)
				// p3 = p1;
				
			break;
		}
		p1 = p3;
	}
	
	ret.x = p3.x;
	ret.z = p3.y;
	ret.y = pStart.y;
	
	glm::vec3 e = pEnd;
	e.y = pStart.y;
	
	/* Thales theorem */
	float d1 = glm::distance(pStart, e);
	float d2 = glm::distance(pStart, ret);
	
	ret.y += d2 * (pEnd.y - pStart.y) / d1;
	
	return ret;
}
