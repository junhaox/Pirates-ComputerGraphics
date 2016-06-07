#ifndef _PARALLAX_H_
#define _PARALLAX_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

class parallax
{
public:
    GLuint diffuseMap;
    GLuint normalMap;
    GLuint heightMap;
    
	parallax(GLuint);
	~parallax();

	glm::mat4 toWorld;
	GLuint VBO, VAO, EBO;
	float angle;

    void RenderQuad();
    void draw(GLuint shaderProgram, GLfloat height_scale, GLboolean parallax_mapping, glm::vec3 cam_pos);
    void translate(float x, float y, float z);
	void update();
    void scale(float);
	void spin(float);
    
    GLuint loadTexture(GLchar* path);
};

#endif

