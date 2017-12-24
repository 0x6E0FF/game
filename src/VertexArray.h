#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

struct Vertex
{
	float values[6];
};

class VertexArray
{
public:
	VertexArray();
	VertexArray(const Vertex *vertices, 
				   unsigned int nb_vertices,
				   const unsigned int *indexes, 
				   unsigned int nb_indexes);
	void draw(int mode);

private:
	unsigned int mVAO;
	unsigned int mNbVertices;
	unsigned int mNbIndexes;
};

#endif /* __VERTEXARRAY_H__ */
