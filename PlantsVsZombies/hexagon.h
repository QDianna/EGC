#pragma once
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"


	class Hexagon
	{
    public:
        Hexagon();

        Hexagon(float sX, float sY);
        
        ~Hexagon();


        Mesh* CreateCircle(
            const std::string& name,
            glm::vec3 center,
            float radius,
            glm::vec3 color,
            int numSegments,
            bool fill = true);

        Mesh* Hexagon::CreateInsideCircle(
            const std::string& name,
            glm::vec3 center,
            float radius,
            glm::vec3 color,
            int numSegments,
            bool fill = true);

        glm::mat3 Hexagon::Spawn(
            float x,
            float y);

        void Hexagon::Update(
            float time);

        int color;
        float radius;

        float centerX, centerY;
        float spawnX, spawnY;
        float currentX, currentY;

        float translateX;
        float scaleX, scaleY;
        float speed;

        int life;
        Mesh* mesh;
        Mesh* intMesh;
	};
