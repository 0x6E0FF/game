#include "Object.h"

#include <glad/glad.h>
#include "Shader.h"

Object::Object(float sizeX, float sizeY, float sizeZ, float r, float g, float b)
{
	float halfX = sizeX / 2.0f;
	float halfY = sizeY / 2.0f;
	float halfZ = sizeZ / 2.0f;
	Vertex vertices[] = {
		/* front face */
		{ -halfX,  halfY,  halfZ,  0.0f, 0.0f, 1.0f },
		{  halfX,  halfY,  halfZ,  0.0f, 0.0f, 1.0f },
		{  halfX, -halfY,  halfZ,  0.0f, 0.0f, 1.0f },
		{ -halfX, -halfY,  halfZ,  0.0f, 0.0f, 1.0f },
	
		/* right face */
		{  halfX,  halfY,  halfZ,  1.0f, 0.0f, 0.0f },
		{  halfX,  halfY, -halfZ,  1.0f, 0.0f, 0.0f },
		{  halfX, -halfY, -halfZ,  1.0f, 0.0f, 0.0f },
		{  halfX, -halfY,  halfZ,  1.0f, 0.0f, 0.0f },
		
		/* back face */
		{ -halfX,  halfY, -halfZ,  0.0f, 0.0f, -1.0f },
		{  halfX,  halfY, -halfZ,  0.0f, 0.0f, -1.0f },
		{  halfX, -halfY, -halfZ,  0.0f, 0.0f, -1.0f },
		{ -halfX, -halfY, -halfZ,  0.0f, 0.0f, -1.0f },
		
		/* left face */
		{  -halfX,  halfY,  halfZ, -1.0f, 0.0f, 0.0f },
		{  -halfX,  halfY, -halfZ, -1.0f, 0.0f, 0.0f },
		{  -halfX, -halfY, -halfZ, -1.0f, 0.0f, 0.0f },
		{  -halfX, -halfY,  halfZ, -1.0f, 0.0f, 0.0f },
		
		/* top face */
		{   halfX,  halfY,  halfZ, 0.0f, 1.0f, 0.0f },
		{  -halfX,  halfY,  halfZ, 0.0f, 1.0f, 0.0f },
		{  -halfX,  halfY, -halfZ, 0.0f, 1.0f, 0.0f },
		{   halfX,  halfY, -halfZ, 0.0f, 1.0f, 0.0f },
		
		/* bottom face */
		{   halfX, -halfY,  halfZ, 0.0f, -1.0f, 0.0f },
		{  -halfX, -halfY,  halfZ, 0.0f, -1.0f, 0.0f },
		{  -halfX, -halfY, -halfZ, 0.0f, -1.0f, 0.0f },
		{   halfX, -halfY, -halfZ, 0.0f, -1.0f, 0.0f }
	};
	unsigned int nb_vertices = sizeof(vertices) / sizeof(Vertex);
	mDrawer = VertexArray(vertices, nb_vertices, NULL, 0);
	
	mR = r;
	mG = g;
	mB = b;
}

void Object::draw(Shader &pShader)
{
	glUniform3f(glGetUniformLocation(pShader.id(), "color"), mR, mG, mB);
	mDrawer.draw(GL_QUADS);
}
