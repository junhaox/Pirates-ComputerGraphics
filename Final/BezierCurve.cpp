#include "BezierCurve.h"

const float width = 4.0f;

BezierCurve::BezierCurve( ControlPoint *cp[], int num_cp)
{
	this->toWorld = glm::mat4(1.0f);
	this->angle = 0.0f;
    counter = 0;
    timer = 0.0f;
    
    cp[0] = new ControlPoint(-1*30,  2,  -3*30, 232);
    cp[1] = new ControlPoint(-4.0f*30,  2,  -2.5f*30, 233);
    cp[2] = new ControlPoint(-5.0f*30, 2, -3.0f*30, 234);
    
    cp[3] = new ControlPoint(-5.0f*30, 2, -5.0f*30, 235);
    glm::vec3 handle = oppositeHandle(cp[3], cp[2]);
    cp[4] = new ControlPoint(handle.x, handle.y, handle.z, 236);
    
    cp[5] = new ControlPoint(-9*30, 2, -2*30, 237);
    cp[6] = new ControlPoint(-10.5f*30,  2,  1.5f*30, 238);
    handle = oppositeHandle(cp[6], cp[5]);
    cp[7] = new ControlPoint( handle.x, handle.y, handle.z, 239);
    
    cp[8] = new ControlPoint(-10.0f*30,  2,  3.1f*30,  240);
    cp[9] = new ControlPoint(-11.0f*30,  2,  4.5f*30,  241 );
    handle = oppositeHandle(cp[9], cp[8]);
    cp[10] = new ControlPoint( handle.x, handle.y, handle.z,  242 );
    
    cp[11] = new ControlPoint( -3.5f*30,  2,  5.5f*30, 243 );
    cp[12] = new ControlPoint( -3.0f*30, 2.0f,  6.5f*30, 244 );
    handle = oppositeHandle(cp[12], cp[11]);
    cp[13] = new ControlPoint( handle.x, handle.y, handle.z, 245 );
    
    cp[14] = new ControlPoint(6.0f*30,  2,  4.4f*30, 246);
    cp[15] = new ControlPoint(8.0f*30, 2, 3.6f*30, 247);
    handle = oppositeHandle(cp[15], cp[14]);
    cp[16] = new ControlPoint( handle.x, handle.y, handle.z,  248 );
    
    cp[17] = new ControlPoint(9.5f*30,  2,  1.0f*30,  249);
    cp[18] = new ControlPoint(10.6f*30,  2, 0.0f*30,  250);
    handle = oppositeHandle(cp[18], cp[17]);
    cp[19] = new ControlPoint( handle.x, handle.y, handle.z, 251 );
    
    cp[20] = new ControlPoint(10.0f*30,  2,  -2.0f*30,  252);
    cp[21] = new ControlPoint(8.5f*30,  2, -3.7f*30,  253);
    handle = oppositeHandle(cp[21], cp[20]);
    cp[22] = new ControlPoint( handle.x, handle.y, handle.z, 254);
    
    handle = oppositeHandle(cp[0], cp[1]);
    cp[23] = new ControlPoint( handle.x, handle.y, handle.z, 255 );
    
    interpolate(cp, num_cp);
    
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, curveDots.size()* sizeof(glm::vec3), &curveDots[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // the call to glVertexAttribPointer registered VBO as the currently bound vertex
    // buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // Unbind VAO
}

BezierCurve::~BezierCurve()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

glm::vec3 BezierCurve::getTallest(){ return tallestPoint; }
glm::vec3 BezierCurve::getCurrent(){ return currentPoint; }
int BezierCurve::getCounter(){ return counter; }
float BezierCurve::getTime(){ return timer; }

glm::vec3 BezierCurve::oppositeHandle( ControlPoint *cp1, ControlPoint *cp2 ) {
    
    glm::vec3 opposite = -(cp2->position - cp1->position);
    opposite += cp1->position;
    
    return opposite;
}

void BezierCurve::interpolate ( ControlPoint *cp[], int num_cp ){

    curveDots.clear();

    tallestPoint = cp[21]->position;
    currentPoint = tallestPoint;
    
    for (int i = 0; i < 22; i += 3) {
        
        glm::vec3 p0 = glm::vec3 ( cp[i]->position );
        glm::vec3 p1 = glm::vec3 ( cp[i+1]->position );
        glm::vec3 p2 = glm::vec3 ( cp[i+2]->position );
        glm::vec3 p3;
        
        if( (i+3) >= 24 )
            p3 = glm::vec3 ( cp[0]->position );
        else
            p3 = glm::vec3 ( cp[i+3]->position );

        // interpolate 100 points
        for (float t = 0;  t < 1; t += 0.01) {
            
            float c0 = (1-t) * (1-t) * (1-t);
            float c1 = 3 * t * (1-t) * (1-t);
            float c2 = 3 * t * t * (1-t);
            float c3 = t * t * t;
            
            glm::vec3 dot = c0 * p0 + c1 * p1 + c2 * p2 + c3 * p3;

            curveDots.push_back(dot);
            currentPoint = dot;

            if( dot.y > tallestPoint.y ) {
                tallestPoint = dot;
                counter = i;
                timer = t;
            }
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, curveDots.size()* sizeof(glm::vec3), &curveDots[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BezierCurve::draw(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
    
    // Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
    glm::mat4 MVP = Window::P * Window::V * toWorld;
    glm::mat4 model = toWorld;
    glm::mat4 camera = Window::V;
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint CamID = glGetUniformLocation(shaderProgram, "camera");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "model");
    
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(CamID, 1, GL_FALSE, &camera[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);
    
    glBindVertexArray(VAO);
    glLineWidth(width);
    glDrawArrays(GL_LINE_LOOP, 0, (GLint)curveDots.size()) ;
    
    glBindVertexArray(0);
}


void BezierCurve::moveAll( ControlPoint *cp[], int i, glm::vec3 x_axis, float offsetX, glm::vec3 y_axis, float offsetY ){
    
    int oppositeIndex = -1;
    int centerIndex = -1;
    
    switch (i) {
        case 1:
            oppositeIndex = 23;
            centerIndex = 0;
            break;
        case 23:
            oppositeIndex = 1;
            centerIndex = 0;
            break;
        case 2:
            oppositeIndex = 4;
            centerIndex = 3;
            break;
        case 4:
            oppositeIndex = 2;
            centerIndex = 3;
            break;
        case 5:
            oppositeIndex = 7;
            centerIndex = 6;
            break;
        case 7:
            oppositeIndex = 5;
            centerIndex = 6;
            break;
        case 8:
            oppositeIndex = 10;
            centerIndex = 9;
            break;
        case 10:
            oppositeIndex = 8;
            centerIndex = 9;
            break;
        case 11:
            oppositeIndex = 13;
            centerIndex = 12;
            break;
        case 13:
            oppositeIndex = 11;
            centerIndex = 12;
            break;
        case 14:
            oppositeIndex = 16;
            centerIndex = 15;
            break;
        case 16:
            oppositeIndex = 14;
            centerIndex = 15;
            break;
        case 17:
            oppositeIndex = 19;
            centerIndex = 18;
            break;
        case 19:
            oppositeIndex = 17;
            centerIndex = 18;
            break;
        case 20:
            oppositeIndex = 22;
            centerIndex = 21;
            break;
        case 22:
            oppositeIndex = 20;
            centerIndex = 21;
            break;
        default:
            break;
    }
    // change the position of the selected handle point
    cp[i]->position = cp[i]->position + ( x_axis * offsetX / 40.0f ) + ( y_axis * offsetY / 40.0f );

    // change the selected handle point's opposite
    if( (centerIndex != -1) && (oppositeIndex != -1) ) {
        
        glm::vec3 handle = oppositeHandle ( cp[centerIndex], cp[i] );
        cp[oppositeIndex]->position.x = handle.x;
        cp[oppositeIndex]->position.y = handle.y;
        cp[oppositeIndex]->position.z = handle.z;
        
    } else if ( i >= 0 && i <= 32 ) {

        // the selected is middle point, move the whole handle with it.
        if (i == 0)
            cp[23]->position = cp[23]->position + ( x_axis * offsetX / 40.0f ) + ( y_axis * offsetY / 40.0f );
        else
            cp[i-1]->position = cp[i-1]->position + ( x_axis * offsetX / 40.0f ) + ( y_axis * offsetY / 40.0f );
        
        cp[i+1]->position = cp[i+1]->position + ( x_axis * offsetX / 40.0f ) + ( y_axis * offsetY / 40.0f );
    }
}

void BezierCurve::scale(float i){
    
    glm::mat4 matrix = glm::mat4(1.0f);
    this->toWorld = this->toWorld * glm::scale(matrix, glm::vec3(i, i, i)) ;
}
