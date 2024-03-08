#include "rectangle.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Rectangle::Rectangle()
{
}


Rectangle::~Rectangle()
{
}

Mesh* Rectangle::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
       VertexFormat(corner, color),
       VertexFormat(corner + glm::vec3(width, 0, 0), color),
       VertexFormat(corner + glm::vec3(width, length, 0), gGlow),
       VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}
