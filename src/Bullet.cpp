#include "Bullet.h"

#include "glad/glad.h"
#include "Shader.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Bullet::curId = 0;

Bullet::Bullet(const glm::vec3 &pStartPos, const glm::vec3 &pTrajectory)
	: mStartPos(pStartPos), mTrajectory(pTrajectory), mX(0.5),
	  mBullet(0.06f, 0.06f, 0.06f, 1.0f, 0.0f, 0.0f)
{
	mId = curId++;
}

void Bullet::draw(Shader &pShader, float pDeltaTime)
{
	mX += pDeltaTime * 100.0f;
	// cout << mX << " " << pDeltaTime << endl;
	
	glm::vec3 pos = mStartPos + mTrajectory * mX;
	// cout << "#" << mId << " " << pos.x << " " << pos.y << " " << pos.z << endl;
	
	glm::mat4 model;
	model = glm::translate(model, pos);
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, 
		glm::value_ptr(model));
	
	mBullet.draw(pShader);
}

