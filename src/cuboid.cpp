#include "cuboid.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Cuboid::Cuboid(glm::vec3 pos, glm::vec3 dim, float rot, color_t color) {
    this->position = pos;
    this->rotation = rot;
    this->rz = 0;
    float length = dim.x, width = dim.y, depth = dim.z;
    this->vel = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = 
    {
        length / 2, width / 2, depth / 2, //front
        length / 2, -width / 2, depth / 2,
        -length / 2, -width / 2, depth / 2,
        -length / 2, -width / 2, depth / 2,
        -length / 2, width / 2, depth / 2,
        length / 2, width / 2, depth / 2,

        length / 2, width / 2, -depth / 2, //back
        length / 2, -width / 2, -depth / 2,
        -length / 2, -width / 2, -depth / 2,
        -length / 2, -width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        length / 2, width / 2, -depth / 2,
        
        length / 2, width / 2, depth / 2, //right
        length / 2, width / 2, -depth / 2,
        length / 2, -width / 2, -depth / 2,
        length / 2, -width / 2, -depth / 2,
        length / 2, -width / 2, depth / 2,
        length / 2, width / 2, depth / 2,

        -length / 2, width / 2, depth / 2, //left
        -length / 2, width / 2, -depth / 2,
        -length / 2, -width / 2, -depth / 2,
        -length / 2, -width / 2, -depth / 2,
        -length / 2, -width / 2, depth / 2,
        -length / 2, width / 2, depth / 2,

        length / 2, width / 2, depth / 2, //top
        length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, depth / 2,
        length / 2, width / 2, depth / 2, 

        length / 2, width / 2, depth / 2, //bottom
        length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, depth / 2,
        length / 2, width / 2, depth / 2,
    };
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Cuboid::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    Matrices.model *= (glm::rotate((float) (this->rz * M_PI / 180.0f), glm::vec3(0, 0, 1)));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
