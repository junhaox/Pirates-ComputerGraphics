#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class OBJObject
{
//private:
public:
    
    std::vector<unsigned int> indexVert, indexUv, indexNorm;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvec;
    std::vector<glm::vec3> normals;
    
    glm::mat4 toWorld;
    GLuint VBO, VAO, EBO, NBO;
    
	OBJObject(const char* filepath);

	float angle;

	void parse(const char* );
	void draw(GLuint);
    void draw(GLuint, glm::mat4);
    bool loadMaterial(std::string sFullMtlFileName);
    void update();
    void moveTo( glm::vec3 here );
	void spin(float);
    void translate(float, float, float);
    void scale(float);
    void orbit(float);
    void reset();
};

#endif
