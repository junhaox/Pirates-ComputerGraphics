//
//  BoatGenerator.h
//  PA3
//
//  Created by Pierre Karpov on 6/6/16.
//  Copyright © 2016 Joe Lu Wu. All rights reserved.
//

#ifndef BoatGenerator_h
#define BoatGenerator_h

#include <stdio.h>
#include <vector>
#include "MatrixTransform.cpp"
#include "Cylinder.h"
#include "OBJObject.h"


class BoatGenerator {
public:
    BoatGenerator();
    void update();
    void draw();
    glm::mat4 getMatrix(glm::vec3 m_pos,
                        glm::vec3 m_scale,
                        float m_angle,
                        int axis);
    
    MatrixTransform *world;
    std::vector<Cylinder*> c_vec;
    
};

#endif /* BoatGenerator_h */
