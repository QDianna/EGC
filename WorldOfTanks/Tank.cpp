#include "Tank.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>

using namespace std;


Tank::Tank()
{
}


Tank::~Tank()
{
}


// ------------------------------------- (WHOLE) TANK ------------------------------------- //
float Tank::tankSpeed = 2.0f;
float Tank::rotationSpeed = 1.5f;
glm::vec3 enemyRandomCoord;


// --------------------------------------- TANK BODY --------------------------------------- //
Mesh* Tank::TankBody::CreateBody(const std::string& name, glm::vec3 center, int tankID)
{
    glm::vec3 bodyColor;

    if (tankID == -1) {  // my tank
        cout << "Creating my tank...\n";
        // initialize tank body current coords
        coord = center;
        bodyColor = glm::vec3(0.6f, 0.125f, 0);
    }
    else {  // enemy tanks
        cout << "Creating enemy's tank...\n";
        bodyColor = glm::vec3(0, 0.32f, 0.37f);

        float x, z;
        if (tankID == 0) {
            x = -4;
            z = -4;
            rotationAngle = 3.14f;
        }
        if (tankID == 1) {
            x = 5;
            z = 4.5f;
            rotationAngle = 1.57f;
        }
        if (tankID == 2) {
            x = -5;
            z = 5.5f;
            rotationAngle = 0;
        }

        spawnCoord = glm::vec3(x, 0, z);  // spawn coords (dont change)
        coord = spawnCoord;  // current coords = spawn coords (change when moving)
    }

    float halfWidth = 0.5f;
    float halfDepth = 1.0f;
    float height = 0.4f;


    std::vector<VertexFormat> vertices = {
        // bottom face
        VertexFormat(center + glm::vec3(-halfWidth, 0, -halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, -halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(-halfWidth, 0, halfDepth), bodyColor),
        // top face
        VertexFormat(center + glm::vec3(-halfWidth, height, -halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(halfWidth, height, -halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(halfWidth, height, halfDepth), bodyColor),
        VertexFormat(center + glm::vec3(-halfWidth, height, halfDepth), bodyColor)
    };

    Mesh* base = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3,  // bottom face
                                          4, 5, 6, 4, 6, 7,  // top face
                                          0, 4, 5, 0, 5, 1,  // backwards face
                                          2, 3, 7, 2, 7, 6,  // forwards face
                                          0, 7, 3, 0, 4, 7,  // left face
                                          6, 2, 1, 1, 5, 6   // right face
                                        };

    base->InitFromData(vertices, indices);
    return base;
}


glm::mat4 Tank::TankBody::MoveW(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankBody::RotateA(float time)
{
    rotationAngle += time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankBody::MoveS(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, - tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));


    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankBody::RotateD(float time)
{
    rotationAngle -= time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankBody::Stay()
{
    // keep tank in place
    glm::vec3 translateXZ = glm::vec3(coord.x, 0, coord.z);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), translateXZ);
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

     return modelMatrix;
}


// ------------------------------------- TANK TURRET ------------------------------------- //
Mesh* Tank::TankTurret::CreateTurret(const std::string& name, glm::vec3 center, int tankID)
{
    glm::vec3 turretColor;

    if (tankID == -1) {  // my tank
       // cout << "Creating my tank's turret...\n";
        turretColor = glm::vec3(0.878f, 0.514, 0);
        // initialize tank turret current coords
        coord = center;
    }
    else {  // enemy tanks
      //  cout << "Creating enemy tank's turret...\n";
        turretColor = glm::vec3(0, 0.89, 1);

        float x, z;
        if (tankID == 0) {
            x = -4;
            z = -4;
            rotationAngle = 3.14f;
        }
        if (tankID == 1) {
            x = 5;
            z = 4.5f;
            rotationAngle = 1.57f;
        }
        if (tankID == 2) {
            x = -5;
            z = 5.5f;
            rotationAngle = 0;
        }

        spawnCoord = glm::vec3(x, 0, z);  // spawn coords (dont change)
        coord = spawnCoord;  // current coords = spawn coords (change when moving)
    }

    float halfWidth = 0.33f;
    float halfDepth = 0.5f;
    float height = 0.2f;


    std::vector<VertexFormat> vertices = {
        // bottom face
        VertexFormat(center + glm::vec3(-halfWidth, 0, -halfDepth), turretColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, -halfDepth), turretColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, halfDepth), turretColor),
        VertexFormat(center + glm::vec3(-halfWidth, 0, halfDepth), turretColor),
        // top face
        VertexFormat(center + glm::vec3(-halfWidth, height, -halfDepth), turretColor),
        VertexFormat(center + glm::vec3(halfWidth, height, -halfDepth), turretColor),
        VertexFormat(center + glm::vec3(halfWidth, height, halfDepth), turretColor),
        VertexFormat(center + glm::vec3(-halfWidth, height, halfDepth), turretColor)
    };

    Mesh* turret = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3,  // bottom face
                                          4, 5, 6, 4, 6, 7,  // top face
                                          0, 4, 5, 0, 5, 1,  // backwards face
                                          2, 3, 7, 2, 7, 6,  // forwards face
                                          0, 7, 3, 0, 4, 7,  // left face
                                          6, 2, 1, 1, 5, 6   // right face
    };

    turret->InitFromData(vertices, indices);
    return turret;
}


