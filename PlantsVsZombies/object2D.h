#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"

// Tema1

namespace object2D
{
     glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
     glm::vec3 gGlow(0.976f, 0.949f, 0.584f);

    Mesh* CreateSquare(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color,
        bool fill = true);

    Mesh* CreateCircle(
        const std::string& name,
        glm::vec3 center,
        float radius,
        glm::vec3 color,
        int numSegments,
        bool fill = true);

    Mesh* CreateRectangle(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float width,
        float length,
        glm::vec3 color,
        bool fill = true);

    Mesh* CreateStar(
        const std::string& name,
        glm::vec3 center,
        float radius,
        glm::vec3 color,
        bool fill = true);

    Mesh* CreateRomb(
        const std::string& name,
        glm::vec3 center,
        float length,
        glm::vec3 color,
        bool fill = true);

}
