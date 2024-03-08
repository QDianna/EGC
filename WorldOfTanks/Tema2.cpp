#pragma once
#include "lab_m1/Tema2/Tema2.h"
#include "Tank.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // *************************************** PLANE INIT *************************************** //
    float planeSize = 30;
    float planeHeight = -0.02f;
    glm::vec3 planeColor = glm::vec3(0.24f, 0, 0.32f);

    std::vector<VertexFormat> planeVertices = {
        VertexFormat(glm::vec3(-planeSize, planeHeight, -planeSize), planeColor),  // BL
        VertexFormat(glm::vec3(planeSize, planeHeight, -planeSize), planeColor),   // BR
        VertexFormat(glm::vec3(planeSize, planeHeight, planeSize), planeColor),    // TR
        VertexFormat(glm::vec3(-planeSize, planeHeight, planeSize), planeColor)    // TL
    };

    std::vector<unsigned int> planeIndices = {
        0, 1, 2,
        0, 2, 3
    };

    XOZplane = new Mesh("XOZplane");
    XOZplane->InitFromData(planeVertices, planeIndices);

    // ******************************* TANK COMPONENTS CENTERS ******************************* // 
    glm::vec3 bodyCenter(0, 0.2f, 0);  // center of tank body bottom face
    glm::vec3 turretCenter(0, 0.6f, 0);  // center of tank turret bottom face
    glm::vec3 treadsCenter(0, 0, 0);  // center of tank treads bottom face
    glm::vec3 cannonCenter(0, 0.7, 0.5);  // center of tank cannon - center of the cylinder

    // ************************************* MY TANK INIT ************************************* //
    int tankID = -1;  // -1 for my tank; 0, 1, 2 for enemy tanks
    // create tank body mesh
    Mesh* bodyMesh = myTank.tankBody.CreateBody("tankBody", bodyCenter, tankID);
    myTank.tankBody.mesh = bodyMesh;

    // create turret body mesh
    Mesh* turretMesh = myTank.tankTurret.CreateTurret("tankTurret", turretCenter, tankID);
    myTank.tankTurret.mesh = turretMesh;

    // create tank treads mesh
    Mesh* treadsMesh = myTank.tankTreads.CreateTreads("tankTreads", treadsCenter, tankID);
    myTank.tankTreads.mesh = treadsMesh;

    // create tank cannon mesh
    Mesh* cannonMesh = myTank.tankCannon.CreateCannon("tankCannon", cannonCenter, tankID);
    myTank.tankCannon.mesh = cannonMesh;


    // ************************************* ENEMY TANKS INIT ************************************* //
    /*Mesh* enemyTankBodyMesh = enemyTank.tankBody.CreateBody("tankBody", bodyCenter, 3);  // 0 for enemy tank just for now
    enemyTank.tankBody.mesh = enemyTankBodyMesh;
    enemyTank.tankBody.rotationAngle = -0.5f;*/

    //Tank enemy1, enemy2, enemy3;
    //enemyTanks.push_back(enemy1);
    //enemyTanks.push_back(enemy1);
    //enemyTanks.push_back(enemy1);
    for (int tankID = 0; tankID < 3; tankID ++) {
        Tank newEnemy;

        Mesh* bodyMesh = newEnemy.tankBody.CreateBody("tankBody", bodyCenter, tankID);
        newEnemy.tankBody.mesh = bodyMesh;

        Mesh* turretMesh = newEnemy.tankTurret.CreateTurret("tankTurret", turretCenter, tankID);
        newEnemy.tankTurret.mesh = turretMesh;

        Mesh* treadsMesh = newEnemy.tankTreads.CreateTreads("tankTreads", treadsCenter, tankID);
        newEnemy.tankTreads.mesh = treadsMesh;

        Mesh* cannonMesh = newEnemy.tankCannon.CreateCannon("tankCannon", cannonCenter, tankID);
        newEnemy.tankCannon.mesh = cannonMesh;

        enemyTanks.push_back(newEnemy);
    }

    // ************************************* BUILDINGS INIT ************************************* //
    for (int i = 0; i < 6; i++) {
        Tema2::Building building;
        building.buildingMesh = building.CreateBuilding("building", glm::vec3(0, 0, 0));
        buildings.push_back(building);
    }

    // ********************************** CAMERA INIT ********************************** //
    camera = new implemented_Tema2::Camera();

    /*glm::vec3 tankPosition = tankBody.coord;
    glm::vec3 cameraPosition = tankPosition;

    cout << "** CAMERA SETUP **";
    camera->Set(tankPosition, tankPosition, glm::vec3(0, 1, 0));*/

    // hardcodded projection parameters
    projectionType = 1;
    left = right = bottom = top = 0;
    zNear = 0.01f;
    zFar = 200.0f;
    fov = RADIANS(90);
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

    float cameraAngle = myTank.tankBody.rotationAngle;

    float cameraX = myTank.tankBody.coord.x - camera->distanceToTarget * sin(cameraAngle);
    float cameraZ = myTank.tankBody.coord.z - camera->distanceToTarget * cos(cameraAngle);

    camera->Set(glm::vec3(cameraX, 2.8f, cameraZ), myTank.tankBody.coord, glm::vec3(0, 1, 0));

}


