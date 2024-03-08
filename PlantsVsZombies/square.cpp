#include "square.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Square::Square()
{
    length = 100;
    spacing = 30;
    translateX = 0;
    translateY = 0;
}


Square::~Square()
{
}

glm::mat3 Square::gridPlacement(
    int row,
    int col,
    float gridBottomLeftX,
    float gridBottomLeftY)
{
    glm::mat3 modelMatrix = glm::mat3(1);

    translateX = row * length + row * spacing + gridBottomLeftX;
    translateY = col * length + col * spacing + gridBottomLeftY;

    modelMatrix *= transform2D::Translate(translateX, translateY);

    return modelMatrix;
}

Mesh* Square::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), gGlow),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
