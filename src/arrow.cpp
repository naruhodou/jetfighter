#include "arrow.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Arrow::Arrow(glm::vec3 pos, color_t color) {
    this->position = pos;
    this->rotation = 0;
    this->isdraw = true;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float length = 2, height = 1;
    GLfloat vertex_buffer_data[] = 
    {
        -length, 0, 0, //lower
        0, -height / 2, 0,
        0, 0, 0,
        0, 0, 0,
        length, 0, 0,
        0, -height / 2, 0,

        -length / 2, 0, 0, //upper
        -length / 2, height, 0,
        length / 2, 0, 0,
        length / 2, 0, 0,
        length / 2, height, 0,
        -length / 2, height, 0, 
    };
    this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, color, GL_FILL);
}

void Arrow::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Arrow::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Arrow::tick(int sn) {
    // this->rotation += 10 * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    if(sn == 1)
    {
        //forward
        this -> position.z -= 0.1;
    }
    else if(sn == -1)
    {
        //backward
        this -> position.z += 0.1;
    }
    else if(sn == 2)
    {
        //up
        this -> position.y += 0.1;
    }
}

