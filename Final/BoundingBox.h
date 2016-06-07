//
//  BoundingBox.h
//  Final
//
//  Created by Junhao Xie on 6/5/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#ifndef BoundingBox_h
#define BoundingBox_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "OBJObject.h"
#include "Cylinder.h"
#include "Window.h"

class BoundingBox
{
public:
    //BoundingBox(OBJObject *obj);
    BoundingBox(Cylinder *obj);
    BoundingBox();
    ~BoundingBox();
    
    GLuint VAO,VBO,EBO;
    glm::mat4 toWorld;
    glm::vec3 min, max;
    glm::vec3 size;
    glm::vec3 center;
    glm::mat4 transform;
    Cylinder *object;
    int collided;
    
    bool checkCollision(BoundingBox *bb);
    void doCollision();
    void draw(GLuint shaderProgram);
    void update();
};

#endif /* BoundingBox_h */
