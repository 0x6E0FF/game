#include "VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray() :mVAO(0), mNbIndexes(0), mNbVertices(0)
{
}
 
VertexArray::VertexArray(const Vertex *vertices, 
				   unsigned int nb_vertices,
				   const unsigned int *indexes, 
				   unsigned int nb_indexes)
{
	unsigned int VBO;
	
	mNbVertices = nb_vertices;
	mNbIndexes = 0;
	
	glGenBuffers(1, &VBO);  
	glGenVertexArrays(1, &mVAO);  

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mNbVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	if (indexes != NULL)
	{
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nb_indexes * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
		mNbIndexes = nb_indexes;
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1); 
}
void VertexArray::draw(int mode)
{
	glBindVertexArray(mVAO);
	if (mNbIndexes > 0)
		glDrawElements(mode, mNbIndexes, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(mode, 0, mNbVertices);
	glBindVertexArray(0);
}