glm::mat4 Tank::TankTurret::MoveW(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankTurret::RotateA(float time)
{
    rotationAngle += time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankTurret::MoveS(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, -tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));


    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankTurret::RotateD(float time)
{
    rotationAngle -= time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankTurret::Stay()
{
    // keep tank in place
    glm::vec3 translateXZ = glm::vec3(coord.x, 0, coord.z);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), translateXZ);
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


// ------------------------------------- TANK TREADS ------------------------------------- //
Mesh* Tank::TankTreads::CreateTreads(const std::string& name, glm::vec3 center, int tankID)
{
    glm::vec3 treadsColor;
    if (tankID == -1) {  // my tank
       // cout << "Creating my tank's treads...\n";
        treadsColor = glm::vec3(1, 0.969f, 0.922f);
        // initialize tank treads current coords
        coord = center;
    }
    else {  // enemy tanks
      //  cout << "Creating enemy tank's treads...\n";
        treadsColor = glm::vec3(0.81, 0.99, 1);

        float x, z;
        if (tankID == 0) {
            x = -4;
            z = -4;
            rotationAngle = 3.14f;
        }
        if (tankID == 1) {
            x = 5;
            z = 4.5f;
            rotationAngle = 1.57f;
        }
        if (tankID == 2) {
            x = -5;
            z = 5.5f;
            rotationAngle = 0;
        }

        spawnCoord = glm::vec3(x, 0, z);  // spawn coords (dont change)
        coord = spawnCoord;  // current coords = spawn coords (change when moving)
    }

    float halfWidth = 0.083f;
    float halfDepth = 1;
    float treadDistancing = 0.4f;
    float height = 0.2f;

    std::vector<VertexFormat> vertices = {
        // left tread - bottom face
        VertexFormat(center + glm::vec3(-halfWidth - treadDistancing, 0, -halfDepth), treadsColor),       // 0
        VertexFormat(center + glm::vec3(halfWidth - treadDistancing, 0, -halfDepth), treadsColor),        // 1
        VertexFormat(center + glm::vec3(halfWidth - treadDistancing, 0, halfDepth), treadsColor),         // 2
        VertexFormat(center + glm::vec3(-halfWidth - treadDistancing, 0, halfDepth), treadsColor),        // 3
        // left tread - top face
        VertexFormat(center + glm::vec3(-halfWidth - treadDistancing, height, -halfDepth), treadsColor),  // 4
        VertexFormat(center + glm::vec3(halfWidth - treadDistancing, height, -halfDepth), treadsColor),   // 5
        VertexFormat(center + glm::vec3(halfWidth - treadDistancing, height, halfDepth), treadsColor),    // 6
        VertexFormat(center + glm::vec3(-halfWidth - treadDistancing, height, halfDepth), treadsColor),   // 7
        // right tread - bottom face
        VertexFormat(center + glm::vec3(-halfWidth + treadDistancing, 0, -halfDepth), treadsColor),       // 8  = 0
        VertexFormat(center + glm::vec3(halfWidth + treadDistancing, 0, -halfDepth), treadsColor),        // 9  = 1
        VertexFormat(center + glm::vec3(halfWidth + treadDistancing, 0, halfDepth), treadsColor),         // 10 = 2
        VertexFormat(center + glm::vec3(-halfWidth + treadDistancing, 0, halfDepth), treadsColor),        // 11 = 3
        // right tread - top face
        VertexFormat(center + glm::vec3(-halfWidth + treadDistancing, height, -halfDepth), treadsColor),  // 12 = 4
        VertexFormat(center + glm::vec3(halfWidth + treadDistancing, height, -halfDepth), treadsColor),   // 13 = 5
        VertexFormat(center + glm::vec3(halfWidth + treadDistancing, height, halfDepth), treadsColor),    // 14 = 6
        VertexFormat(center + glm::vec3(-halfWidth + treadDistancing, height, halfDepth), treadsColor)    // 15 = 7
    };

    Mesh* treads = new Mesh(name);
    std::vector<unsigned int> indices = { 
                                         // left tread
                                          0, 1, 2, 0, 2, 3,  // bottom face
                                          4, 5, 6, 4, 6, 7,  // top face
                                          0, 4, 5, 0, 5, 1,  // backwards face
                                          2, 3, 7, 2, 7, 6,  // forwards face
                                          0, 7, 3, 0, 4, 7,  // left face
                                          6, 2, 1, 1, 5, 6,  // right face
                                          // right tread
                                          8, 9, 10, 8, 10, 11,
                                          12, 13, 14, 12, 14, 15,
                                          8, 12, 13, 8, 13, 9,
                                          10, 11, 15, 10, 15, 14,
                                          8, 15, 11, 8, 12, 15,
                                          14, 10, 9, 9, 13, 14
    };

    treads->InitFromData(vertices, indices);
    return treads;
}


glm::mat4 Tank::TankTreads::MoveW(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankTreads::RotateA(float time)
{
    rotationAngle += time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankTreads::MoveS(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, -tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));


    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankTreads::RotateD(float time)
{
    rotationAngle -= time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankTreads::Stay()
{
    // keep tank in place
    glm::vec3 translateXZ = glm::vec3(coord.x, 0, coord.z);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), translateXZ);
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


// ------------------------------------- TANK CANNON ------------------------------------- //
Mesh* Tank::TankCannon::CreateCannon(const std::string& name, glm::vec3 center, int tankID)
{
    glm::vec3 cannonColor;
    if (tankID == -1) {  // my tank
       // cout << "Creating my tank's cannon...\n";
        cannonColor = glm::vec3(1, 0.969f, 0.922f);
    }
    else {  // enemy tanks
        //cout << "Creating enemy tank's cannon...\n";
        cannonColor = glm::vec3(0.81, 0.99, 1);

        float x, z;
        if (tankID == 0) {
            x = -4;
            z = -4;
            rotationAngle = 3.14f;
        }
        if (tankID == 1) {
            x = 5;
            z = 4.5f;
            rotationAngle = 1.57f;
        }
        if (tankID == 2) {
            x = -5;
            z = 5.5f;
            rotationAngle = 0;
        }

        spawnCoord = glm::vec3(x, 0, z);  // spawn coords (dont change)
        coord = spawnCoord;  // current coords = spawn coords (change when moving)
    }

    int numSegments = 40;
    float length = 2;
    float radius = 0.06f;

    std::vector<VertexFormat> vertices;
    // create vertices for the side of the cylinder
    for (int i = 0; i < numSegments; i ++) {
        float theta = static_cast<float>(i) / numSegments * glm::two_pi<float>();
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);

        // side vertices (oriented horizontally, parallel with OZ)
        vertices.emplace_back(center + glm::vec3(x, y, length * 0.5f), cannonColor);
        vertices.emplace_back(center + glm::vec3(x, y, -length * 0.5f), cannonColor);
    }

    // add first 2 vertices to close the cylinder
    vertices.push_back(vertices[0]);
    vertices.push_back(vertices[1]);

    // create vertices for the top and bottom faces of the cylinder
    for (int i = 0; i < numSegments; i ++) {
        float theta = static_cast<float>(i) / numSegments * glm::two_pi<float>();
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);

        // top face vertex
        vertices.emplace_back(center + glm::vec3(x, y, length * 0.5f), cannonColor);

        // bottom face vertex
        vertices.emplace_back(center + glm::vec3(x, y, -length * 0.5f), cannonColor);
    }

    Mesh* cannon = new Mesh(name);
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < numSegments * 2; i += 2)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 1);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
    }

    cannon->InitFromData(vertices, indices);
    return cannon;
}


