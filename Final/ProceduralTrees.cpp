//
//  ProceduralTrees.cpp
//  Final
//
//  Created by Junhao Xie on 6/5/16.
//  Copyright © 2016 Junhao Xie. All rights reserved.
//

#include "ProceduralTrees.h"

ProceduralTrees::ProceduralTrees()
{
    
}

ProceduralTrees::~ProceduralTrees()
{
    
}

void ProceduralTrees::push()
{
    glPushMatrix();
    if (lineWidth > 0)
        lineWidth -= 1;
}

void ProceduralTrees::pop()
{
    glPopMatrix();
    lineWidth += 1;
}

void ProceduralTrees::rotLeft()
{
    glRotatef(angle, 1, 0, 0);
    glRotatef(angle*4, 0, 1, 0);
    glRotatef(angle, 0, 0, 1);
}

void ProceduralTrees::rotRight()
{
    glRotatef(-angle, 1, 0, 0);
    glRotatef(angle*4, 0, 1, 0);
    glRotatef(-angle, 0, 0, 1);
}

void ProceduralTrees::leaf()
{
    glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff
    
    //glColor3f(0.50, 1.0, 0.0);
    GLfloat ambient[4] = { 0.50, 1.0, 0.0 };    // ambient reflection
    GLfloat specular[4] = { 0.55, 1.0, 0.0 };   // specular reflection
    GLfloat diffuse[4] = { 0.50, 0.9, 0.0 };   // diffuse reflectio
    
    // set the ambient reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    // set the diffuse reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    // set the specular reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    // set the size of the specular highlights
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
    
    //glutSolidCube(depth+1);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, 0, 0);
    glVertex3f(0.2, 0, 0.3);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(-0.2, 0, -0.3);
    glVertex3f(0, 1, 0);
    glEnd();
    glPopAttrib();
}

void ProceduralTrees::draw()
{
    string ch = "";
    string LSystem = trees->at(depth);
    for (int i = 0; i < LSystem.length(); i++){
        ch = LSystem.at(i);
        
        if (ch.compare("D") == 0 || ch.compare("X") == 0){
            drawLine();
        } else if (ch.compare("[") == 0){
            push();
        } else if (ch.compare("]") == 0){
            pop();
        } else if (ch.compare("V") == 0){
            leaf();
        } else if (ch.compare("R") == 0){
            rotRight();
        } else if (ch.compare("L") == 0){
            rotLeft();
        }
    }
}

void ProceduralTrees::drawLine()
{
    glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff
    
    
    //glColor3f(0.55, 0.27, 0.07);
    GLfloat ambient[4] = {0.55, 0.27, 0.07};    // ambient reflection
    GLfloat specular[4] = {0.55, 0.27, 0.07};   // specular reflection
    GLfloat diffuse[4] = {0.55, 0.27, 0.07};   // diffuse reflection
    
    
    // set the ambient reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    // set the diffuse reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    // set the specular reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glLineWidth(lineWidth);
    
    glBegin(GL_LINES);
    
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    glEnd();
    
    glTranslatef(0, length, 0);
    glPopAttrib();
}


void ProceduralTrees::expand(float num)
{
    string ch = "";
    
    for (int i = 0; i < str.length(); i++){
        ch = str.at(i);
        
        if (ch.compare("D") == 0){
            str.replace(i, 1, "DD");
            i = i + 1;
            
        } else if (ch.compare("X") == 0){
            
            if (num < 0.4){
                //LSystem.replace(i, 1, "D[LX]D[RX]LX");
                str.replace(i, 1, "D[LXV]D[RXV]LX");
                
            } else {
                //LSystem.replace(i, 1, "D[RX]D[LX]RX");
                str.replace(i, 1, "D[RXV]D[LXV]RX");
            }
            
            i = i + 13;	//11
        } 
        
    }
    
    trees->push_back(str);
}

void ProceduralTrees::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (Window::fov, 1.0, 1, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z, Window::cam_look_at.x, Window::cam_look_at.y, Window::cam_look_at.z, 0, 1, 0);
    
    
    
    glPushMatrix();
    
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glPushAttrib(GL_LIGHTING_BIT); //saves current lighting stuff
    GLfloat ambient[4] = { 0.82, 0.41, 0.12 };    // ambient reflection
    GLfloat diffuse[4] = { 0.82, 0.41, 0.12};   // diffuse reflection
    // set the ambient reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    // set the diffuse reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-20,0,-20);
    glVertex3f(20,0,-20);
    glVertex3f(20,0,20);
    glVertex3f(-20,0,20);
    glVertex3f(-20,0,-20);
    glVertex3f(20,0,20);
    
    glEnd();
    glPopMatrix();
    glPopAttrib();
    
    draw();
    glutSwapBuffers();
    glutPostRedisplay();
}

void ProceduralTrees::animate()
{
    time_t sc;
    
    if (lastTime == 0)
        lastTime = time(&sc)*1000;
    
    elapsedTime = time(&sc)*1000-lastTime;
    
    // Change the angle to make it blow in the wind
    float numR = (float) rand()/RAND_MAX;
    
    
    if (angle > 21.5){
        if (numR < 0.5){
            incr = -0.15;
        } else {
            incr = -0.1;
        }
    } else if (angle < 18.5){
        if (numR > 0.5){
            incr = 0.15;
        } else {
            incr = 0.1;
        }
    }
    angle += incr;
    
    if (depth < DEPTH)
        length += 0.001;
    
    if(elapsedTime-lastElapsedTime > 2000 && depth < DEPTH){
        depth++;
        lastElapsedTime = elapsedTime;
    }
    
    elapsedTime = elapsedTime/5000;
    float t = (sin((elapsedTime*PI-PI/2))+1)/2;
    float p = (1-t)*startI + t*endI;
    
    if (cam)
        Window::cam_pos.x = p;
    glutPostRedisplay();
}

