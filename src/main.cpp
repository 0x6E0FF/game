#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

using namespace std;

struct Vertex
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
};

class ColoredVertexArray
{
public:
	ColoredVertexArray(): mVAO(0), mNbIndexes(0), mNbVertices(0){} 
	ColoredVertexArray(const Vertex *vertices, 
					   unsigned int nb_vertices,
					   const unsigned int *indexes, 
					   unsigned int nb_indexes)
	{
		unsigned int VBO;
		
		mNbVertices = nb_vertices;
		mNbIndexes = 0;
		
		glGenBuffers(1, &VBO);  
		glGenVertexArrays(1, &mVAO);  

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mNbVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		
		if (indexes != NULL)
		{
			unsigned int EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nb_indexes * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
			mNbIndexes = nb_indexes;
		}
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
		glEnableVertexAttribArray(1); 
	}
	void draw(GLenum mode)
	{
		glBindVertexArray(mVAO);
		if (mNbIndexes > 0)
			glDrawElements(mode, mNbIndexes, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(mode, 0, mNbVertices);
		glBindVertexArray(0);
	}
	unsigned int mVAO;
	unsigned int mNbVertices;
	unsigned int mNbIndexes;
};

class Grid
{
public:
	Grid(int nb, float increment)
	{
		Vertex *vertices;
		Vertex *vertex_cur;
		float min = -nb * increment / 2.0f;
		float max = min + nb * increment;
		
		unsigned int nb_vertices = (nb+1) * 2 * 2;
		vertices = new Vertex[nb_vertices];
		vertex_cur = vertices;
		for(int i = 0; i <= nb; ++i)
		{
			*vertex_cur = { min, 0.0f, min + i * increment, (float)i/nb, 1.0f - (float)i/nb, 0.0f}; vertex_cur++;
			*vertex_cur = { max, 0.0f, min + i * increment, (float)i/nb, 1.0f - (float)i/nb, 0.0f}; vertex_cur++;
		}	
		for(int i = 0; i <= nb; ++i)
		{
			*vertex_cur = { min + i * increment, 0.0f, min, 1.0f - (float)i/nb, 0.0f, (float)i/nb}; vertex_cur++;
			*vertex_cur = { min + i * increment, 0.0f, max, 1.0f - (float)i/nb, 0.0f, (float)i/nb}; vertex_cur++;
		}	
		
		drawer = ColoredVertexArray(vertices, nb_vertices, 0, 0);
		delete vertices;
	}
	
	void draw(Shader &shader)
	{
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		drawer.draw(GL_LINES);
	}
	ColoredVertexArray drawer;
};

class Cube
{
public:
	Cube()
	{
		Vertex vertices[] = {
			{ -0.7f,  0.7f,  0.7f,  0.7f, 0.0f, 0.5f },
			{  0.7f,  0.7f,  0.7f,  0.3f, 0.2f, 0.5f },
			{  0.7f, -0.7f,  0.7f,  0.1f, 0.6f, 0.2f },
			{ -0.7f, -0.7f,  0.7f,  0.7f, 0.7f, 0.5f },
			{ -0.7f,  0.7f, -0.7f,  1.0f, 0.0f, 0.3f },
			{  0.7f,  0.7f, -0.7f,  0.0f, 0.3f, 0.8f },
			{  0.7f, -0.7f, -0.7f,  0.1f, 0.9f, 0.2f },
			{ -0.7f, -0.7f, -0.7f,  0.5f, 0.0f, 0.7f }
		};
		unsigned int indexes[] = 
		{
			0, 1, 2, 3,
			1, 5, 6, 2,
			5, 4, 7, 6,
			4, 0, 3, 7,
			0, 1, 5, 4,
			3, 2, 6, 7
		};
		unsigned int nb_vertices = sizeof(vertices) / sizeof(Vertex);
		unsigned int nb_indexes = sizeof(indexes) / sizeof(unsigned int);
		drawer = ColoredVertexArray(vertices, nb_vertices, indexes, nb_indexes);
	}
	
	void draw(Shader &shader)
	{
		glm::mat4 model;

		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		drawer.draw(GL_QUADS);
	}
	
	ColoredVertexArray drawer;
};

class Bullet
{
public:
	static int curId;
	
