#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"
#include "Object.h"
#include "Bullet.h"
#include "Shader.h"
#include "Camera.h"
#include "Avatar.h"
#include "Enemy.h"
#include "Level.h"

#define M_PI 3.14159265359f

#define WIDTH 1440.0f
#define HEIGHT 900.0f

using namespace std;

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
			*vertex_cur = { min, 0.1f, min + i * increment, (float)i/nb, 1.0f - (float)i/nb, 0.0f}; vertex_cur++;
			*vertex_cur = { max, 0.1f, min + i * increment, (float)i/nb, 1.0f - (float)i/nb, 0.0f}; vertex_cur++;
		}	
		for(int i = 0; i <= nb; ++i)
		{
			*vertex_cur = { min + i * increment, 0.1f, min, 1.0f - (float)i/nb, 0.0f, (float)i/nb}; vertex_cur++;
			*vertex_cur = { min + i * increment, 0.1f, max, 1.0f - (float)i/nb, 0.0f, (float)i/nb}; vertex_cur++;
		}	
		
		drawer = VertexArray(vertices, nb_vertices, 0, 0);
		delete vertices;
	}
	
	void draw(Shader &shader)
	{
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		drawer.draw(GL_LINES);
	}
	VertexArray drawer;
};

struct SceneData
{
	Avatar avatar;
	double prevx;
	double prevy;
	bool first;
	float fov;
};

static int max_framerate = 60;
static float speed_factor = 5.0F;
static bool print_framerate = false;

static void keyboard_input(GLFWwindow* window, float deltaTime)
{
	SceneData *data = (SceneData *)glfwGetWindowUserPointer(window);
	
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		speed_factor -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		speed_factor += 1.0f;
	
	float speed = deltaTime *  speed_factor;
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
	
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		print_framerate ^= 1;
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		max_framerate -= 1;
    if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)
		max_framerate += 1;
	
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

	glfwWindowHint(GLFW_VISIBLE, 0);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 1920-WIDTH- 50, 50);
	glfwShowWindow(window);
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
	
	Shader vertColorShader = Shader("src/shader.vert","src/shader.frag");
	Shader lightingShader = Shader("src/lighting.vert","src/lighting.frag");
	Shader lightSourceShader = Shader("src/lightSource.vert","src/lightSource.frag");
	
	Vertex origin[] = {
		{ 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		
		{ 0.0f, 0.0f,  0.0f,   0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f,  0.0f,   0.0f, 1.0f, 0.0f },
		
		{ 0.0f, 0.0f,  0.0f,   0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f,  1.0f,   0.0f, 0.0f, 1.0f }
	};
	
	VertexArray originObject(origin, sizeof(origin) / sizeof(Vertex), 0, 0);
	
	Grid grid(50, 1.0);
	Object cube(1.4f, 1.4f, 1.4f, 0.0f, 0.0f, 1.0f);
	Object lightSource(0.5f);
	glm::mat4 testCubeModel;
	glm::mat4 view;
	glm::mat4 model;
	vector<Enemy> enemies;
	for (int i = 0; i < 10; i++)
	{
		enemies.push_back(Enemy(float(25 - rand() % 50), float(25 - rand() % 50)));
	}
	
	testCubeModel = glm::translate(glm::mat4(), glm::vec3(0.0f, 3.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(sceneData.fov), WIDTH/HEIGHT, 0.1f, 100.0f);	
	
	Level level("resources/test.pbm");
	sceneData.avatar.setLevel(&level);
	
	Object impact(0.3f, 0.3f, 0.3f, 1.0f, 0.0f, 0.0f);
	
	float lightSourceAngle = 0.0f;
	
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		
		if (deltaTime > 1.0 / max_framerate)
		{
		lastFrame = curFrame;
		keyboard_input(window, deltaTime);
		
		if (print_framerate)
			cout << 1.0 / deltaTime << endl;
				
		/* compute bullets and enemies collisions */
		list<vector<Enemy>::iterator> killed;
		for(auto enemy_it = enemies.begin(); enemy_it != enemies.end(); ++enemy_it)
		{
			for (auto bullet_it = sceneData.avatar.bullets_begin(); bullet_it != sceneData.avatar.bullets_end(); ++bullet_it)
			{
				if (enemy_it->collide(bullet_it->pos()))
				{
					killed.push_back(enemy_it);
				}
			}
		}
		for (auto enemy_it = killed.begin(); enemy_it != killed.end(); ++enemy_it)
		{
			enemies.erase(*enemy_it);
		}

        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/* Colored Vertex shader */
		/*-----------------------*/
		vertColorShader.use();
		glUniformMatrix4fv(glGetUniformLocation(vertColorShader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(glGetUniformLocation(vertColorShader.id(), "view"), 1, GL_FALSE, glm::value_ptr(sceneData.avatar.viewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(vertColorShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
		
		originObject.draw(GL_LINES);
		grid.draw(vertColorShader);
		
		/* lightSource shader */
		/*--------------------*/
		lightSourceShader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.id(), "view"), 1, GL_FALSE, glm::value_ptr(sceneData.avatar.viewMatrix()));
		
		lightSourceAngle += deltaTime * 1.0f;
		if (lightSourceAngle > 2*M_PI)
			lightSourceAngle = 0.0f;
		float lightX = cos(lightSourceAngle) * 5.0f;
		float lightY = 5.0f;
		float lightZ = sin(lightSourceAngle) * 5.0f;
		model = glm::translate(glm::mat4(), glm::vec3(lightX, lightY, lightZ));
		glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightSource.draw(lightSourceShader);
		
		/* lighting shader */
		/*-----------------*/
		lightingShader.use();
		glUniform3f(glGetUniformLocation(lightingShader.id(), "lightPos"), lightX, lightY, lightZ);
		glUniform3f(glGetUniformLocation(lightingShader.id(), "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));	
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.id(), "view"), 1, GL_FALSE, glm::value_ptr(sceneData.avatar.viewMatrix()));
		
		level.draw(lightingShader);
		
		sceneData.avatar.draw(lightingShader, deltaTime);
		
		testCubeModel = glm::rotate(testCubeModel, glm::radians(deltaTime * 10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		testCubeModel = glm::rotate(testCubeModel, glm::radians(deltaTime * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(testCubeModel));
		cube.draw(lightingShader);
		
		for(auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			it->draw(lightingShader);
		}
		
		/* sight -> wall intersection */
		glm::vec3 start = sceneData.avatar.position();
		glm::vec3 end = sceneData.avatar.position() + sceneData.avatar.direction() * 1000.0f;
		glm::vec3 impactPos = level.intersectWall(start, end);
		cout << impactPos.x << " " << impactPos.z << endl;
		
		glm::mat4 impactModel = glm::translate(glm::mat4(), impactPos);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.id(), "model"), 1, GL_FALSE, glm::value_ptr(impactModel));
		impact.draw(lightingShader);
		
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
		}
    }

    glfwTerminate();
    return 0;
}