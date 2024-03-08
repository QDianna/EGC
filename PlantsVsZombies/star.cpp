#include "star.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Star::Star()
{
    radius = 100 * 0.4f;
    speed = 200;
    rotationSpeed = 5;
    
    centerX = 0;
    centerY = 0;
    translateX = 0;
    translateY = 0;
    scaleX = 1;
    scaleY = 1;
    angularStep = 0;
}

Star::Star(float sX, float sY)
{
    // constructor used for spawning projectile at coord (sX, sY)
    radius = 100 * 0.4f;
    speed = 200;
    rotationSpeed = 5;

    spawnX = currentX = sX;
    spawnY = currentY = sY;

    centerX = 0;
    centerY = 0;
    translateX = 0;
    translateY = 0;
    scaleX = 1;
    scaleY = 1;
    angularStep = 0;
}


Star::~Star()
{
}

void Star::Update(
    float time)
{
    translateX += speed * time;
    currentX += speed * time;
    angularStep += rotationSpeed * time;
}

glm::mat3 Star::Spawn(
    float x,
    float y)
{
    glm::mat3 modelMatrix = glm::mat3(1);

    // stea spawnata de rombul de pe patratul x, y din grid
    modelMatrix *= transform2D::Translate(x + radius, y);
    modelMatrix *= transform2D::Translate(translateX, 0);

    modelMatrix *= transform2D::Translate(centerX, centerY);
    modelMatrix *= transform2D::Rotate(-angularStep);
    modelMatrix *= transform2D::Translate(-centerX, -centerY);

      

    return modelMatrix;
}

glm::mat3 Star::GuiSpawn(
    float resx,
    float resy)
{
    glm::mat3 modelMatrix = glm::mat3(1);

    // stea spawnata random pe mapa
    int xmax = static_cast<unsigned int>(resx);
    int ymax = static_cast<unsigned int>(resy);
    int margin = 100;

    spawnX = std::rand() % (xmax - margin + 1) + margin;
    spawnY = std::rand() % (ymax - margin + 1) + margin;

    modelMatrix *= transform2D::Translate(spawnX, spawnY);

    return modelMatrix;
}


Mesh* Star::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numPoints = 5;
    float outerRadius = radius;
    float innerRadius = radius / 2;

    // Calculate rotation angle to align the star vertically
    float rotationAngle = RADIANS(18.0f);  // Adjust the angle as needed

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 3), wGlow));

    // Calculate vertices for the star
    for (int i = 0; i < numPoints * 2; ++i) {
        float currentAngle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numPoints * 2);
        float rotatedAngle = currentAngle + rotationAngle;

        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        glm::vec3 vertex = center + glm::vec3(radius * cos(rotatedAngle), radius * sin(rotatedAngle), 3.0f);
        vertices.push_back(VertexFormat(vertex, color));
    }

    // Create indices for triangles
    for (int i = 1; i < numPoints * 2 + 1; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Connect the last and first vertices to close the shape
    indices.push_back(0);
    indices.push_back(numPoints * 2);
    indices.push_back(1);

    Mesh* star = new Mesh(name);

    if (!fill) {
        star->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        star->SetDrawMode(GL_TRIANGLES);
    }

    star->InitFromData(vertices, indices);
    return star;
}