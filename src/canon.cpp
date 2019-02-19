#include "canon.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Canon::Canon(glm::vec3 pos, float rotation) {
    this->position = pos;
    this->rotation = rotation;
    this->isdraw = true;
    this->lower = Cuboid(this->position, {5, 2, 5}, 0, COLOR_DEEPGREEN);
    this->upper = Cuboid({pos.x, pos.y + 1, pos.z}, {2, 4, 1}, 0, COLOR_GREEN);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
}

void Canon::draw(glm::mat4 VP) {
    this->lower.draw(VP);
    this->upper.draw(VP);
}

void Canon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Canon::tick(int sn) {
    // this->rotation += 10 * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    if(sn == 1)
    {
        //forward
        this -> position.y -= 0.05;
    }
    else if(sn == -1)
    {
        //backward
        this -> position.y += 0.05;
    }
}

