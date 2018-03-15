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
	
	// glm::vec2 p1, p2, p3;
	// p1.x = mPosition.x;
	// p1.y = mPosition.z;
	// p2.x = lNewPos.x;
	// p2.y = lNewPos.z;
	
	// float dx = p2.x - p1.x;
	// float dy = p2.y - p1.y;
	
	// int next_x = (int)(p1.x + ((dx < 0) ? 0.0 : 1.0));
	// int next_y = (int)(p1.y + ((dy < 0) ? 0.0 : 1.0));
	
	// while ( ((dx < 0 && p1.x >= p2.x) || (dx >= 0 && p1.x <= p2.x))
       	 // || ((dy < 0 && p1.y >= p2.y) || (dy >= 0 && p1.y <= p2.y)))
	// {
		// if (dx == 0.0)
		// {
			// p3.x = p1.x;
			// p2.y = next_y;
			// next_y += (dy < 0) ? -1 : 1;
		// }
		// else if (dy == 0.0)
		// {
			// p3.x = next_x;
			// p3.y = p1.y;
			// next_x += (dx < 0) ? -1 : 1;
		// }
		// else 
		// {
			// /* general case : compute next intersection of line with the grid */
			
			// /* first, try fixing x and compute y with line equation : */
			// /*            dy
			 // *  y - y1 = ---- . (x - x1)
			 // *            dx 
			 // */
			// p3.x = next_x;
			// p3.y = (dy / dx) * (p3.x - p1.x) + p1.y;

			// /* if computed point doesn't lie on current cell border, retry with fixed y */
			// if ((dy <  0 && p3.y <  next_y)
			 // || (dy >= 0 && p3.y >= next_y))
			// {
				// p3.y = next_y;
				// p3.x = (dx / dy) * (p3.y - p1.y) + p1.x;
				
				// next_y += (dy < 0) ? -1 : 1;
			// }
			// else
			// {
				// next_x += (dx < 0) ? -1 : 1;
			// }
		// }
		
		// /* cell coordinates */
		// int x = (int)p3.x + mLevel->width() / 2;
		// int y = (int)p3.y + mLevel->height() / 2;
		// if (dx < 0 && (float)x == p1.x)
			// x--;
		// if (dy < 0 && (float)y == p1.y)
			// y--;
		
		// if (mLevel->map()[y * mLevel->width() + x] == '1')
		// {
			// mPosition.x = p3.x;
			// mPosition.z = p3.y;
			// mCamera.setPosition(mPosition);
			// cout << "cap to " << x << " " << y << endl;
			// return;
		// }
		// p1 = p3;
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