	Bullet(const glm::vec3 &pStartPos, const glm::vec3 &pTrajectory)
	: mStartPos(pStartPos), mTrajectory(pTrajectory), mX(0.5)
	{
		mId = curId++;
		Vertex vertices[] = {
			{ -0.03f,  0.03f,  0.03f,  1.0f, 0.0f, 0.0f },
			{  0.03f,  0.03f,  0.03f,  1.0f, 0.0f, 0.0f },
			{  0.03f, -0.03f,  0.03f,  1.0f, 0.0f, 0.0f },
			{ -0.03f, -0.03f,  0.03f,  1.0f, 0.0f, 0.0f },
			{ -0.03f,  0.03f, -0.03f,  1.0f, 0.0f, 0.0f },
			{  0.03f,  0.03f, -0.03f,  1.0f, 0.0f, 0.0f },
			{  0.03f, -0.03f, -0.03f,  1.0f, 0.0f, 0.0f },
			{ -0.03f, -0.03f, -0.03f,  1.0f, 0.0f, 0.0f }
		};
		unsigned int indexes[] = 
		{
			0, 1, 2, 3,
			1, 5, 6, 2,
			5, 4, 7, 6,
			4, 0, 3, 7,
			0, 1, 5, 4,
			3, 2, 6, 7
		};
		unsigned int nb_vertices = sizeof(vertices) / sizeof(Vertex);
		unsigned int nb_indexes = sizeof(indexes) / sizeof(unsigned int);
		mBulletVertices = ColoredVertexArray(vertices, nb_vertices, indexes, nb_indexes);
	}
	void draw(Shader &pShader, float pDeltaTime)
	{
		mX += pDeltaTime * 100.0f;
		// cout << mX << " " << pDeltaTime << endl;
		
		glm::vec3 pos = mStartPos + mTrajectory * mX;
		// cout << "#" << mId << " " << pos.x << " " << pos.y << " " << pos.z << endl;
		
		glm::mat4 model;
		model = glm::translate(model, pos);
		glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, 
			glm::value_ptr(model));
		
		mBulletVertices.draw(GL_QUADS);
	}
	int mId;
	float mX;
	ColoredVertexArray mBulletVertices;
	glm::vec3 mTrajectory;
	glm::vec3 mStartPos;
};
int Bullet::curId = 0;

class Avatar
{	
const float SENSITIVTY =  0.1f;
public:
	Avatar() :
		mPosition(glm::vec3(0.0f, 2.0f,  10.0f)),
		mCamera(mPosition, 0.0f, -90.0f),
		mMoveFront(glm::vec3(0.0f, 0.0f, -1.0f)),
		mMoveRight(glm::vec3(1.0f, 0.0f,  0.0f)),
		mBullets()
	{
		Vertex weaponVtx[] = {
			{ -0.5f,  0.05f,  0.05f,  0.0f, 0.2f, 0.5f },
			{  0.5f,  0.05f,  0.05f,  0.0f, 0.2f, 0.5f },
			{  0.5f, -0.05f,  0.05f,  0.0f, 0.2f, 0.5f },
			{ -0.5f, -0.05f,  0.05f,  0.0f, 0.2f, 0.5f },
			{ -0.5f,  0.05f, -0.05f,  0.0f, 0.2f, 0.5f },
			{  0.5f,  0.05f, -0.05f,  0.0f, 0.2f, 0.5f },
			{  0.5f, -0.05f, -0.05f,  0.0f, 0.2f, 0.5f },
			{ -0.5f, -0.05f, -0.05f,  0.0f, 0.2f, 0.5f }
		};
		unsigned int indexes[] = 
		{
			0, 1, 2, 3,
			1, 5, 6, 2,
			5, 4, 7, 6,
			4, 0, 3, 7,
			0, 1, 5, 4,
			3, 2, 6, 7
		};
		unsigned int nb_vertices = sizeof(weaponVtx) / sizeof(Vertex);
		unsigned int nb_indexes = sizeof(indexes) / sizeof(unsigned int);
		weapon = ColoredVertexArray(weaponVtx, nb_vertices, indexes, nb_indexes);
	}
	
	void forward(float pSpeed)  { mPosition += pSpeed * mMoveFront; mCamera.setPosition(mPosition); }
	void backward(float pSpeed) { mPosition -= pSpeed * mMoveFront; mCamera.setPosition(mPosition); }
	void left(float pSpeed)     { mPosition -= pSpeed * mMoveRight; mCamera.setPosition(mPosition); }
	void right(float pSpeed)    { mPosition += pSpeed * mMoveRight; mCamera.setPosition(mPosition); }
	
