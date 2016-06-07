//
//  Cylinder.h
//  Final
//
//  Created by Junhao Xie on 6/7/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#ifndef Cylinder_h
#define Cylinder_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <stdio.h>
#include "Geode.h"

class Cylinder: public Geode
{
public:
    Cylinder(OBJObject *cylinder);
    ~Cylinder();
};
#endif /* Cylinder_h */
