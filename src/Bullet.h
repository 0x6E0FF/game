#ifndef __BULLET_H__
#define __BULLET_H__

#include "Object.h"
#include "glm/vec3.hpp"

class Shader;
class Bullet
{
public:
	static int curId;
	Bullet(const glm::vec3 &pStartPos, const glm::vec3 &pTrajectory);
	void draw(Shader &pShader, float pDeltaTime);
	inline float dist() const { return mDist; }
	inline glm::vec3 pos() const { return mStartPos + mTrajectory * mDist; }
private:
	int mId;
	float mDist;
	Object mBullet;
	glm::vec3 mTrajectory;
	glm::vec3 mStartPos;
};

#endif /* __BULLET_H__ */
