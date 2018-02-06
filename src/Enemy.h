#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Object.h"
#include "glm/vec3.hpp"

class Shader;
class Enemy
{
public:
	Enemy(float pX, float pY);
	void draw(Shader &pShader);

	bool collide(float pX, float pY, float pZ);
	bool collide(glm::vec3 pPos);
private:
	Object mVolume;
	float mX;
	float mY;
};

#endif /* __ENEMY_H__ */
