#include "Enemy.h"

#include "Shader.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Enemy::Enemy(float pX, float pY) 
	: mX(pX), mY(pY), mVolume(0.4f, 3.0f, 0.4f, 0.8f, 0.1f, 0.1f)
{
}
	
void Enemy::draw(Shader &pShader)
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(mX, 1.5f, mY));
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	mVolume.draw(pShader);
}

bool Enemy::collide(float pX, float pY, float pZ)
{
	glm::vec3 p(pX - mX, pY - 0.5f, pZ - mY);
	float halfX = mVolume.sizeX() / 2.0f;
	float halfY = mVolume.sizeY() / 2.0f;
	float halfZ = mVolume.sizeZ() / 2.0f;
	return p.x > -halfX && p.x < halfX &&
		   p.y > -halfY && p.y < halfY &&
		   p.z > -halfZ && p.z < halfZ;
}

bool Enemy::collide(glm::vec3 pPos)
{
	return collide(pPos.x, pPos.y, pPos.z);
}
