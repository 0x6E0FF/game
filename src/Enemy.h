#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Object.h"

class Shader;
class Enemy
{
public:
	Enemy(float pX, float pY);
	void draw(Shader &pShader);
private:
	Object mVolume;
	float mX;
	float mY;
};

#endif /* __ENEMY_H__ */
