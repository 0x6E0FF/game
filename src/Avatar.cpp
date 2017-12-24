#include "Avatar.h"

#include "glad/glad.h"
#include "Shader.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

Avatar::Avatar() :
		mPosition(glm::vec3(0.0f, 2.0f,  10.0f)),
		mCamera(mPosition, 0.0f, -90.0f),
		mMoveFront(glm::vec3(0.0f, 0.0f, -1.0f)),
		mMoveRight(glm::vec3(1.0f, 0.0f,  0.0f)),
		mBullets(),
		mWeapon(1.0f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f)
{
}

void Avatar::forward(float pSpeed)  { mPosition += pSpeed * mMoveFront; mCamera.setPosition(mPosition); }
void Avatar::backward(float pSpeed) { mPosition -= pSpeed * mMoveFront; mCamera.setPosition(mPosition); }
void Avatar::left(float pSpeed)     { mPosition -= pSpeed * mMoveRight; mCamera.setPosition(mPosition); }
void Avatar::right(float pSpeed)    { mPosition += pSpeed * mMoveRight; mCamera.setPosition(mPosition); }

void Avatar::changeDirection(float pDeltaX, float pDeltaY)
{
	mCamera.updateDirection(pDeltaX, pDeltaY);
	
	float yawRad = glm::radians(mCamera.yaw());
	mMoveFront.x = cos(yawRad);
	mMoveFront.y = 0.0f;
	mMoveFront.z = sin(yawRad);
	
	mMoveFront = glm::normalize(mMoveFront);
	mMoveRight = glm::normalize(glm::cross(mMoveFront, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Avatar::shoot()
{
	mBullets.push_back(Bullet(mWeaponPos, mCamera.front()));
	cout << '\a';
}

void Avatar::draw(Shader &pShader, float pDeltaTime)
{
	glm::mat4 model;
	mWeaponPos = mPosition + mMoveFront * 0.5f + mMoveRight * 0.3f + glm::vec3(0.0f, -0.3f, 0.0f);
	model = glm::translate(model, mWeaponPos);
	model = glm::rotate(model, -glm::radians(mCamera.yaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mCamera.pitch()), glm::vec3(0.0f, 0.0f, 1.0f));
	
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	mWeapon.draw(pShader);
	
	/* lazer */
	// glBegin(GL_LINES);
	// glVertex3f(0.0f, 0.0f, 0.0f);
	// glVertex3f(100.0f, 0.0f, 0.0f);
	// glEnd();
	
	for(auto it = mBullets.begin(); it != mBullets.end(); ++it)
	{
		it->draw(pShader, pDeltaTime);
	}
	while (!mBullets.empty() && mBullets.front().x() > 100.0)
	{
		mBullets.pop_front();
	}
}
