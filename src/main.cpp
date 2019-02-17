#include "main.h"
#include "timer.h"
#include "ball.h"
#include "plane.h"
#include "cuboid.h"
#include "arrow.h"
#include "volcano.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
#define GLM_ENABLE_EXPERIMENTAL

/**************************
* Customizable functions *
**************************/
Plane player;
Cuboid lake, altitude_tracker, speed_tracker, fuel_tracker;
Arrow direct_player;
vector <Volcano> volcanoes;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, diff = 10;
float camera_rotation_angle = 90;
int window_size = 20, awidth, aheight, sync_arrow = 80, global_timestamp = 0;
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
    glm::vec3 eye (player.position.x, player.position.y + 4, player.position.z + 6);
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
    for(auto x : volcanoes)
        x.draw(VP);
    altitude_tracker.draw(VP);
    speed_tracker.draw(VP);
    fuel_tracker.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int forward  = glfwGetKey(window, GLFW_KEY_W);
    int back = glfwGetKey(window, GLFW_KEY_S);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int lrotate = glfwGetKey(window, GLFW_KEY_A);
    int rrotate = glfwGetKey(window, GLFW_KEY_D);
    int ltilt = glfwGetKey(window, GLFW_KEY_Z);
    int rtilt = glfwGetKey(window, GLFW_KEY_C);
    if(forward)
    {
        player.tick(1);
        player.fuel -= 0.1;
    }
    if(back)
    {
        player.tick(-1);
        player.fuel -= 0.1;
    }
    if(up)
    {
        player.fuel -= 0.1;
        player.tick(2);
    }
    if(down)
    {
        player.fuel -= 0.1;
        player.tick(-2);
    }
    if(lrotate)
    {
        player.fuel -= 0.1;
        player.ry += 0.5;
    }
    if(rrotate)
    {
        player.ry -= 0.5;
    }
    if(ltilt)
    {
        player.rz += 0.5;
    }
    if(rtilt)
    {
        player.rz -= 0.5;
    }
    if(!forward && !back)
        player.tick(0);
}

//altitude bar
void handle_altitude()
{
    float unit_length = 0.05;
    altitude_tracker = Cuboid({player.position.x - 10, player.position.y + 5, player.position.z - 20.0f}, {unit_length * player.position.y, 1, 0.01}, 0, COLOR_BLACK);
}
//speed handler
void handle_speed()
{
    float unit_length = 20;
    speed_tracker = Cuboid({player.position.x - 10, player.position.y + 3, player.position.z - 20.0f}, {unit_length * player.speed, 1, 0.01}, 0, COLOR_DEEPBLUE);
}
void handle_fuel()
{
    float unit_length = 0.08;
    fuel_tracker = Cuboid({player.position.x - 10, player.position.y + 1, player.position.z - 20.0f}, {unit_length * player.fuel, 1, 0.01}, 0, COLOR_BLACK);
}
//dashboard_handler
void dashboard_handler()
{
    if(player.position.y > 0)
        handle_altitude();
    handle_speed();
    if(player.fuel > 0)
        handle_fuel();
}

void tick_elements() {
    
    //always happening
    global_timestamp++;
    dashboard_handler();
    if(!direct_player.isdraw && global_timestamp % 100 == 0)
    {
        direct_player.isdraw = true;
        direct_player.position = {player.position.x, player.position.y, player.position.z - 60};
        if(volcanoes.size() < 40)
        {
            for(int i = 1; i <= 2; i++)
            {
                volcanoes.push_back(Volcano({20 * i + player.position.x, 2, player.position.z - 25 * i}, {7, 3, 7}, COLOR_ORANGE));
            }
            for(int i = 1; i <= 2; i++)
            {
                volcanoes.push_back(Volcano({player.position.x - 30 * i, 2, player.position.z - 25 * i}, {7, 3, 7}, COLOR_ORANGE));
            }
        }
    }
    if(global_timestamp % sync_arrow < sync_arrow / 2)
    {
        direct_player.tick(-1);
    }
    else
    {
        direct_player.tick(1);
    }
    if(2 * abs(player.position.z - direct_player.position.z) < player.h && 2 * abs(player.position.x - direct_player.position.x) < player.l)
    {
        if(direct_player.isdraw)
            player.fuel += 75;
        direct_player.isdraw = false;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    player      = Plane({0, 10, 0}, {2, 1, 8}, COLOR_GREEN);
    lake      = Cuboid({player.position.x, player.position.y - diff, player.position.z}, {1000, 1, 1000}, 0, COLOR_BLUE);
    direct_player = Arrow({0, 10, -40}, COLOR_RED);
    for(int i = 1; i <= 2; i++)
    {
        volcanoes.push_back(Volcano({10 * i + player.position.x, 2, player.position.z - 20 * i}, {7, 3, 7}, COLOR_ORANGE));
    }
    for(int i = 1; i <= 2; i++)
    {
        volcanoes.push_back(Volcano({player.position.x - 10 * i, 2, player.position.z - 25 * i}, {7, 3, 7}, COLOR_ORANGE));
    }
    altitude_tracker = Cuboid({player.position.x - 10, player.position.y + 5, player.position.z - 20.0f}, {0.05 * player.position.y, 1, 0.01}, 0, COLOR_BLACK);
    speed_tracker = Cuboid({player.position.x - 10, player.position.y + 3, player.position.z - 20.0f}, {20 * player.speed, 1, 0.01}, 0, COLOR_DEEPBLUE);
    fuel_tracker = Cuboid({player.position.x - 10, player.position.y + 1, player.position.z - 20.0f}, {0.08 * player.fuel, 1, 0.01}, 0, COLOR_BLACK);
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
    Matrices.projection = glm::perspective(glm::radians(80.0f), (float)awidth /(float)aheight , 0.1f, 200.0f);
}
