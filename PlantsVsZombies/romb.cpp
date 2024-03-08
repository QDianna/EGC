#include "romb.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Romb::Romb()
{
    centerX = 0;
    centerY = 0;
    radius = 100 * 0.4f;
    scaleX = 1;
    scaleY = 1;

    fireing = false;

}

Romb::~Romb()
{
}

glm::mat3 Romb::Spawn(
    float x,
    float y)
{
    glm::mat3 modelMatrix = glm::mat3(1);

    // hexagon spawnat pe linia x
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);

    return modelMatrix;
}

Mesh* Romb::CreateRomb(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    float d1 = sin(RADIANS(60)) * length;  // jum din diagonala verticala
    float d2 = length * 0.5f;  // jum din diagonala orizontala

    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-d2, 0, 1), color), //                                         0
        VertexFormat(center + glm::vec3(0, -d1, 1), color), //                                         1
        VertexFormat(center + glm::vec3(d2 / 2, -d1 / 2, 1), color), //                                2
        VertexFormat(center + glm::vec3(d2 * 2, -d1 / 2, 1), color), //                                3
        VertexFormat(center + glm::vec3(d2, 0, 1), wGlow), //                                          4
        VertexFormat(center + glm::vec3(d2 * 2, d1 / 2, 1), color), //                                 5
        VertexFormat(center + glm::vec3(d2 / 2, d1 / 2, 1), color), //                                 6
        VertexFormat(center + glm::vec3(0, d1, 1), color),  //                                         7
    };

    std::vector<unsigned int> indices;
    Mesh* romb = new Mesh(name);

    if (!fill) {
        indices = { 0, 1, 2, 3, 5, 6, 7 };
        romb->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices = { 0, 1, 4,
                    0, 4, 7,
                    2, 3, 4,
                    5, 4, 3,
                    4, 5, 6 };
        romb->SetDrawMode(GL_TRIANGLES);
    }


    romb->InitFromData(vertices, indices);
    return romb;

}