std::chrono::steady_clock::time_point gameStartTime = std::chrono::steady_clock::now();
void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(currentTime - gameStartTime).count();

    // check if 1 minute has passed since the start of the game
    if (elapsedMinutes >= 1.) {
        cout << "***** G A M E    O V E R *****\n";
        gameRunning = false;
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
    // *********************************** CAMERA POSITIONING *********************************** //
    if (setCamera == true) {
        float cameraAngle = myTank.tankBody.rotationAngle;

        float cameraX = myTank.tankBody.coord.x - camera->distanceToTarget * sin(cameraAngle);
        float cameraZ = myTank.tankBody.coord.z - camera->distanceToTarget * cos(cameraAngle);

        camera->Set(glm::vec3(cameraX, 2.8f, cameraZ), myTank.tankBody.coord, glm::vec3(0, 1, 0));
    }

    glm::mat4 modelMatrix;
    
    // ************************************* TANK MOVEMENT ************************************* //
    if (movingW && gameRunning) {
        //cout << "moving forwards.... ";

        RenderMesh(myTank.tankBody.mesh, shaders["VertexColor"], myTank.tankBody.MoveW(deltaTimeSeconds));

        RenderMesh(myTank.tankTurret.mesh, shaders["VertexColor"], myTank.tankTurret.MoveW(deltaTimeSeconds));

        RenderMesh(myTank.tankTreads.mesh, shaders["VertexColor"], myTank.tankTreads.MoveW(deltaTimeSeconds));

        RenderMesh(myTank.tankCannon.mesh, shaders["VertexColor"], myTank.tankCannon.MoveW(deltaTimeSeconds));
    }

    else if (rotatingA && gameRunning) {
       // cout << "rotating left...... ";

        RenderMesh(myTank.tankBody.mesh, shaders["VertexColor"], myTank.tankBody.RotateA(deltaTimeSeconds));

        RenderMesh(myTank.tankTurret.mesh, shaders["VertexColor"], myTank.tankTurret.RotateA(deltaTimeSeconds));

        RenderMesh(myTank.tankTreads.mesh, shaders["VertexColor"], myTank.tankTreads.RotateA(deltaTimeSeconds));

        RenderMesh(myTank.tankCannon.mesh, shaders["VertexColor"], myTank.tankCannon.RotateA(deltaTimeSeconds));
    }

    else if (movingS && gameRunning) {
       // cout << "moving backwards... ";

        RenderMesh(myTank.tankBody.mesh, shaders["VertexColor"], myTank.tankBody.MoveS(deltaTimeSeconds));

        RenderMesh(myTank.tankTurret.mesh, shaders["VertexColor"], myTank.tankTurret.MoveS(deltaTimeSeconds));

        RenderMesh(myTank.tankTreads.mesh, shaders["VertexColor"], myTank.tankTreads.MoveS(deltaTimeSeconds));

        RenderMesh(myTank.tankCannon.mesh, shaders["VertexColor"], myTank.tankCannon.MoveS(deltaTimeSeconds));
    }

    else if (rotatingD && gameRunning) {
        //cout << "rotating right..... ";

        RenderMesh(myTank.tankBody.mesh, shaders["VertexColor"], myTank.tankBody.RotateD(deltaTimeSeconds));

        RenderMesh(myTank.tankTurret.mesh, shaders["VertexColor"], myTank.tankTurret.RotateD(deltaTimeSeconds));

        RenderMesh(myTank.tankTreads.mesh, shaders["VertexColor"], myTank.tankTreads.RotateD(deltaTimeSeconds));

        RenderMesh(myTank.tankCannon.mesh, shaders["VertexColor"], myTank.tankCannon.RotateD(deltaTimeSeconds));
    }

    else {
        // keeping tank in place
        RenderMesh(myTank.tankBody.mesh, shaders["VertexColor"], myTank.tankBody.Stay());
        RenderMesh(myTank.tankTurret.mesh, shaders["VertexColor"], myTank.tankTurret.Stay());
        RenderMesh(myTank.tankTreads.mesh, shaders["VertexColor"], myTank.tankTreads.Stay());
        RenderMesh(myTank.tankCannon.mesh, shaders["VertexColor"], myTank.tankCannon.Stay());
    }

    // ************************************* PROJECTILE COLLISION ************************************* //
    for (auto it = myTank.tankCannon.projectiles.begin(); it != myTank.tankCannon.projectiles.end();) {
        auto& proj = *it;

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - proj.projectileSpawnTime).count();

        bool deletedProj = false;

        float projX = proj.projectileCoords.x;
        float projZ = proj.projectileCoords.z;
        float projRadius = 0.1f;

        // check projectile colision with enemy tanks
        for (auto tit = enemyTanks.begin(); tit != enemyTanks.end() && !deletedProj;) {
            auto& tank = *tit;
            
            float tankX = tank.tankBody.coord.x;
            float tankZ = tank.tankBody.coord.z;

            if (!deletedProj && projX + projRadius >= tankX - 1 && projX <= tankX + 1 &&
                projZ + projRadius >= tankZ - 0.5f && projZ <= tankZ + 0.5f) {
                cout << "******** YOU'VE HIT AN ENEMY ********\n";
                //cout << "tank " << tank.tankBody.coord << " proj " << proj.projectileCoords << "\n";

                it = myTank.tankCannon.projectiles.erase(it);
                deletedProj = true;
            }
            tit++;
        }

        // check projectile colision with buildings
        for (auto bit = buildings.begin(); bit != buildings.end() && !deletedProj;) {
            auto& bld = *bit;
            float bldX = bld.translateX;
            float bldZ = bld.translateZ;

            if (projX + projRadius >= bld.translateX - bld.width / 2 && projX <= bld.translateX + bld.width / 2 &&
                projZ + projRadius >= bld.translateX - bld.length / 2 && bld.translateX + bld.length / 2) {
                cout << "******** YOU'VE HIT A BUILDING ********\n";

                it = myTank.tankCannon.projectiles.erase(it);
                deletedProj = true;
            }
            bit++;
        }
        
        if (!deletedProj  && elapsedSeconds >= 4) {
            it = myTank.tankCannon.projectiles.erase(it);
            deletedProj = true;

        } else if (!deletedProj) {
            RenderMesh(proj.projectileMesh, shaders["VertexColor"], myTank.tankCannon.MoveProjectile(deltaTimeSeconds, proj));
            it++;
        }
    }

    // ************************************ BUILDINGS RENDERING ************************************ //
    for (auto it = buildings.begin(); it != buildings.end();) {
        auto& building = *it;
        glm::vec3 translatePos = glm::vec3(building.translateX, 0, building.translateZ);

        modelMatrix = glm::mat4(1);
        modelMatrix *= glm::translate(glm::mat4(1), translatePos);

        RenderMesh(building.buildingMesh, shaders["VertexColor"], modelMatrix);
        it++;
    }

    // ************************************ RENDER ENEMYS ************************************ //

    // make tanks go forwards and backwards in a straight line
    for (auto tit = enemyTanks.begin(); tit != enemyTanks.end();) {
        auto& t = *tit;

        if (t.tankBody.distanceTraveled >= 4) {
            //out << "must go back now\n";
            t.enemyDirection *= -1;
            t.tankBody.distanceTraveled = 0;
        }

        tit++;
    }

    for (int i = 0; i < enemyTanks.size(); i++) {
        RenderMesh(enemyTanks[i].tankBody.mesh, shaders["VertexColor"], enemyTanks[i].tankBody.MoveEnemy(enemyTanks[i].enemyDirection * deltaTimeSeconds));
        RenderMesh(enemyTanks[i].tankTurret.mesh, shaders["VertexColor"], enemyTanks[i].tankTurret.MoveEnemy(enemyTanks[i].enemyDirection* deltaTimeSeconds));
        RenderMesh(enemyTanks[i].tankTreads.mesh, shaders["VertexColor"], enemyTanks[i].tankTreads.MoveEnemy(enemyTanks[i].enemyDirection* deltaTimeSeconds));
        RenderMesh(enemyTanks[i].tankCannon.mesh, shaders["VertexColor"], enemyTanks[i].tankCannon.MoveEnemy(enemyTanks[i].enemyDirection* deltaTimeSeconds));  
    }

    // ************************************ RENDER PLANE ************************************ //
    modelMatrix = glm::mat4(1);
    RenderMesh(XOZplane, shaders["VertexColor"], modelMatrix);

}


