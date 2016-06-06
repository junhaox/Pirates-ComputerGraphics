//
//  ProceduralTrees.h
//  Final
//
//  Created by Junhao Xie on 6/5/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#ifndef ProceduralTrees_h
#define ProceduralTrees_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Window.h"

using namespace std;

class ProceduralTrees
{
public:
    const float PI = 3.14, DEPTH = 6;
    const int startI = -500, endI = 10;
    float angle = 20, depth = 0;
    double lastTime = 0, elapsedTime = 0, lastElapsedTime = 0;
    bool cam = false;
    float lineWidth = 5, length = 0.001, incr = 0.1, num = 0;
    
    vector<string> *trees = new vector<string>();
    string str = "X";
    
    ProceduralTrees();
    ~ProceduralTrees();
    
    void push();
    void pop();
    void rotLeft();
    void rotRight();
    void leaf();
    void draw();
    void drawLine();
    void expand(float num);
    void display();
    void animate();
};

#endif /* ProceduralTrees_h */
