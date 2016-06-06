//
//  BoundingBox.cpp
//  Final
//
//  Created by Junhao Xie on 6/5/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#include "BoundingBox.h"

GLfloat vertices2[] = {
    -0.5, -0.5, -0.5, 1.0,
    0.5, -0.5, -0.5, 1.0,
    0.5,  0.5, -0.5, 1.0,
    -0.5,  0.5, -0.5, 1.0,
    -0.5, -0.5,  0.5, 1.0,
    0.5, -0.5,  0.5, 1.0,
    0.5,  0.5,  0.5, 1.0,
    -0.5,  0.5,  0.5, 1.0,
};

GLushort elements[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
};

BoundingBox::BoundingBox(OBJObject *obj)
{
    this->object = obj;
    this->toWorld = glm::mat4(1.0f);
    this->collided = 0;
    
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    this->min.x = this->object->vertices[0].x;
    this->max.x = this->object->vertices[0].x;
    this->min.y = this->object->vertices[0].y;
    this->max.y = this->object->vertices[0].y;
    this->min.z = this->object->vertices[0].z;
    this->max.z = this->object->vertices[0].z;
    
    for (int i = 0; i < this->object->vertices.size(); i++) {
        if (this->object->vertices[i].x < this->min.x)
            this->min.x = this->object->vertices[i].x;
        if (this->object->vertices[i].x > this->max.x)
            this->max.x = this->object->vertices[i].x;
        
        if (this->object->vertices[i].y < this->min.y)
            this->min.y = this->object->vertices[i].y;
        if (this->object->vertices[i].y > this->max.y)
            this->max.y = this->object->vertices[i].y;
        
        if (this->object->vertices[i].z < this->min.z)
            this->min.z = this->object->vertices[i].z;
        if (this->object->vertices[i].z > this->max.z)
            this->max.z = this->object->vertices[i].z;
    }
    
    this->size = glm::vec3(max.x-min.x, max.y-min.y, max.z-min.z);
    this->center = glm::vec3((min.x+max.x)/2, (min.y+max.y)/2, (min.z+max.z)/2);
    this->transform = glm::translate(glm::mat4(1), this->center) * glm::scale(glm::mat4(1), this->size);
    
}


BoundingBox::BoundingBox()
{
    
}


BoundingBox::~BoundingBox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    this->toWorld = this->object->toWorld * transform;
 
    glm::mat4 MVP = Window::P * Window::V * this->toWorld;
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void BoundingBox::update()
{
    /*glm::vec3 temp = glm::vec3(this->object->toWorld * glm::vec4(this->object->vertices[0], 1.0f));

    this->min.x = temp.x;
    this->max.x = temp.x;
    this->min.y = temp.y;
    this->max.y = temp.y;
    this->min.z = temp.z;
    this->max.z = temp.z;
    
    for (int i = 0; i < this->object->vertices.size(); i++) {
        temp = glm::vec3(this->object->toWorld * glm::vec4(this->object->vertices[i], 1.0f));
        if (temp.x < this->min.x)
            this->min.x = temp.x;
        if (temp.x > this->max.x)
            this->max.x = temp.x;
        
        if (temp.y < this->min.y)
            this->min.y = temp.y;
        if (temp.y > this->max.y)
            this->max.y = temp.y;
        
        if (temp.z < this->min.z)
            this->min.z = temp.z;
        if (temp.z > this->max.z)
            this->max.z = temp.z;
    }*/
    
    this->min = glm::vec3(this->toWorld * glm::vec4(this->min, 0.0f));
    this->max = glm::vec3(this->toWorld * glm::vec4(this->max, 0.0f));
}