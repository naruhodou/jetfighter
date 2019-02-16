#include "main.h"

#ifndef ARROW_H
#define ARROW_H
#define GLM_ENABLE_EXPERIMENTAL

class Arrow {
public:
    Arrow() {}
    Arrow(glm::vec3 pos, color_t color);
    glm::vec3 position;
    float rotation;
    bool isdraw;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
    double speed;
private:
    VAO *object;
};

#endif
