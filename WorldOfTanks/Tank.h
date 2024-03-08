#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include <chrono>

#include "components/simple_scene.h"


class Tank
{
public:
    Tank();
    ~Tank();

    static float tankSpeed;
    static float rotationSpeed;

    int enemyDirection = 1;  // 1 for forwards, -1 for backwards

    class TankBody {
    public:
        Mesh* mesh;

        float rotationAngle = 0;

        glm::vec3 coord = glm::vec3(0);  // current coords of center of bottom face
        glm::vec3 spawnCoord = glm::vec3(0);  // random spawn coords for enemy tanks
        float distanceTraveled = 0;  // dist traveled by the enemy before it changes direction

        Mesh* CreateBody(const std::string& name,
                         glm::vec3 center,
                         int tankID);

        glm::mat4 MoveW(float time);

        glm::mat4 MoveS(float time);

        glm::mat4 RotateA(float time);

        glm::mat4 RotateD(float time);

        glm::mat4 Stay();

        glm::mat4 MoveEnemy(float time);

    };

    class TankTurret {
    public:
        Mesh* mesh;

        float rotationAngle = 0;
        glm::vec3 coord = glm::vec3(0);  // current coords of center of bottom face
        glm::vec3 spawnCoord = glm::vec3(0);  // random spawn coords for enemy tanks


        Mesh* CreateTurret(const std::string& name,
                           glm::vec3 center,
                           int tankID);

        glm::mat4 MoveW(float time);

        glm::mat4 MoveS(float time);

        glm::mat4 RotateA(float time);

        glm::mat4 RotateD(float time);

        glm::mat4 Stay();

        glm::mat4 MoveEnemy(float time);

    };

    class TankTreads {
    public:
        Mesh* mesh;

        float rotationAngle = 0;
        glm::vec3 coord = glm::vec3(0);  // current coords of center of bottom face
        glm::vec3 spawnCoord = glm::vec3(0);  // random spawn coords for enemy tanks


        Mesh* CreateTreads(const std::string& name,
                           glm::vec3 center,
                           int tankID);

        glm::mat4 MoveW(float time);

        glm::mat4 MoveS(float time);

        glm::mat4 RotateA(float time);

        glm::mat4 RotateD(float time);

        glm::mat4 Stay();

        glm::mat4 MoveEnemy(float time);

    };

    class TankCannon {
    public:
        Mesh* mesh;

        float rotationAngle = 0;
        glm::vec3 coord = glm::vec3(0);  // current coords of center of cylinder
        glm::vec3 spawnCoord = glm::vec3(0);  // random spawn coords for enemy tanks


        Mesh* CreateCannon(const std::string& name,
                           glm::vec3 center,
                           int tankID);

        glm::mat4 MoveW(float time);

        glm::mat4 MoveS(float time);

        glm::mat4 RotateA(float time);

        glm::mat4 RotateD(float time);

        glm::mat4 Stay();

        glm::mat4 MoveEnemy(float time);

        class Projectile {
        public:
            Mesh* projectileMesh;
            // glm::vec3 projectileSpawn = glm::vec3(0);  // spawned coords of projectile
            float projectileAngle;
            std::chrono::steady_clock::time_point projectileSpawnTime;

            glm::vec3 projectileCoords = glm::vec3(0);  // current coords of projectile
        };

        float projectileSpeed = 3.0f;
        
        Mesh* CreateProjectile(const std::string& name, glm::vec3 center);
        glm::mat4 MoveProjectile(float time, Projectile &projectile);
        std::vector<Projectile> projectiles;

    };

    // tank-component objects
    TankBody tankBody;
    TankTurret tankTurret;
    TankTreads tankTreads;
    TankCannon tankCannon;
};
