#include "main.h"
#include "timer.h"
#include "ball.h"
#include "plane.h"
#include "cuboid.h"
#include "arrow.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
#define GLM_ENABLE_EXPERIMENTAL

/**************************
* Customizable functions *
**************************/
Plane player;
Cuboid lake;
Arrow direct_player;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, diff = 10;
float camera_rotation_angle = 90;
int window_size = 10, awidth, aheight, sync_arrow = 80, global_timestamp = 0;
Timer t60(1.0 / 60);
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
#define GLM_ENABLE_EXPERIMENTAL
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye (player.position.x, player.position.y + 6, player.position.z + 8);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (player.position.x, player.position.y, player.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
#define GLM_ENABLE_EXPERIMENTAL
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    player.draw(VP);
    if(direct_player.isdraw)
        direct_player.draw(VP);
    lake.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int forward  = glfwGetKey(window, GLFW_KEY_W);
    int back = glfwGetKey(window, GLFW_KEY_S);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    if(forward)
        player.tick(1);
    if(back)
        player.tick(-1);
    if(up)
    {
        player.tick(2);
    }
    if(down)
    {
        player.tick(-2);
    }
}

void tick_elements() {
    // camera_rotation_angle += 1;
    global_timestamp++;
    if(!direct_player.isdraw && global_timestamp % 300 == 0)
    {
        direct_player.isdraw = true;
        direct_player.position = {player.position.x, player.position.y, player.position.z - 100};
    }
    if(global_timestamp % sync_arrow < sync_arrow / 2)
    {
        direct_player.tick(-1);
    }
    else
    {
        direct_player.tick(1);
    }
    if(2 * abs(player.position.z - direct_player.position.z) < player.h)
        direct_player.isdraw = false;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    player      = Plane({0, 10, 0}, {2, 1, 8}, COLOR_RED);
    lake      = Cuboid({player.position.x, player.position.y - diff, player.position.z}, {400, 1, 20}, -15, COLOR_BLUE);
    direct_player = Arrow({0, 15, -20}, COLOR_GREEN);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1200;
    int height = 1100;
    awidth = width;
    aheight = height;
    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + window_size / screen_zoom;
    float bottom = screen_center_y - window_size / screen_zoom;
    float left   = screen_center_x - window_size / screen_zoom;
    float right  = screen_center_x + window_size / screen_zoom;
    Matrices.projection = glm::perspective(glm::radians(80.0f), (float)awidth /(float)aheight , 0.1f, 500.0f);
}
