//
//  BoatGenerator.cpp
//  PA3
//
//  Created by Pierre Karpov on 6/6/16.
//  Copyright © 2016 Joe Lu Wu. All rights reserved.
//

#include "BoatGenerator.h"

#define HULL_SLICE_WIDTH 5.0
#define HULL_SLICE_HEIGHT 3.0
#define HULL_SLICE_LENGTH 2.0
#define HULL_SLICE_FACTOR 5.0
#define CANNON_FACTOR 2.0
#define CANNON_WIDTH 1.43
#define CANNON_HEIGHT 1.92
#define CANNON_LENGTH 3.70
#define SAIL_SCOONER_FACTOR 4.0
#define SAIL_SCOONER_BACK_LENGTH 5.29
#define SAIL_SCOONER_OFFSET 1.0



BoatGenerator::BoatGenerator() {
    int numOfSlices = 30;
    
    OBJObject *hullSlice;
    OBJObject *hullFront;
    OBJObject *cannon;
    OBJObject *frontSail;
    OBJObject *midSail;
    
    MatrixTransform *mt_hull_front;
    std::vector<MatrixTransform*> mt_hull_slice_vec;
    std::vector< MatrixTransform*> mt_cannon_vec;
    MatrixTransform *mt_sail_front;
    std::vector< MatrixTransform*> mt_sail_vec;
    
    std::vector<Cylinder*> c_hull_side_vec;
    std::vector<Cylinder*> c_cannon_vec;
    std::vector<Cylinder*> c_sail_vec;
    
    world = new MatrixTransform();
    
    //front hull
    hullFront = new OBJObject("HullFrontWooden.obj");
    Cylinder *c_hullFront = new Cylinder( hullFront );
    mt_hull_front = new MatrixTransform( getMatrix(glm::vec3(0, 0, 0), glm::vec3(HULL_SLICE_FACTOR, HULL_SLICE_FACTOR, -HULL_SLICE_FACTOR), 0, 0));
    mt_hull_front->addChild(c_hullFront);
    world->addChild(mt_hull_front);
    
    c_vec.push_back(c_hullFront);
    
    //midsection of hull
    hullSlice = new OBJObject("HullSliceWooden.obj");
    for(int i = 0; i < numOfSlices; i++) {
        Cylinder *c_hull_slide = new Cylinder( hullSlice );
        MatrixTransform *mt_hull = new MatrixTransform( getMatrix(glm::vec3(0, 0, -HULL_SLICE_FACTOR * HULL_SLICE_LENGTH * i),
                                                                  glm::vec3(HULL_SLICE_FACTOR, HULL_SLICE_FACTOR, HULL_SLICE_FACTOR),
                                                                  0,
                                                                  0));
        mt_hull->addChild(c_hull_slide);
        mt_hull_slice_vec.push_back(mt_hull);
        c_hull_side_vec.push_back(c_hull_slide);
        world->addChild(mt_hull);
        
        c_vec.push_back(c_hull_slide);
    }
    
    //cannons
    cannon = new OBJObject("cannon.obj");
    for(int i = 0; i < numOfSlices; i++) {
        Cylinder *c_cannon_1 = new Cylinder( cannon );
        Cylinder *c_cannon_2 = new Cylinder( cannon );
        MatrixTransform *mt_cannon_1 = new MatrixTransform( getMatrix(glm::vec3(CANNON_FACTOR * CANNON_LENGTH,
                                                                                HULL_SLICE_FACTOR * HULL_SLICE_HEIGHT,
                                                                                -CANNON_FACTOR * CANNON_WIDTH -  i * HULL_SLICE_FACTOR * HULL_SLICE_LENGTH),
                                                                      glm::vec3(CANNON_FACTOR, CANNON_FACTOR, CANNON_FACTOR),
                                                                      180,
                                                                      2));
        MatrixTransform *mt_cannon_2 = new MatrixTransform( getMatrix(glm::vec3(HULL_SLICE_FACTOR * HULL_SLICE_WIDTH - CANNON_FACTOR * CANNON_LENGTH,
                                                                                HULL_SLICE_FACTOR * HULL_SLICE_HEIGHT,
                                                                                - i * HULL_SLICE_FACTOR * HULL_SLICE_LENGTH),
                                                                      glm::vec3(CANNON_FACTOR, CANNON_FACTOR, CANNON_FACTOR),
                                                                      0,
                                                                      2));
        mt_cannon_1->addChild(c_cannon_1);
        mt_cannon_vec.push_back(mt_cannon_1);
        c_cannon_vec.push_back(c_cannon_1);
        world->addChild(mt_cannon_1);
        
        mt_cannon_2->addChild(c_cannon_2);
        mt_cannon_vec.push_back(mt_cannon_2);
        c_cannon_vec.push_back(c_cannon_2);
        world->addChild(mt_cannon_2);
        
        c_vec.push_back(c_cannon_1);
        c_vec.push_back(c_cannon_2);
    }
    
    //front sail
    frontSail = new OBJObject("FrontSailScooner.obj");
    Cylinder *c_sail_front = new Cylinder( frontSail );
    mt_sail_front = new MatrixTransform( getMatrix(glm::vec3(HULL_SLICE_FACTOR * HULL_SLICE_WIDTH / 2, HULL_SLICE_FACTOR * HULL_SLICE_HEIGHT, 0),
                                                   glm::vec3(SAIL_SCOONER_FACTOR, SAIL_SCOONER_FACTOR, SAIL_SCOONER_FACTOR),
                                                   0,
                                                   0));
    mt_sail_front->addChild(c_sail_front);
    world->addChild(mt_sail_front);
    
    c_vec.push_back(c_sail_front);
    
    //mid sails
    float free_space_for_sails = numOfSlices * HULL_SLICE_FACTOR * HULL_SLICE_LENGTH
    - (SAIL_SCOONER_FACTOR * SAIL_SCOONER_BACK_LENGTH);
    int num_sail_possible = free_space_for_sails / ((SAIL_SCOONER_BACK_LENGTH + SAIL_SCOONER_OFFSET) * SAIL_SCOONER_FACTOR);
    
    midSail = new OBJObject("MidSailScooner.obj");
    for(int i = 0; i < num_sail_possible; i++) {
        Cylinder *c_sail = new Cylinder( midSail );
        MatrixTransform *mt_sail = new MatrixTransform( getMatrix(glm::vec3(HULL_SLICE_FACTOR * HULL_SLICE_WIDTH / 2,
                                                                            HULL_SLICE_FACTOR * HULL_SLICE_HEIGHT,
                                                                            -(SAIL_SCOONER_BACK_LENGTH + SAIL_SCOONER_OFFSET) * SAIL_SCOONER_FACTOR * (i+1)),
                                                                  glm::vec3(SAIL_SCOONER_FACTOR, SAIL_SCOONER_FACTOR, SAIL_SCOONER_FACTOR),
                                                                  0,
                                                                  0));
        mt_sail->addChild(c_sail);
        mt_sail_vec.push_back(mt_sail);
        c_sail_vec.push_back(c_sail);
        world->addChild(mt_sail);
        
        c_vec.push_back(c_sail);
    }
}

void BoatGenerator::draw() {
    // something like that
    // world->draw(shaderProgram);
    // or you can in window call bg->world->draw(shaderProgram);
    //the latter is better imo
}

void BoatGenerator::update() {
    // do any extra movement
    // like cannons rotating, etc.
    // overall movent will most likely be done in window
    
}

// m_pos = Translate vector, m_scale = resize vectore, m_angle = angle to rotate, axis type 1=x, 2=y, 3=z
glm::mat4 BoatGenerator::getMatrix(glm::vec3 m_pos, glm::vec3 m_scale, float m_angle, int axis)
{
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_pos);
    glm::mat4 scale = glm::scale(glm::mat4(1.0), m_scale);
    glm::mat4 rotate = glm::mat4(1.0f);
    
    if (axis == 1)
        rotate = glm::rotate(glm::mat4(1.0f), -m_angle/ 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    else if (axis == 2)
        rotate = glm::rotate(glm::mat4(1.0f), -m_angle/ 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    else if (axis == 3)
        rotate = glm::rotate(glm::mat4(1.0f), -m_angle/ 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    
    return translate * scale * rotate;
}