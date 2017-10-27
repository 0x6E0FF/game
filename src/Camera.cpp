#include "Camera.h"

Camera::Camera(glm::vec3 pPosition, float pPitch, float pYaw)
	: mPosition(pPosition), mPitch(pPitch), mYaw(pYaw), mSensitivity(0.1)
{
	updateVectors();
}

void Camera::setPosition(glm::vec3 &pPosition)
{
	mPosition = pPosition;
}

void Camera::updateDirection(float pDeltaX, float pDeltaY)
{
	mPitch += pDeltaY * mSensitivity;
	if (mPitch > 89.0f) 
		mPitch = 89.0f;
	else if (mPitch < -89.0f) 
		mPitch = -89.0f;
	mYaw += pDeltaX * mSensitivity;
	updateVectors();
}

glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateVectors()
{
	glm::vec3 front;
	float pitchRad = glm::radians(mPitch);
	float yawRad = glm::radians(mYaw);
	
	front.x = cos(pitchRad) * cos(yawRad);
	front.y = sin(pitchRad);
	front.z = cos(pitchRad) * sin(yawRad);
	
	mFront = glm::normalize(front);
	mRight = glm::normalize(glm::cross(mFront, glm::vec3(0.0f, 1.0f,  0.0f)));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}
