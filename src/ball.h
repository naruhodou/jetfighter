#include "main.h"

#ifndef BALL_H
#define BALL_H
#define GLM_ENABLE_EXPERIMENTAL

class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