void Tema2::FrameEnd()
{
     DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // start tank movement
    if (key == GLFW_KEY_W && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        //cout << "started moving forwards\n";
        movingW = true;

    }

    if (key == GLFW_KEY_A && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        //cout << "started rotating left\n";
        rotatingA = true;
    }

    if (key == GLFW_KEY_S && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        //cout << "started moving backwards\n";
        movingS = true;
    }

    if (key == GLFW_KEY_D && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        //cout << "started rotating right\n";
        rotatingD = true;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // stop tank movement
    if (key == GLFW_KEY_W && movingW) {
      //  cout << "stopped moving forwards\n";
        movingW = false;
    }

    if (key == GLFW_KEY_A && rotatingA) {
       // cout << "stopped rotating left\n";
        rotatingA = false;
    }

    if (key == GLFW_KEY_S && movingS) {
       // cout << "stopped moving backwards\n";
        movingS = false;
    }

    if (key == GLFW_KEY_D && rotatingD) {
        //cout << "stopped rotating right\n";
        rotatingD = false;
    }
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensitivityOX = 0.001f;
    float sensitivityOY = 0.001f;

    // rotate the camera position around the tark with mouse
    if (setCamera == false)
    {
        // OY rotation
        cameraAngle -= sensitivityOY * deltaX;
        // OX rotation
        cameraPitch -= sensitivityOX * deltaY;

        // limit angle
        float minPitch = glm::radians(0.0f);
        float maxPitch = glm::radians(60.0f);
        cameraPitch = glm::clamp(cameraPitch, minPitch, maxPitch);

        float cameraX = myTank.tankBody.coord.x - camera->distanceToTarget * sin(cameraAngle);
        float cameraY = myTank.tankBody.coord.y + camera->distanceToTarget * sin(cameraPitch);
        float cameraZ = myTank.tankBody.coord.z - camera->distanceToTarget * cos(cameraAngle);

        camera->Set(glm::vec3(cameraX, cameraY, cameraZ), myTank.tankBody.coord, glm::vec3(0, 1, 0));
    }
}


std::chrono::steady_clock::time_point lastFireTime = std::chrono::steady_clock::now();
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_3) {
        //cout << "pressed RMB";
        setCamera = false;
        cameraAngle = myTank.tankBody.rotationAngle;
        cameraPitch = 1;
    }

    if (button == GLFW_MOUSE_BUTTON_2 && gameRunning) {
        //cout << "pressed LMB\n";
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastFireTime).count();

        if (elapsedSeconds >= 1) {
            cout << "Firing...\n";

            firing = true;
            Tank::TankCannon::Projectile newProjectile;
            newProjectile.projectileMesh = myTank.tankCannon.CreateProjectile("projectile", myTank.tankTurret.coord);

            newProjectile.projectileCoords = myTank.tankTurret.coord + glm::vec3(
                1.5f * std::sin(myTank.tankCannon.rotationAngle),
                0,
                1.5f * std::cos(myTank.tankCannon.rotationAngle)
            );

            newProjectile.projectileAngle = myTank.tankTurret.rotationAngle;
            newProjectile.projectileSpawnTime = currentTime;

            myTank.tankCannon.projectiles.push_back(newProjectile);
            firing = false;

            lastFireTime = currentTime;
        }
        else {
            cout << "You're on cooldown!\n";
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_3) {
        //cout << "RMB released\n";
        setCamera = true;
    }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

