#include "plane.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Plane::Plane(glm::vec3 pos, glm::vec3 dim, color_t color) {
    this->position = pos;
    this->rx = this->ry = this->rz = this->speed = 0;
    float length = dim.x, width = dim.y, depth = dim.z;
    this->l = dim.x; this->b = dim.y; this->h = dim.z;
    this->fuel = 80;
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
        
        -length / 2, 0, -depth / 4, //left wing
        -3 * length / 2, 0, -depth / 4,
        -length /2 , 0, 0,

        length / 2, 0, -depth / 4, //right wing
        3 * length / 2, 0, -depth / 4,
        length / 2, 0, 0,

        length / 2, width / 2, -depth / 2, //head
        length / 2, -width / 2, -depth / 2,
        0, 0, -3 * depth / 4,
        length / 2, width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        0, 0, -3 * depth / 4,
        length / 2, -width / 2, -depth / 2,
        -length / 2, -width / 2, -depth / 2,
        0, 0, -3 * depth / 4,
        -length / 2, -width / 2, -depth / 2,
        -length / 2, width / 2, -depth / 2,
        0, 0, -3 * depth / 4,

        0, width / 2, depth / 2, //tail
        0, 7 * width / 3, depth / 2,
        0, width / 2, depth / 4,
    };
    this->object = create3DObject(GL_TRIANGLES, 19*3, vertex_buffer_data, color, GL_FILL);
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rx * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    rotate    = glm::rotate((float) (this->ry * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (rotate);
    rotate    = glm::rotate((float) (this->rz * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plane::tick(int sn) {
    // this->rotation += 10 * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
    if(this->fuel < 0)
        this->fuel = 0;
    if(sn == 1)
    {
        //forward
        float angle = this -> ry;
        this -> position.z -= 0.1 * cos(M_PI * angle / 180);
        this -> position.x -= 0.1 * sin(M_PI * angle / 180);
        this -> speed = 0.1;
    }
    else if(sn == -1)
    {
        //backward
        float angle = this -> ry;
        this -> position.z += 0.1 * cos(M_PI * angle / 180);
        this -> position.x += 0.1 * sin(M_PI * angle / 180);
        this -> speed = 0.1;
    }
    else if(sn == 2)
    {
        //up
        this -> position.y += 0.1;
    }
    else if(sn == -2)
    {
        //down
        this->position.y -= 0.1;
    }
    else
    {
        this -> speed -= 0.01;
        if(this->speed < 0)
            this -> speed = 0;
    }
}

