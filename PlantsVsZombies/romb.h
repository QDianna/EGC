#pragma once
#include <string>
#include "star.h"
#include "hexagon.h"

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"


    class Romb
    {
    public:
        Romb();
        ~Romb();

        Mesh* CreateRomb(
            const std::string& name,
            glm::vec3 center,
            float length,
            glm::vec3 color,
            bool fill = true);

        glm::mat3 Romb::Spawn(
            float x,
            float y);

        float centerX, centerY;
        float currentX, currentY;
        float radius;
        float scaleX, scaleY;
        Mesh* mesh;
        bool fireing = false;

        int color;
        Star projectile;

        int gridSquareNumber;

    };
