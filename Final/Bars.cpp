#include "Bars.h"

Bars::Bars( ControlPoint *cp[], int num_cp)
{
	this->toWorld = glm::mat4(1.0f);
	this->angle = 0.0f;
    
    barPoints.clear();
    
    barPoints.push_back( cp[23]->position );
    barPoints.push_back( cp[0]->position );
    barPoints.push_back( cp[0]->position );
    barPoints.push_back( cp[1]->position );
    
    barPoints.push_back( cp[2]->position );
    barPoints.push_back( cp[3]->position );
    barPoints.push_back( cp[3]->position );
    barPoints.push_back( cp[4]->position );
    
    barPoints.push_back( cp[5]->position );
    barPoints.push_back( cp[6]->position );
    barPoints.push_back( cp[6]->position );
    barPoints.push_back( cp[7]->position );
    
    barPoints.push_back( cp[8]->position );
    barPoints.push_back( cp[9]->position );
    barPoints.push_back( cp[9]->position );
    barPoints.push_back( cp[10]->position );
    
    barPoints.push_back( cp[11]->position );
    barPoints.push_back( cp[12]->position );
    barPoints.push_back( cp[12]->position );
    barPoints.push_back( cp[13]->position );
    
    barPoints.push_back( cp[14]->position );
    barPoints.push_back( cp[15]->position );
    barPoints.push_back( cp[15]->position );
    barPoints.push_back( cp[16]->position );
    
    barPoints.push_back( cp[17]->position );
    barPoints.push_back( cp[18]->position );
    barPoints.push_back( cp[18]->position );
    barPoints.push_back( cp[19]->position );
    
    barPoints.push_back( cp[20]->position );
    barPoints.push_back( cp[21]->position );
    barPoints.push_back( cp[21]->position );
    barPoints.push_back( cp[22]->position );
    
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, barPoints.size()* sizeof(glm::vec3), &barPoints[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // the call to glVertexAttribPointer registered VBO as the currently bound vertex
    // buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // Unbind VAO
}

Bars::~Bars()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void Bars::draw(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
    
    // Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
    glm::mat4 MVP = Window::P * Window::V * toWorld;
    glm::mat4 model = toWorld;
    glm::mat4 camera = Window::V;
    
    // We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009),
    // gl_ModelViewProjectionMatrix has been removed from the language. The user is expected to supply
    // this matrix to the shader when using modern OpenGL.
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint CamID = glGetUniformLocation(shaderProgram, "camera");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "model");
    
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(CamID, 1, GL_FALSE, &camera[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (GLint)barPoints.size()) ;
    
    glBindVertexArray(0);
}


void Bars::update(ControlPoint *cp[], int num_cp){
    
    barPoints.clear();
    
    barPoints.push_back( cp[23]->position );
    barPoints.push_back( cp[0]->position );
    barPoints.push_back( cp[0]->position );
    barPoints.push_back( cp[1]->position );
    
    barPoints.push_back( cp[2]->position );
    barPoints.push_back( cp[3]->position );
    barPoints.push_back( cp[3]->position );
    barPoints.push_back( cp[4]->position );
    
    barPoints.push_back( cp[5]->position );
    barPoints.push_back( cp[6]->position );
    barPoints.push_back( cp[6]->position );
    barPoints.push_back( cp[7]->position );
    
    barPoints.push_back( cp[8]->position );
    barPoints.push_back( cp[9]->position );
    barPoints.push_back( cp[9]->position );
    barPoints.push_back( cp[10]->position );
    
    barPoints.push_back( cp[11]->position );
    barPoints.push_back( cp[12]->position );
    barPoints.push_back( cp[12]->position );
    barPoints.push_back( cp[13]->position );
    
    barPoints.push_back( cp[14]->position );
    barPoints.push_back( cp[15]->position );
    barPoints.push_back( cp[15]->position );
    barPoints.push_back( cp[16]->position );
    
    barPoints.push_back( cp[17]->position );
    barPoints.push_back( cp[18]->position );
    barPoints.push_back( cp[18]->position );
    barPoints.push_back( cp[19]->position );
    
    barPoints.push_back( cp[20]->position );
    barPoints.push_back( cp[21]->position );
    barPoints.push_back( cp[21]->position );
    barPoints.push_back( cp[22]->position );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, barPoints.size()* sizeof(glm::vec3), &barPoints[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Bars::scale(float i){
    
    glm::mat4 matrix = glm::mat4(1.0f);
    this->toWorld = this->toWorld * glm::scale(matrix, glm::vec3(i, i, i)) ;
}