	void changeDirection(float pDeltaX, float pDeltaY)
	{
		mCamera.updateDirection(pDeltaX, pDeltaY);
		
		float yawRad = glm::radians(mCamera.yaw());
		mMoveFront.x = cos(yawRad);
        mMoveFront.y = 0.0f;
        mMoveFront.z = sin(yawRad);
		
		mMoveFront = glm::normalize(mMoveFront);
        mMoveRight = glm::normalize(glm::cross(mMoveFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	
	glm::mat4 viewMatrix()
	{
		return mCamera.viewMatrix();
	}
	
	void shoot()
	{
		// Bullet b;
		mBullets.push_back(Bullet(mWeaponPos, mCamera.front()));
		cout << '\a';
	}
	
	void draw(Shader &pShader, float pDeltaTime)
	{
		glm::mat4 model;
		mWeaponPos = mPosition + mMoveFront * 0.5f + mMoveRight * 0.3f + glm::vec3(0.0f, -0.3f, 0.0f);
		model = glm::translate(model, mWeaponPos);
		model = glm::rotate(model, -glm::radians(mCamera.yaw()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mCamera.pitch()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		glUniformMatrix4fv(glGetUniformLocation(pShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		weapon.draw(GL_QUADS);
		
		/* lazer */
		// glBegin(GL_LINES);
		// glVertex3f(0.0f, 0.0f, 0.0f);
		// glVertex3f(100.0f, 0.0f, 0.0f);
		// glEnd();
		
		for(auto it = mBullets.begin(); it != mBullets.end(); ++it)
		{
			it->draw(pShader, pDeltaTime);
		}
		while (!mBullets.empty() && mBullets.front().mX > 100.0)
		{
			mBullets.pop_front();
		}
	}
	
	glm::vec3 mPosition;
	glm::vec3 mMoveFront;
	glm::vec3 mMoveRight;
	Camera mCamera;
	ColoredVertexArray weapon;
	glm::vec3 mWeaponPos;
	list<Bullet> mBullets;
};


struct SceneData
{
	Avatar avatar;
	double prevx;
	double prevy;
	bool first;
	float fov;
};

static void keyboard_input(GLFWwindow* window, float deltaTime)
{
	SceneData *data = (SceneData *)glfwGetWindowUserPointer(window);
	float speed = deltaTime *  5.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		data->avatar.forward(speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		data->avatar.backward(speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		data->avatar.left(speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		data->avatar.right(speed);
	
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		data->fov += 5.0f;
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		data->fov -= 5.0f;
	
	if (data->fov < 5.0f) data->fov = 5.0f;
	if (data->fov > 170.0f) data->fov = 170.0f;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	SceneData *data = (SceneData *)glfwGetWindowUserPointer(window);
	if (data->first)
	{
		data->first = false;
	}
	else
	{
		data->avatar.changeDirection(xpos - data->prevx, data->prevy - ypos);
	}	
	data->prevx = xpos;
	data->prevy = ypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	SceneData *data = (SceneData *)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        data->avatar.shoot();
}

int main(void)
{
    GLFWwindow* window;
	float curFrame = 0.0f;
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
	
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	
	SceneData sceneData;
	sceneData.first = true;
	sceneData.fov = 45.0;
	glfwSetWindowUserPointer(window, &sceneData);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); 
	glLineWidth(1);
	
	char *ver = (char *)glGetString(GL_VERSION);
	cout << ver << endl;
	
	Shader shader1 = Shader("src/shader.vert","src/shader.frag");

	Vertex ground[] = {
        {-100.0f, 0.0f, -100.0f,   1.0f, 0.0f, 0.0f },
        {-100.0f, 0.0f,  100.0f,   0.0f, 1.0f, 0.0f },
        { 100.0f, 0.0f,  100.0f,   0.0f, 0.0f, 1.0f },
		{ 100.0f, 0.0f, -100.0f,   0.0f, 1.0f, 1.0f }
	};
	
	Vertex origin[] = {
		{ 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		
		{ 0.0f, 0.0f,  0.0f,   0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f,  0.0f,   0.0f, 1.0f, 0.0f },
		
		{ 0.0f, 0.0f,  0.0f,   0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f,  1.0f,   0.0f, 0.0f, 1.0f }
	};
	
	Vertex test[] = {
       {  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f },
       {  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f },
       { -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f },
	   { -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f }
	};
	
	// ColoredVertexArray groundObject(ground, sizeof(ground) / sizeof(Vertex));
	ColoredVertexArray originObject(origin, sizeof(origin) / sizeof(Vertex), 0, 0);
	ColoredVertexArray testObject(test, sizeof(test) / sizeof(Vertex), 0, 0);
	
	Grid grid(10, 1.0);
	Cube cube;
	
	glm::mat4 model;
	glm::mat4 view;
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f)); 
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	shader1.use();
		
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		keyboard_input(window, deltaTime);
		
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		
		glm::mat4 projection = glm::perspective(glm::radians(sceneData.fov), 800.0f/600.0f, 0.1f, 100.0f);	
		glUniformMatrix4fv(glGetUniformLocation(shader1.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(glGetUniformLocation(shader1.id(), "view"), 1, GL_FALSE, glm::value_ptr(sceneData.avatar.viewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader1.id(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
		
		originObject.draw(GL_LINES);
		grid.draw(shader1);
		cube.draw(shader1);
		
		model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader1.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		testObject.draw(GL_LINE_LOOP);
		
		sceneData.avatar.draw(shader1, deltaTime);
		// cout << 1.0f / deltaTime << " FPS" << endl;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}