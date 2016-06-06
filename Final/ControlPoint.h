#ifndef _CONTTROLPOINT_H_
#define _CONTTROLPOINT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>

using namespace std;

class ControlPoint
{
public:
    ControlPoint(float, float, float, int ID);
	~ControlPoint();

    int ID;
    float angle;
	glm::mat4 toWorld;
    glm::vec3 position;
	GLuint VBO, VAO;

	void draw(GLuint);
    void selectionDraw(GLuint);
	void update();
    void scale(float);
    void translate(float x, float y, float z);
};

#endif

