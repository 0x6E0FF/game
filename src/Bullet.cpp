#include "Bullet.h"

#include "glad/glad.h"
#include "Shader.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Bullet::curId = 0;

Bullet::Bullet(const glm::vec3 &pStartPos, const glm::vec3 &pTrajectory)
	: mStartPos(pStartPos), mTrajectory(pTrajectory), mDist(0.5),
	  mBullet(0.06f, 0.06f, 0.06f, 1.0f, 0.0f, 0.0f)
{
	mId = curId++;
}

void Bullet::draw(Shader &pShader, float pDeltaTime)
{	
	mDist += pDeltaTime * 100.0f;
	glm::mat4 model = glm::translate(glm::mat4(), pos());
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	mBullet.draw(pShader);
}
