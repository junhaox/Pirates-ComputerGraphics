//
//  BoundingBox.cpp
//  Final
//
//  Created by Junhao Xie on 6/5/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#include "BoundingBox.h"


BoundingBox::BoundingBox(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, GLfloat minZ, GLfloat maxZ, OBJObject *obj)
{
    this->object = obj;
    this->toWorld = glm::mat4(1.0f);
    this->min.x = minX;
    this->max.x = maxX;
    this->min.y = minY;
    this->max.y = maxY;
    this->min.z = minZ;
    this->max.z = maxZ;
}


BoundingBox::BoundingBox()
{
    
}


BoundingBox::~BoundingBox()
{
    
}


bool BoundingBox::checkCollision(BoundingBox *bb)
{
    // Check overlay in x-axis
    bool collisionX = (this->max.x >= bb->min.x) && (bb->max.x >= this->min.x);
    
    // Check overlay in y-axis
    bool collisionY = (this->max.y >= bb->min.y) && (bb->max.y >= this->min.y);
    
    // Check overlay in z-axis
    bool collisionZ = (this->max.z >= bb->min.z) && (bb->max.z >= this->min.z);
    
    return collisionX && collisionY && collisionZ;
}


void BoundingBox::doCollision()
{
    
}


void BoundingBox::draw(GLuint shaderProgram)
{
    GLfloat centerX = (this->min.x + this->max.x) / 2;
    GLfloat centerY = (this->min.y + this->max.y) / 2;
    GLfloat centerZ = (this->min.z + this->max.z) / 2;
}


void BoundingBox::update(glm::mat4 trans)
{
    this->min = glm::vec3(trans * glm::vec4(this->min, 1.0f));
    this->max = glm::vec3(trans * glm::vec4(this->max, 1.0f));
}