#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f), float pPitch = 0.0f, float pYaw = 0.0f);
	
	void setPosition(glm::vec3 &pPosition);
	void updateDirection(float pDeltaX, float pDeltaY);
	glm::mat4 viewMatrix();
	
	inline void setSensitivity(float pSensitivity) { mSensitivity = pSensitivity; }
	inline float pitch(){ return mPitch; }
	inline float yaw(){ return mYaw; }
	inline const glm::vec3 &front(){ return mFront; }
	
private:
	void updateVectors();

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mRight;
	glm::vec3 mUp;

	float mPitch;
	float mYaw;
	float mSensitivity;
};

#endif // __CAMERA_H__