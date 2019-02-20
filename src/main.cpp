#include "main.h"
#include "timer.h"
#include "ball.h"
#include "plane.h"
#include "cuboid.h"
#include "arrow.h"
#include "volcano.h"
#include "canon.h"
#include "parachute.h"
#include "compass.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
#define GLM_ENABLE_EXPERIMENTAL

/**************************
* Customizable functions *
**************************/
Plane player;
Compass c1;
Cuboid lake, altitude_tracker, speed_tracker, fuel_tracker;
Arrow direct_player;
Canon enemy1;
Parachute enemy2;
vector <Volcano> volcanoes;
vector <Cuboid> missiles, bombs;
int plane_view, top_view, tower_view, follow_cam = 1, helicopter_view;
glm :: vec3 set_tower_view = {0, 0, 0};
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, diff = 10;
float camera_rotation_angle = 90;
int window_size = 20, awidth, aheight, sync_arrow = 80, start_barrel_roll;
long long global_timestamp = 0;
double camera_zoom = 1;
Timer t60(1.0 / 60);
double gx, gy, hx, hy, px, py;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // Do something
    camera_zoom += 0.1*yoffset;
    reset_screen();
}
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
    glm::vec3 eye;
    if(plane_view)
    {
        eye = {player.position.x, player.position.y, player.position.z - 5 * player.h / 4};
    }
    else if(top_view)
    {
        eye = player.position;
        eye.y += 20;
        eye.z += 3;
    }
    if(follow_cam)
    {
        eye = {player.position.x, player.position.y + 4, player.position.z + 6};
    }
    if(tower_view)
    {
        if(abs(set_tower_view.x) + abs(set_tower_view.y) + abs(set_tower_view.z) < 0.01)
        {
            set_tower_view = player.position;
            set_tower_view.x -= 5;
            set_tower_view.y += 10;
        }
        eye = set_tower_view;
    }
    
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (player.position.x, player.position.y, player.position.z);
    if(plane_view)
    {
        target = eye;
        target.z -= 10 * cos(M_PI * player.ry / 180);
        target.x -= 10 * sin(M_PI * player.ry / 180);
    }
    if(helicopter_view)
    {
        glfwGetCursorPos(window, &gx, &gy);
        hx += (gx - px) * 90 / awidth;
        hy -= (gy - py) * 90 / aheight;
        px = gx;
        py = gy;
        eye = glm::vec3(player.position.x + (12 - 8 * camera_zoom) * cos(hx * M_PI / 180) * sin(hy * M_PI / 180), player.position.y + (12 - 8 * camera_zoom) * cos(hy * M_PI / 180), player.position.x + (12 - 8 * camera_zoom) * sin(hx * M_PI / 180) * sin(hy * M_PI / 180));
        target = player.position;
    }
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
    altitude_tracker.draw({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
    speed_tracker.draw({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
    fuel_tracker.draw({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
    if(enemy1.isdraw)
        enemy1.draw(VP);
    for(auto x : missiles)
        x.draw(VP);
    for(int i = 0; i < bombs.size(); i++)
        bombs[i].draw(VP);
    if(enemy2.isdraw)
        enemy2.draw(VP);
    c1.draw({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        missiles.push_back(Cuboid({player.position.x, player.position.y - 3 * player.b / 4, player.position.z}, {0.5, 0.5, 1.5}, 0, COLOR_RED));
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        bombs.push_back(Cuboid({player.position.x, player.position.y - 3 * player.b / 4, player.position.z}, {0.2, 0.2, 0.2}, 0, COLOR_RED));
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
    int sbr = glfwGetKey(window, GLFW_KEY_L);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    if(sbr)
        start_barrel_roll = global_timestamp;
    if(!plane_view)
    {
        plane_view = glfwGetKey(window, GLFW_KEY_1);
        if(plane_view)
        {
            tower_view = top_view = follow_cam = helicopter_view = 0;
        }
    }
    if(!top_view)
    {
        top_view = glfwGetKey(window, GLFW_KEY_2);
        if(top_view)
        {
            plane_view = tower_view = follow_cam = helicopter_view = 0;
        }
    }
    if(!follow_cam)
    {
        follow_cam = glfwGetKey(window, GLFW_KEY_3);
        if(follow_cam)
        {
            plane_view = tower_view = top_view = helicopter_view = 0;
        }
    }
    if(!tower_view)
    {
        tower_view = glfwGetKey(window, GLFW_KEY_4);
        set_tower_view = {0, 0, 0};
        if(tower_view)
        {
            plane_view = top_view = follow_cam = helicopter_view = 0;
        }
    }
    if(!helicopter_view)
    {
        helicopter_view = glfwGetKey(window, GLFW_KEY_5);
        if(helicopter_view)
        {
            plane_view = top_view = follow_cam = tower_view = 0;
        }
    }
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
void reset_game()
{
    player      = Plane({0, 10, 0}, {2, 1, 8}, COLOR_GREEN);
    volcanoes.clear();
    enemy1.isdraw = enemy2.isdraw = direct_player.isdraw = false;
}
//altitude bar
void handle_altitude()
{
    float unit_length = 0.005;
    altitude_tracker = Cuboid({-0.8 + (unit_length * player.position.y) / 2, 0.7, 0}, {unit_length * player.position.y, 0.05, 0}, 0, COLOR_BLACK);
}
//speed handler
void handle_speed()
{
    float unit_length = 1;
    speed_tracker = Cuboid({-0.8 + (unit_length * player.speed) / 2, 0.6, 0}, {unit_length * player.speed, 0.05, 0}, 0, COLOR_DEEPBLUE);
}
void handle_fuel()
{
    float unit_length = 0.002 ;
    fuel_tracker = Cuboid({-0.8 + (unit_length * player.fuel) / 2, 0.5, 0}, {unit_length * player.fuel, 0.05, 0}, 0, COLOR_FUEL);
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

//enemy2 movement
void enemy2_move()
{
    if(enemy2.position.y < 1)
    {
        enemy2.isdraw = false;
        return;
    }
    if(global_timestamp % 60 >= 15 && global_timestamp < 45)
        enemy2.tick(1);
    else
        enemy2.tick(-1);
}


//missile movement
void miss_move()
{
    for(int i = 0; i < missiles.size(); i++)
    {
        missiles[i].position.z -= 0.3 * cos(M_PI * player.ry / 180);
        missiles[i].position.x -= 0.3 * sin(M_PI * player.ry / 180);
        float fv = missiles[i].vel - 1.0f / 6.0f;
        missiles[i].position.y = missiles[i].position.y + (missiles[i].vel * missiles[i].vel - fv * fv) / 20.0f;
        missiles[i].vel = fv;
    }
    vector <Cuboid> temp;
    for(int i = 0; i < missiles.size(); i++)
    {
        float d1 = abs(enemy1.position.x - missiles[i].position.x);
        float d2 = abs(enemy1.position.y - missiles[i].position.y);
        float d3 = abs(enemy1.position.z - missiles[i].position.z);
        bool c1 = 0, c2 = 0;
        if(abs(2 * d1 - 0.5) < 1 && abs(2 * d2 - 0.5) < 1 && abs(2 * d3 - 1.5) < 1 && enemy1.isdraw)
        {
            enemy1.isdraw = false;
            c1 = 1;
        }
        d1 = abs(enemy2.position.x - missiles[i].position.x);
        d2 = abs(enemy2.position.y - missiles[i].position.y);
        d3 = abs(enemy2.position.z - missiles[i].position.z);
        if(abs(2 * d1 - 0.5) < 1 && abs(2 * d2 - 2.5) < 1 && abs(2 * d3 - 0.5) < 1 && enemy2.isdraw)
        {
            enemy2.isdraw = false;
            c2 = 1;
        }
        if(c1 || c2)
            continue;
        if(missiles[i].position.y > 0)
            temp.push_back(missiles[i]);
    }
    missiles = temp;
}

void bomb_move()
{
    for(int i = 0; i < bombs.size(); i++)
    {
        bombs[i].position.z -= 0.3 * cos(M_PI * player.ry / 180);
        bombs[i].position.x -= 0.3 * sin(M_PI * player.ry / 180);
        float fv = bombs[i].vel - 1.0f / 6.0f;
        bombs[i].position.y = bombs[i].position.y + (bombs[i].vel * bombs[i].vel - fv * fv) / 20.0f;
        bombs[i].vel = fv;
    }
    vector <Cuboid> temp;
    for(int i = 0; i < bombs.size(); i++)
    {
        float d1 = abs(enemy1.position.x - bombs[i].position.x);
        float d2 = abs(enemy1.position.y - bombs[i].position.y);
        float d3 = abs(enemy1.position.z - bombs[i].position.z);
        bool c1 = 0, c2 = 0;
        if(abs(2 * d1 - 0.5) < 1 && abs(2 * d2 - 0.5) < 1 && abs(2 * d3 - 1.5) < 1 && enemy1.isdraw)
        {
            enemy1.isdraw = false;
            c1 = 1;
        }
        d1 = abs(enemy2.position.x - bombs[i].position.x);
        d2 = abs(enemy2.position.y - bombs[i].position.y);
        d3 = abs(enemy2.position.z - bombs[i].position.z);
        if(abs(2 * d1 - 0.5) < 1 && abs(2 * d2 - 2.5) < 1 && abs(2 * d3 - 0.5) < 1 && enemy2.isdraw)
        {
            enemy2.isdraw = false;
            c2 = 1;
        }
        if(c1 || c2)
            continue;
        if(bombs[i].position.y > 0)
            temp.push_back(bombs[i]);
    }
    bombs = temp;
}

//volcano no fly zone
void handle_volcano()
{
    vector <glm::vec2> corners = {{player.position.x, player.position.z - 5 * player.l / 4}, {player.position.x - player.l / 2, player.position.z}, {player.position.x - 3 * player.l / 2, player.position.z}, {player.position.x + player.l / 2, player.position.z}, {player.position.x + 3 * player.l / 2, player.position.z}};
    for(int i = 0; i < volcanoes.size(); i++)
    {
        for(int j = 0; j < corners.size(); j++)
        {
            cout << (corners[j].x - volcanoes[i].position.x) * (corners[j].x - volcanoes[i].position.x) + (corners[j].y - volcanoes[i].position.z) * (corners[j].y - volcanoes[i].position.z) << endl;
            if((corners[j].x - volcanoes[i].position.x) * (corners[j].x - volcanoes[i].position.x) + (corners[j].y - volcanoes[i].position.z) * (corners[j].y - volcanoes[i].position.z) < volcanoes[i].tradius * volcanoes[i].tradius)
            {
                reset_game();
                return;
            }
        }
    }
}

void tick_elements() {
    
    //always happening
    global_timestamp++;
    miss_move();
    bomb_move();
    enemy2_move();
    handle_volcano();
    dashboard_handler();
    if(player.fuel < 0.1 || player.position.y < 0.5)
    {
        reset_game();
    }
    if(global_timestamp - start_barrel_roll < 60 && start_barrel_roll)
    {
        player.rz += 5.6;
    }
    else
        start_barrel_roll = 0;
    if(!direct_player.isdraw && global_timestamp % 100 == 0)
    {
        direct_player.isdraw = true;
        direct_player.position = {player.position.x, player.position.y, player.position.z - 60};
        enemy1 = Canon({player.position.x, 1, player.position.z - 60}, 0);
        enemy1.isdraw = true;
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
    if(!enemy2.isdraw && global_timestamp % 400 == 0)
    {
        enemy2 = Parachute({player.position.x, player.position.y + 5, player.position.z - 50});
        enemy2.isdraw = true;
    }
    if(global_timestamp % sync_arrow < sync_arrow / 2)
    {
        direct_player.tick(-1);
    }
    else
    {
        direct_player.tick(1);
    }
    // if(2 * abs(player.position.z - direct_player.position.z) < player.h && 2 * abs(player.position.x - direct_player.position.x) < player.l)
    // {
    //     if(direct_player.isdraw)
    //         player.fuel += 75;
    //     direct_player.isdraw = false;
    // }
    if(!enemy1.isdraw)
    {
        if(direct_player.isdraw)
            player.fuel += 75;
        direct_player.isdraw = false;
    }
    c1.rotation = -player.ry;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    player      = Plane({0, 10, 0}, {2, 1, 8}, COLOR_GREEN);
    c1          = Compass({0.8, -0.8, 0}, 0.1);
    lake      = Cuboid({player.position.x, player.position.y - diff, player.position.z}, {1000, 1, 1000}, 0, COLOR_BLUE);
    direct_player = Arrow({0, 10, -40}, COLOR_RED);
    enemy1 = Canon({0, 1, -40}, 0);
    enemy2 = Parachute({0, 10, -60});


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
