#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "VertexArray.h"

class Shader;
class Object
{
public:
	Object(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void draw(Shader &shader);

	inline float sizeX() const { return mSizeX; }
	inline float sizeY() const { return mSizeY; }
	inline float sizeZ() const { return mSizeZ; }

private:
	VertexArray mDrawer;
	float mSizeX;
	float mSizeY;
	float mSizeZ;
	float mR;
	float mG;
	float mB;
};

#endif /* __OBJECT_H__ */
