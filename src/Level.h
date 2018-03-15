#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "VertexArray.h"
#include <string>

class Shader;

class Level
{
public:	
	Level(const char *pPBMFile);
	
	void draw(Shader &pShader);
	
	inline const std::string &map() const { return mMap; }
	inline int width() const { return mW; }
	inline int height() const { return mH; }
	
private:
	VertexArray walls;
	VertexArray ground;
	VertexArray roof;
	int mW;
	int mH;	
	std::string mMap;
};

#endif  /* __LEVEL_H__ */