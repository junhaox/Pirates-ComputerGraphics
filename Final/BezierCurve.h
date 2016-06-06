#ifndef _BEZIERCURVE_H_
#define _BEZIERCURVE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>
#include "Window.h"
#include "ControlPoint.h"

using namespace std;

class BezierCurve
{
public:
    BezierCurve( ControlPoint *cp[], int num_cp );
	~BezierCurve();

    std::vector<glm::vec3> curveDots;
    glm::mat4 toWorld;
	GLuint VBO, VAO;
    float angle;
    int counter;
    float timer;
    glm::vec3 tallestPoint;
    glm::vec3 currentPoint;

    glm::vec3 getTallest();
    glm::vec3 getCurrent();
    int getCounter();
    float getTime();
    glm::vec3 oppositeHandle( ControlPoint *cp1, ControlPoint *cp2);
    void interpolate ( ControlPoint *cp[], int num_cp );
    void moveAll(ControlPoint *cp[],int i, glm::vec3 x_axis, float offsetX, glm::vec3 y_axis, float offsetY);
	void draw(GLuint);
    void scale(float);
};

#endif

