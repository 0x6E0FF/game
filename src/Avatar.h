#ifndef __AVATAR_H__
#define __AVATAR_H__

#include "Camera.h"
#include "Object.h"
#include "Bullet.h"
#include "glm/vec3.hpp"
#include <list>
#include <string>

class Shader;
class Level;
class Avatar
{
public:
	const float SENSITIVTY =  0.1f;

	Avatar();
	
	void forward(float pSpeed);
	void backward(float pSpeed);
	void left(float pSpeed);
	void right(float pSpeed);
	void changeDirection(float pDeltaX, float pDeltaY);
	void shoot();
	void draw(Shader &pShader, float pDeltaTime);
	
	void setLevel(Level *pLevel);
	
	inline std::list<Bullet>::const_iterator bullets_begin() const { return mBullets.begin(); }
	inline std::list<Bullet>::const_iterator bullets_end() const { return mBullets.end(); }
	inline glm::mat4 viewMatrix() { return mCamera.viewMatrix(); }
	
private:
	void updatePosition(glm::vec3 pMoveVector);

	glm::vec3 mPosition;
	glm::vec3 mMoveFront;
	glm::vec3 mMoveRight;
	Camera mCamera;
	Object mWeapon;
	glm::vec3 mWeaponPos;
	std::list<Bullet> mBullets;
	Level *mLevel;
};

#endif /* __AVATAR_H__ */
