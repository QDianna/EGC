#pragma once
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"


    class Rectangle
    {
    public:
        Rectangle();
        ~Rectangle();

        Mesh* CreateRectangle(
            const std::string& name,
            glm::vec3 leftBottomCorner,
            float width,
            float length,
            glm::vec3 color,
            bool fill = true);
        
        float width = 50;
        float length = 360;
        Mesh* mesh;
    };
