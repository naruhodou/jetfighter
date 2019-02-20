#include "parachute.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Parachute::Parachute(glm::vec3 pos) {
    this->position = pos;
    this->rotation = 0;
    this->isdraw = true;
    this->lower = Cuboid(this->position, {0.5, 2, 0.5}, 0, COLOR_YELLOW);
    this->upper = Cuboid({pos.x, pos.y + 1.25, pos.z}, {5, 0.5, 0.5}, 0, COLOR_PURPLE);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
}

void Parachute::draw(glm::mat4 VP) {
    this->lower.draw(VP);
    this->upper.draw(VP);
}

void Parachute::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Parachute::tick(int sn) {
    // this->rotation += 10 * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    if(sn == 1)
    {
        //forward
        this -> position.y -= 0.01;
        this->lower.position.y -= 0.01;
        this->upper.position.y -= 0.01;
        // this->lower.rz += 0.1;
        // this->upper.rz += 0.1;
    }
    else if(sn == -1)
    {
        //backward
        this -> position.y -= 0.01;
        this->lower.position.y -= 0.01;
        this->upper.position.y -= 0.01;
        // this->upper.rz -= 0.1;
        // this->lower.rz -= 0.1;
    }
}

