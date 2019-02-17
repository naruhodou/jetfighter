#include "main.h"

#ifndef VOLCANO_H
#define VOLCANO_H
#define GLM_ENABLE_EXPERIMENTAL

class Volcano {
public:
    Volcano() {}
    Volcano(glm::vec3 pos, glm :: vec3 dim, color_t color);
    glm::vec3 position;
    float rotation, bradius, tradius, length;
    bool isdraw;
    void draw(glm::mat4 VP);
    void set_position(glm::vec3);
    void tick(int sn);
    double speed;
private:
    VAO *object;
};

#endif
