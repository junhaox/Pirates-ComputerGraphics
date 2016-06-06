#ifndef _BARS_H_
#define _BARS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include "Window.h"
#include "ControlPoint.h"

using namespace std;

class Bars
{
public:
    Bars( ControlPoint *cp[], int num_cp );
	~Bars();

    std::vector<glm::vec3> barPoints;
    glm::mat4 toWorld;
	GLuint VBO, VAO;
    float angle;

	void draw(GLuint);
	void update(ControlPoint *cp[], int num_cp);
    void scale(float);
};

#endif