Mesh* Tema2::Building::CreateBuilding(const std::string& name, glm::vec3 center)
{
    cout << "Generating random building...\n";
    float R = std::rand() % (1 - 0 + 1) + 0;
    float G = std::rand() % (1 - 0 + 1) + 0;
    float B = std::rand() % (1 - 0 + 1) + 0;
    glm::vec3 buildingColor(R, G, B);
    
    width = std::rand() % (4 - 2 + 1) + 2;
    float halfWidth = width / 2;

    length = std::rand() % (5 - 2 + 1) + 2;
    float halfDepth = length / 2;

    height = std::rand() % (5 - 2 + 1) + 2;

    if (std::rand() % 2 == 0) {
        translateX = - std::rand() % 20 - 6;  // if even, generate a value between -25 and -2
    }
    else {
        translateX = std::rand() % 20 + 6;  // if odd, generate a value between 2 and 25
    }

    if (std::rand() % 2 == 0) {
        translateZ = -std::rand() % 20 - 6;  // if even, generate a value between -25 and -2
    }
    else {
        translateZ = std::rand() % 20 + 6;  // If odd, generate a value between 2 and 25
    }
    // initialize tank turret current coords
    // coord = center;

    std::vector<VertexFormat> vertices = {
        // bottom face
        VertexFormat(center + glm::vec3(-halfWidth, 0, -halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, -halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(halfWidth, 0, halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(-halfWidth, 0, halfDepth), buildingColor),
        // top face
        VertexFormat(center + glm::vec3(-halfWidth, height, -halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(halfWidth, height, -halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(halfWidth, height, halfDepth), buildingColor),
        VertexFormat(center + glm::vec3(-halfWidth, height, halfDepth), buildingColor)
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
