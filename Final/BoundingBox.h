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

class BoundingBox
{
public:
    BoundingBox(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, GLfloat minZ, GLfloat maxZ, OBJObject *obj);
    BoundingBox();
    ~BoundingBox();
    
    glm::mat4 toWorld;
    glm::vec3 min, max;
    OBJObject *object;
    bool collided;
    
    bool checkCollision(BoundingBox *bb);
    void doCollision();
    void draw(GLuint shaderProgram);
    void update(glm::mat4 matrix);
};

#endif /* BoundingBox_h */
