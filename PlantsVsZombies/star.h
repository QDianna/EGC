#pragma once
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"

    class Star
    {
    public:
        Star();

        Star::Star(float sX, float sY);

        ~Star();

        Mesh* CreateStar(
            const std::string& name,
            glm::vec3 center,
            float radius,
            glm::vec3 color,
            bool fill = true);

        glm::mat3 Star::Spawn(
            float x,
            float y);
        glm::mat3 Star::GuiSpawn(
            float resx,
            float resy);

        void Star::Update(
            float time
            );
        int color;
        float radius;

        float centerX, centerY;
        float spawnX, spawnY;
        float currentX, currentY;

        float translateX, translateY;
        float scaleX, scaleY;
        float speed, rotationSpeed;
        float angularStep;

        Mesh* mesh;
        glm::mat3 modelMatrix;
    };
