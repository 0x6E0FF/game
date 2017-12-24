#include "Enemy.h"

#include "Shader.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Enemy::Enemy(float pX, float pY) 
	: mX(pX), mY(pY), mVolume(0.3f, 1.0f, 0.3f, 0.8f, 0.1f, 0.1f)
{
}
	
void Enemy::draw(Shader &pShader)
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(mX, 0.5f, mY));
	glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	mVolume.draw(pShader);
}