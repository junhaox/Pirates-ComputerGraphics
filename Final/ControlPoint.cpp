#include "ControlPoint.h"
#include "Window.h"

const float POINT_SIZE = 20.0f;

ControlPoint::ControlPoint(float x, float y, float z, int ID)
{
	this->toWorld = glm::mat4(1.0f);
	this->angle = 0.0f;
    this->ID = ID;
    this->position = glm::vec3(x, y, z);
    
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &position, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // the call to glVertexAttribPointer registered VBO as the currently bound vertex
    // buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // Unbind VAO
    
}

ControlPoint::~ControlPoint()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ControlPoint::update(){
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &position, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ControlPoint::draw(GLuint shaderProgram)
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
    
    glPointSize ( POINT_SIZE );
    glDrawArrays(GL_POINTS, 0, (GLfloat) 1) ;
    
    glBindVertexArray(0);
}

void ControlPoint::selectionDraw(GLuint shaderProgram)
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

    glPointSize( POINT_SIZE);
    GLuint idLocation = glGetUniformLocation(shaderProgram, "id");
    glUniform1ui(idLocation, this->ID);
    
    glDrawArrays(GL_POINTS, 0, (GLfloat) 1) ;

    glBindVertexArray(0);
}

void ControlPoint::scale(float i){
    
    glm::mat4 matrix = glm::mat4(1.0f);
    this->toWorld = this->toWorld * glm::scale(matrix, glm::vec3(i, i, i)) ;
}

void ControlPoint::translate(float x, float y, float z){
//    
//    glm::mat4 matrix = glm::mat4(1.0f);
//    this->toWorld = glm::translate(matrix, glm::vec3(x, y, z)) * this->toWorld;
    position.x += x;
    position.y += y;
    position.z += z;
}