glm::mat4 Tank::TankCannon::MoveW(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankCannon::RotateA(float time)
{
    rotationAngle += time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankCannon::MoveS(float time)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, -tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));


    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankCannon::RotateD(float time)
{
    rotationAngle -= time * rotationSpeed;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(coord.x, 0, coord.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


glm::mat4 Tank::TankCannon::Stay()
{
    // keep tank in place
    glm::vec3 translateXZ = glm::vec3(coord.x, 0, coord.z);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(glm::mat4(1), translateXZ);
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    return modelMatrix;
}


// ------------------------------------- PROJECTILE ------------------------------------- //
Mesh* Tank::TankCannon::CreateProjectile(const std::string& name, glm::vec3 center)
{
   // cout << "Creating projectile...\n";
    glm::vec3 projectileColor(0.6f, 0.125f, 0);

    int numStacks = 40;
    int numSlices = 40;
    float radius = 0.1f;

    // initialize coord and angle
    
    Mesh* projectile = new Mesh(name);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i <= numStacks; ++i) {
        float phi = glm::pi<float>() * static_cast<float>(i) / numStacks;
        for (int j = 0; j <= numSlices; ++j) {
            float theta = 2.0f * glm::pi<float>() * static_cast<float>(j) / numSlices;

            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::sin(phi) * std::sin(theta);
            float z = radius * std::cos(phi);

            vertices.emplace_back(glm::vec3(0, 0.7f, 0) + glm::vec3(x, y, z), projectileColor);

            if (i < numStacks && j < numSlices) {
                unsigned int current = i * (numSlices + 1) + j;
                unsigned int next = current + numSlices + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(next);
                indices.push_back(next + 1);
                indices.push_back(current + 1);
            }
        }
    }

    projectile->InitFromData(vertices, indices);
    return projectile;
}

glm::mat4 Tank::TankCannon::MoveProjectile(float time, Projectile &projectile)
{
    // keep projectile moving forward
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0, 0, projectileSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), projectile.projectileAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = projectile.projectileCoords + rotatedForward;
    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), projectile.projectileAngle, glm::vec3(0, 1, 0));

    projectile.projectileCoords = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankBody::MoveEnemy(float time)
{
    //cout << "moving enemy\n";
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0);
    // translate to random location
    // modelMatrix *= glm::translate(glm::mat4(1), coord);

    forward = glm::vec3(0, 0, 0.45f * tankSpeed * time);


    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    distanceTraveled += glm::length(rotatedForward);

    glm::vec3 newCoords = coord + rotatedForward;
    //cout << "new enemy coords: " << newCoords << "\n";

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}


