#include "hexagon.h"
#include "lab_m1/Tema1/transform2D.h"


#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Hexagon::Hexagon()
{
    radius = 100 * 0.4f;
    speed = 100;

    centerX = 0;
    centerY = 0;
    translateX = 0;
    scaleX = 1;
    scaleY = 1;
    life = 3;
   
}

Hexagon::Hexagon(float sX, float sY)
{
    // constructor used for spawning enemy hexagon at randomised coord (sX, sY)
    radius = 100 * 0.4f;
    speed = 100;
    
    currentX = spawnX = sX;
    currentY = spawnY = sY;

    centerX = 0;
    centerY = 0;
    translateX = 0;
    scaleX = 1;
    scaleY = 1;
    life = 3;

}

Hexagon::~Hexagon()
{
}


void Hexagon::Update(
    float time)
{
    translateX -= speed * time;
    currentX -= speed * time;

}

glm::mat3 Hexagon::Spawn(
    float x,
    float y)
{
    glm::mat3 modelMatrix = glm::mat3(1);

    // hexagon spawnat pe linia y
    
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Translate(translateX, 0);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);

    return modelMatrix;
}

Mesh* Hexagon::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments,
    bool fill)
{
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    centerX = center[0];
    centerY = center[1];
    // add a center vertex
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 1.5f), wGlow));
    indices.push_back(0);

    // Calculate vertices around the circle
    for (int i = 0; i < numSegments; i++) {
        float theta = (360 / numSegments) * i;
        glm::vec3 vertex = center + glm::vec3(radius * cos(RADIANS(theta)), radius * sin(RADIANS(theta)), 1.5f);
        vertices.push_back(VertexFormat(vertex, color));

        // Connect the vertices to form triangles
        indices.push_back(i + 1);
    }


    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // add last triangle vertex
        indices.push_back(1);
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}



Mesh* Hexagon::CreateInsideCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments,
    bool fill)
{
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    centerX = center[0];
    centerY = center[1];
    // add a center vertex
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 2), wGlow));
    indices.push_back(0);

    // Calculate vertices around the circle
    for (int i = 0; i < numSegments; i++) {
        float theta = (360 / numSegments) * i;
        glm::vec3 vertex = center + glm::vec3(radius * cos(RADIANS(theta)), radius * sin(RADIANS(theta)), 2.0f);
        vertices.push_back(VertexFormat(vertex, color));

        // Connect the vertices to form triangles
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // add last triangle vertex
        indices.push_back(1);
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

