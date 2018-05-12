#include "Avatar.h"

#include "glad/glad.h"
#include "Shader.h"
#include "Level.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <string>

using namespace std;

Avatar::Avatar() :
		mPosition(glm::vec3(0.0f, 2.0f,  10.0f)),
		mCamera(mPosition, 0.0f, -90.0f),
		mMoveFront(glm::vec3(0.0f, 0.0f, -1.0f)),
		mMoveRight(glm::vec3(1.0f, 0.0f,  0.0f)),
		mBullets(),
		mWeapon(1.0f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f),
		mLevel(0)
{
}

void Avatar::updatePosition(glm::vec3 pMoveVector)
{
	// int x,y;
	glm::vec3 lNewPos = mPosition + pMoveVector;
	
	// x = lNewPos.x + mLevel->width() / 2;
	// y = lNewPos.z + mLevel->height() / 2;
	// cout << x << " " << y << endl;
		
	// if (x >= 0 && x < mLevel->width() && y >= 0 && y < mLevel->height()
		// && mLevel->map()[y * mLevel->width() + x] == '0')
	// {
		// mPosition = lNewPos;
		// mCamera.setPosition(mPosition);		
	// }
	// else
	// {
		// int prevx = mPosition.x + mLevel->width() / 2;
		// int prevy = mPosition.z + mLevel->height() / 2;
		// if (prevx < x)
			// mPosition.x = (float)prevx+0.8f - mLevel->width() / 2;
		// else if (prevx > x)
			// mPosition.x = (float)prevx+0.2f - mLevel->width() / 2;
		// else
			// mPosition.x = lNewPos.x;
			
		// if (prevy < y)
			// mPosition.z = (float)prevy+0.8f - mLevel->height() / 2;
		// else if (prevy > y)
			// mPosition.z = (float)prevy+0.2f - mLevel->height() / 2;
		// else
			// mPosition.z = lNewPos.z;
		
		// mPosition.y = lNewPos.y;
		// mCamera.setPosition(mPosition);		
	// }
	

	// cout << "move " << dx << " "  << dy << endl;
	mPosition = lNewPos;
	mCamera.setPosition(mPosition);
}

void Avatar::forward(float pSpeed)  { updatePosition( pSpeed * mMoveFront); }
void Avatar::backward(float pSpeed) { updatePosition(-pSpeed * mMoveFront); }
void Avatar::left(float pSpeed)     { updatePosition(-pSpeed * mMoveRight); }
void Avatar::right(float pSpeed)    { updatePosition( pSpeed * mMoveRight); }

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
	while (!mBullets.empty() && mBullets.front().dist() > 100.0)
	{
		mBullets.pop_front();
	}
}

void Avatar::setLevel(Level *pLevel)
{ 
	mLevel = pLevel; 
}
