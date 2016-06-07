#include "window.h"

const char* window_title = "GLFW Starter Project";
glm::mat4 Window::P;
glm::mat4 Window::V;
int Window::width;
int Window::height;

GLint shaderProgram;
GLint skyboxShader;
GLint trackShader;
GLint boxShader;
GLint oceanShader;

const int NUM_CP = 24;
bool stop = true;
bool drawBox = false;
bool collision = false;
float trans = 0.0;
float trans2 = 0.0;

//========================[ Objects ]==========================//
Cube * skyBox;
OBJObject * boat;
OBJObject * boat2;
OBJObject * ocean;
ControlPoint * cp[NUM_CP];
ControlPoint * cp2[NUM_CP];
BezierCurve * bc1;
BezierCurve * bc2;
std::vector<BoundingBox*> bb_vec;
std::vector<BoundingBox*> bb_vec2;
BoatGenerator *boat_generator;
BoatGenerator *boat_generator2;
//===============[ Default camera parameters ]================//
glm::vec3 eye_pos;
glm::vec3 look_dir;
glm::mat4 matrix;

glm::vec3 cam_pos(0.0f, 50.0f, 500.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, -20.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

bool clicked = false;
glm::vec3 lastPoint;                        // coordinates of the last position
glm::vec3 curPoint;                         // coordinates of the current position
GLfloat fov =  45.0f;
//========================[ Colors ]=========================//
//material color: gold
glm::vec3 matAmb(0.329412,0.223529,0.027451);
glm::vec3 matDiff(0.780392, 0.568627, 0.113725);
glm::vec3 matSpec(0.992157, 0.941176, 0.807843);
float matShine = 3;

//material color: turquoise
glm::vec3 matAmb2(0.1, 0.18725, 0.1745);
glm::vec3 matDiff2(0.396, 0.74151, 0.69102);
glm::vec3 matSpec2(0.297254, 0.30829, 0.306678);
float matShine2 = 0.1;

// directional & point light
glm::vec3 dirAmb(0.8f, 0.8f, 0.8f);
glm::vec3 dirDiff(0.50f, 0.50f, 0.50f);
glm::vec3 dirSpec(0.8f, 0.8f, 0.8f);
glm::vec3 dirLight_dir(0, -30, -30);
//=======================[ Sound Effects ]===================================//
ISoundEngine *SoundEngine;
//=======================[ Physics ]===================================//
int counter = 0;                            // for location of the boat on the track
float timePassed = 0;                       // for the time for the track
float velocity;
int counter2 = 0;                            // for location of the boat on the track
float timePassed2 = 0;                       // for the time for the track
float velocity2;

glm::vec3 lastBoatLocation;                  // old location of the pod on the track
glm::vec3 nextBoatLocation;                   // new location of the pod on the track
glm::vec3 lastBoatLocation2;                  // old location of the pod on the track
glm::vec3 nextBoatLocation2;                   // new location of the pod on the track


void Window::initialize_objects()
{
    skyBox = new Cube();
    skyBox->scale(20);
    boat = new OBJObject("Boat.obj");
    boat2 = new OBJObject("Boat.obj");
    
    ocean = new OBJObject("Ocean.obj");
    boat_generator = new BoatGenerator();
    boat_generator2 = new BoatGenerator();
    
    bc1 = new BezierCurve(cp, NUM_CP);
    bc2 = new BezierCurve(cp2, NUM_CP);
    
    for (int i = 0; i < boat_generator->c_vec.size(); i++) {
        BoundingBox * bb = new BoundingBox(boat_generator->c_vec[i]);
        BoundingBox * bb2 = new BoundingBox(boat_generator2->c_vec[i]);
        bb_vec.push_back(bb);
        bb_vec2.push_back(bb2);
    }
    
    SoundEngine = createIrrKlangDevice();
    SoundEngine->play2D("Thunderstorm.wav", GL_TRUE);
    SoundEngine->play2D("Battle sounds.wav", GL_TRUE);
    SoundEngine->play2D("bensound-epic.wav", GL_TRUE);
    
    ocean->scale(300);
    ocean->translate(0.0f, 1.0f, 0.0f);
    boat->scale(0.6);
    boat->translate(40.0f, 1.0f, 180.0f);
    boat2->scale(0.6);
    boat2->translate(-80.0f, 1.0f, 200.0f);
    
    boat_generator->world->update( getMatrix(glm::vec3(0.0, 0.0, -50), glm::vec3(1.0f), 0, 0) );
    boat_generator2->world->update( getMatrix(glm::vec3(-50, 0.0, 160.0), glm::vec3(1.0f), -90, 2) );

    counter = bc1->getCounter();
    timePassed = bc1->getTime();
    counter2 = bc2->getCounter();
    timePassed2 = bc2->getTime();
    
    lastBoatLocation = moveBoat(0);
    lastBoatLocation2 = moveBoat(4);
    
    shaderProgram = LoadShaders("shader.vert", "shader.frag");
    skyboxShader = LoadShaders("skyShader.vert", "skyShader.frag");
    trackShader = LoadShaders("track.vert", "track.frag");
    boxShader = LoadShaders("boxShader.vert", "boxShader.frag");
    oceanShader = LoadShaders("oceanShader.vert", "oceanShader.frag");
}

void Window::clean_up()
{
    delete(boat);
    delete(boat2);
    delete(skyBox);
    delete(ocean);
    delete(bc1);
    delete(bc2);
    delete(boat_generator);
    delete(boat_generator2);
    for (int i = 0; i < bb_vec.size(); i++) {
        delete(bb_vec[i]);
        delete(bb_vec2[i]);
    }
    for(int i =0; i < NUM_CP; i++) {
        delete(cp[i]);
        delete(cp2[i]);
    }

	glDeleteProgram(shaderProgram);
    glDeleteProgram(skyboxShader);
    glDeleteProgram(trackShader);
    glDeleteProgram(boxShader);
    glDeleteProgram(oceanShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0) {
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
    //=======================[ Controls camera ]================================//
    if ( clicked && (lastPoint.x != curPoint.x || lastPoint.y != curPoint.y)) {
        
        glm::vec3 curPoint_3d = trackBallMapping( curPoint.x, curPoint.y );    // 2d position -> sphere location.
        glm::vec3 lastPoint_3d = trackBallMapping( lastPoint.x, lastPoint.y );
        
        glm::vec3 direction = curPoint_3d - lastPoint_3d;
        float velocity = glm::length(direction);
        
        if( velocity > 0.001 ) {      // If little movement - do nothing.
            glm::vec3 rotAxis = glm::cross( lastPoint_3d, curPoint_3d );

            float rot_angle = glm::acos( glm::min(1.0f, glm::dot(curPoint_3d, lastPoint_3d)) ) * 0.01;
            skyBox->angle = rot_angle;

            glm::vec4 new_cam = glm::vec4(cam_pos, 1.0f) ;
            new_cam = new_cam * glm::rotate( glm::degrees(skyBox->angle), rotAxis );

            cam_pos.x = new_cam.x;
            cam_pos.y = new_cam.y;
            cam_pos.z = new_cam.z;
            
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
    }
    
    if (!stop) {
        // ******** Boat 1 movement ******** //
        float displacement = bc1->getTallest().y - nextBoatLocation.y;
        if ( displacement < 0 ) displacement *= -0.5;
        // give it a little push
        displacement += 0.1;
        
        velocity = sqrt( 0.0005 * displacement );
        
        timePassed += velocity;
        
        nextBoatLocation = moveBoat(velocity);
        
        glm::vec3 z_axis = glm::normalize( nextBoatLocation - lastBoatLocation );
        glm::vec3 x_axis = glm::normalize( glm::cross( glm::vec3 (0, 1, 0), z_axis) );
        glm::vec3 y_axis = glm::normalize( glm::cross(z_axis, x_axis)  );
        
        glm::mat4 transMatrix(1.0f);
        transMatrix[0] = glm::vec4( x_axis, 0.0f);
        transMatrix[1] = glm::vec4( y_axis, 0.0f);
        transMatrix[2] = glm::vec4( z_axis, 0.0f);
        transMatrix[3] = glm::vec4( nextBoatLocation - lastBoatLocation, 1.0f);
        
        boat->toWorld = transMatrix;
        boat->moveTo(nextBoatLocation);
        lastBoatLocation = nextBoatLocation;
        
        
        // ******** Boat 2 movement ******** //
        float displacement2 = bc2->getTallest().y - nextBoatLocation2.y;
        if ( displacement2 < 0 ) displacement2 *= -0.5;
        // give it a little push
        displacement2 += 0.1;
        
        velocity2 = sqrt( 0.0001 * displacement2 );
        
        timePassed2 += velocity2;
        
        nextBoatLocation2 = moveBoat2(velocity2);
        
        glm::vec3 z_axis2 = glm::normalize( nextBoatLocation2 - lastBoatLocation2 );
        glm::vec3 x_axis2 = glm::normalize( glm::cross( glm::vec3 (0, 1, 0), z_axis2) );
        glm::vec3 y_axis2 = glm::normalize( glm::cross(z_axis2, x_axis2)  );
        
        glm::mat4 transMatrix2(1.0f);
        transMatrix2[0] = glm::vec4( x_axis2, 0.0f);
        transMatrix2[1] = glm::vec4( y_axis2, 0.0f);
        transMatrix2[2] = glm::vec4( z_axis2, 0.0f);
        transMatrix2[3] = glm::vec4( nextBoatLocation2 - lastBoatLocation2, 1.0f);
        
        boat2->toWorld = transMatrix2;
        boat2->moveTo(-nextBoatLocation2);
        lastBoatLocation2 = nextBoatLocation2;
        
        // ******** Boat 3 movement ******** //
        trans += 0.4;
        boat_generator->world->update( getMatrix(glm::vec3(0.0, 0.0, trans - 50), glm::vec3(1.0f), 0, 0) );
        
        trans2 += 0.8;
        boat_generator2->world->update( getMatrix(glm::vec3(trans2 - 50, 0.0, 160.0), glm::vec3(1.0f), -90, 2) );
    }
    
    collision = false;
    
    for (int i = 0; i < bb_vec.size(); i++) {
        bb_vec[i]->update();
        bb_vec2[i]->update();
    }
    
    for (int i = 0; i < bb_vec.size(); i++) {
        for (int j = 0; j < bb_vec2.size(); j++) {
            if (bb_vec[i]->checkCollision(bb_vec2[j])) {
                collision = true;
                break;
            }
        }
    }
    
    for (int i = 0; i < bb_vec.size(); i++) {
        if (collision) {
            bb_vec[i]->collided = 1;
            bb_vec2[i]->collided = 1;
        }
        else {
            bb_vec[i]->collided = 0;
            bb_vec2[i]->collided = 0;
        }
    }
    
    if (collision)
        SoundEngine->play2D("explosion.wav", GL_FALSE);
    
    
    lastPoint = curPoint;
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(skyboxShader);
    skyBox->draw(skyboxShader);
    
    glUseProgram(oceanShader);
    
    //============================ Material ============================//
    GLuint materialID = glGetUniformLocation(oceanShader, "material.ambient");
    glUniform3fv(materialID, 1, (GLfloat*) &matAmb2);
    materialID = glGetUniformLocation(oceanShader, "material.diffuse");
    glUniform3fv(materialID, 1, (GLfloat*) &matDiff2);
    materialID = glGetUniformLocation(oceanShader, "material.specular");
    glUniform3fv(materialID, 1, (GLfloat*) &matSpec2);
    materialID = glGetUniformLocation(oceanShader, "material.shininess");
    glUniform1f(materialID, matShine2);
    //========================== Direct light ==========================//
    materialID = glGetUniformLocation(oceanShader, "dirLight.ambient");
    glUniform3fv(materialID, 1, (GLfloat*) &dirAmb);
    materialID = glGetUniformLocation(oceanShader, "dirLight.diffuse");
    glUniform3fv(materialID, 1, (GLfloat*) &dirDiff);
    materialID = glGetUniformLocation(oceanShader, "dirLight.specular");
    glUniform3fv(materialID, 1, (GLfloat*) &dirSpec);
    materialID = glGetUniformLocation(oceanShader, "dirLight.direction");
    glUniform3fv(materialID, 1, (GLfloat*) &dirLight_dir );
    
    ocean->draw(oceanShader);
    
    
	glUseProgram(shaderProgram);

    glm::vec3 curPoint; // direction of light follow by mouse

    //============================ Material ============================//
    materialID = glGetUniformLocation(shaderProgram, "material.ambient");
    glUniform3fv(materialID, 1, (GLfloat*) &matAmb);
    materialID = glGetUniformLocation(shaderProgram, "material.diffuse");
    glUniform3fv(materialID, 1, (GLfloat*) &matDiff);
    materialID = glGetUniformLocation(shaderProgram, "material.specular");
    glUniform3fv(materialID, 1, (GLfloat*) &matSpec);
    materialID = glGetUniformLocation(shaderProgram, "material.shininess");
    glUniform1f(materialID, matShine);
    //========================== Direct light ==========================//
    materialID = glGetUniformLocation(shaderProgram, "dirLight.ambient");
    glUniform3fv(materialID, 1, (GLfloat*) &dirAmb);
    materialID = glGetUniformLocation(shaderProgram, "dirLight.diffuse");
    glUniform3fv(materialID, 1, (GLfloat*) &dirDiff);
    materialID = glGetUniformLocation(shaderProgram, "dirLight.specular");
    glUniform3fv(materialID, 1, (GLfloat*) &dirSpec);
    materialID = glGetUniformLocation(shaderProgram, "dirLight.direction");
    glUniform3fv(materialID, 1, (GLfloat*) &dirLight_dir );
    
    
    //boat->draw(shaderProgram);
    //boat2->draw(shaderProgram);
    boat_generator->world->draw(shaderProgram);
    boat_generator2->world->draw(shaderProgram);
    
    if (drawBox) {
        glUseProgram(boxShader);
        for (int i = 0; i < bb_vec.size(); i++) {
            bb_vec[i]->draw(boxShader);
            bb_vec2[i]->draw(boxShader);
        }
    }
    

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
        
        if (key == GLFW_KEY_S) {
            if (stop == true)
                stop = false;
            else
                stop = true;
        }
        
        if (key == GLFW_KEY_D) {
            if (drawBox == true)
                drawBox = false;
            else
                drawBox = true;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mods != GLFW_MOD_SHIFT)
        clicked = true;
    else
        clicked = false;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    lastPoint.x = curPoint.x;
    lastPoint.y = curPoint.y;
    lastPoint.z = 0;
    
    curPoint.x = xpos;
    curPoint.y = ypos;
    curPoint.z = 0;
}

// Zoom in and out of the screen.
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    glm::vec3 dir = glm::normalize( glm::vec3 (0.0f) - cam_pos ) ;
    
    if (yoffset < 0)
        dir = -dir;
    
    glm::mat4 trans = glm::mat4 (1.0f);
    trans[3][0] = dir.x;
    trans[3][1] = dir.y;
    trans[3][2] = dir.z;

    glm::vec4 new_cam = trans * glm::vec4(cam_pos, 1.0f) ;
    cam_pos.x = new_cam.x;
    cam_pos.y = new_cam.y;
    cam_pos.z = new_cam.z;

    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

glm::vec3 Window::moveBoat(float velocity) {
    
    if (timePassed < 0.99) {
        timePassed += velocity;
        if (counter >23 )
            counter = 0;
    } else {
        counter = (counter < 20) ? counter + 3: 0;
        timePassed = 0;
    }
    
    glm::vec3 p0 = glm::vec3 ( cp[counter]->position );
    glm::vec3 p1 = glm::vec3 ( cp[counter+1]->position );
    glm::vec3 p2 = glm::vec3 ( cp[counter+2]->position );
    glm::vec3 p3;
    
    if( (counter+3) >= 24 )
        p3 = glm::vec3 ( cp[0]->position );
    else
        p3 = glm::vec3 ( cp[counter+3]->position );
    
    float c0 = (1-timePassed) * (1-timePassed) * (1-timePassed);
    float c1 = 3 * timePassed * (1-timePassed) * (1-timePassed);
    float c2 = 3 * timePassed * timePassed * (1-timePassed);
    float c3 = timePassed * timePassed * timePassed;
    
    glm::vec3 dot = c0 * p0 + c1 * p1 + c2 * p2 + c3 * p3;
    
    return dot;
}

glm::vec3 Window::moveBoat2(float velocity) {
    
    if (timePassed2 < 0.99) {
        timePassed2 += velocity;
        if (counter2 >23 )
            counter2 = 0;
    } else {
        counter2 = (counter2 < 20) ? counter2 + 3: 0;
        timePassed2 = 0;
    }
    
    glm::vec3 p0 = glm::vec3 ( cp2[counter2]->position );
    glm::vec3 p1 = glm::vec3 ( cp2[counter2+1]->position );
    glm::vec3 p2 = glm::vec3 ( cp2[counter2+2]->position );
    glm::vec3 p3;
    
    if( (counter2+3) >= 24 )
        p3 = glm::vec3 ( cp2[0]->position );
    else
        p3 = glm::vec3 ( cp2[counter2+3]->position );
    
    float c0 = (1-timePassed2) * (1-timePassed2) * (1-timePassed2);
    float c1 = 3 * timePassed2 * (1-timePassed2) * (1-timePassed2);
    float c2 = 3 * timePassed2 * timePassed2 * (1-timePassed2);
    float c3 = timePassed2 * timePassed2 * timePassed2;
    
    glm::vec3 dot = c0 * p0 + c1 * p1 + c2 * p2 + c3 * p3;
    
    return dot;
}

// Utility routine to calculate the 3D position of a projected unit vector onto the xy-plane.
glm::vec3 Window::trackBallMapping(double x, double y)
{
    glm::vec3 coord(0.0f);
    
    coord.x =  (2 * x - width ) / width;
    coord.y = -(2 * y - height) / height;
    
    /* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
    coord.x = glm::clamp( coord.x, -1.0f, 1.0f );
    coord.y = glm::clamp( coord.y, -1.0f, 1.0f );
    
    float length_squared = coord.x * coord.x + coord.y * coord.y;
    
    if( length_squared <= 1.0 )
        coord.z = sqrt( 1.0 - length_squared );
    else
        coord = glm::normalize( coord );
    
    return coord;
}

// m_pos = Translate vector, m_scale = resize vectore, m_angle = angle to rotate, axis type 1=x, 2=y, 3=z
glm::mat4 Window::getMatrix(glm::vec3 m_pos, glm::vec3 m_scale, float m_angle, int axis)
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



