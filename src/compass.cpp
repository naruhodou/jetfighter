#include "compass.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Compass::Compass(glm::vec3 pos, float side) {
    this->position = pos;
    this->rotation = 0;
    this->isdraw = true;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float length = 2, height = 1;
    GLfloat vertex_buffer_data1[] = 
    {
        side / 2, 0, 0,
        0, side, 0, 
        -side / 2, 0, 0,
    };
    GLfloat vertex_buffer_data2[] = 
    {
        side / 2, 0, 0,
        0, -side, 0, 
        -side / 2, 0, 0,
    };
    this->object1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data1, COLOR_RED, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data2, COLOR_PURPLE, GL_FILL);
}

void Compass::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}
