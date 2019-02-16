#include "main.h"

#ifndef CUBOID_H
#define CUBOID_H
#define GLM_ENABLE_EXPERIMENTAL

class Cuboid {
public:
    Cuboid() {}
    Cuboid(glm::vec3 pos, glm::vec3 dimensions, float rot, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif // BALL_H