glm::mat4 Tank::TankTurret::MoveEnemy(float time)
{
    //cout << "moving enemy\n";
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0);
    // translate to random location
    // modelMatrix *= glm::translate(glm::mat4(1), coord);

    forward = glm::vec3(0, 0, 0.45f * tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;
    //cout << "new enemy coords: " << newCoords << "\n";

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}

glm::mat4 Tank::TankTreads::MoveEnemy(float time)
{
    //cout << "moving enemy\n";
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0);
    // translate to random location
    // modelMatrix *= glm::translate(glm::mat4(1), coord);

    forward = glm::vec3(0, 0, 0.45f * tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;
    //cout << "new enemy coords: " << newCoords << "\n";

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}

glm::mat4 Tank::TankCannon::MoveEnemy(float time)
{
    //cout << "moving enemy\n";
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 forward = glm::vec3(0);
    // translate to random location
    // modelMatrix *= glm::translate(glm::mat4(1), coord);

    forward = glm::vec3(0, 0, 0.45f * tankSpeed * time);

    // rotate forward vector with rotateAngle (get movement orientation)
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));
    glm::vec3 rotatedForward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));

    glm::vec3 newCoords = coord + rotatedForward;
    //cout << "new enemy coords: " << newCoords << "\n";

    modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(newCoords.x, 0, newCoords.z));
    modelMatrix *= glm::rotate(glm::mat4(1), rotationAngle, glm::vec3(0, 1, 0));

    coord = newCoords;

    return modelMatrix;
}