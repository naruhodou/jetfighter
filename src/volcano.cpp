#include "volcano.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Volcano::Volcano(glm::vec3 pos, glm::vec3 dim, color_t color) {
    this->position = pos;
    this->rotation = 0;
    this->isdraw = true;
    this->bradius = dim.x; this->tradius = dim.y; this->length = dim.z;
    int sides = 100;
    float r1 = this->bradius, r2 = this->tradius, l = this->length, theta = 2 * M_PI / sides, angle = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[9 * sides];
    for(int i = 0; i < sides; i++, angle += theta)
    {
        vertex_buffer_data[9 * i] = r1 * cosl(angle);
        vertex_buffer_data[9 * i + 1] = -l / 2;
        vertex_buffer_data[9 * i + 2] = r1 * sinl(angle);
        vertex_buffer_data[9 * i + 3] = r1 * cosl(angle + theta);
        vertex_buffer_data[9 * i + 4] = -l / 2;
        vertex_buffer_data[9 * i + 5] = r1 * sinl(angle + theta);
        vertex_buffer_data[9 * i + 6] = r2 * cosl(angle);
        vertex_buffer_data[9 * i + 7] = l / 2;
        vertex_buffer_data[9 * i + 8] = r2 * sinl(angle);
    }
    this->object = create3DObject(GL_TRIANGLES, sides*3, vertex_buffer_data, color, GL_FILL);
}

void Volcano::draw(glm::mat4 VP) {
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

void Volcano::set_position(glm::vec3 pos) {
    this->position = pos;
}

