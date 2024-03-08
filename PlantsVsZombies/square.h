#pragma once
#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "components/simple_scene.h"


    class Square
    {
    public:
        Square();
        ~Square();

        glm::mat3 Square::gridPlacement(
            int row,
            int col,
            float gridBottomLeftX,
            float gridBottomLeftY);

        Mesh* CreateSquare(
            const std::string& name,
            glm::vec3 leftBottomCorner,
            float length,
            glm::vec3 color,
            bool fill = true);

        float length = 100;
        Mesh* mesh;

        // bottom left corner coordinates
        float translateX, translateY;

        // for squares in grid
        float spacing = 30;

        // color of the romb inside it (for the contour squares)
        int colorOfRomb;

        // price of the romb inside it (for the contour squares)
        int priceOfRomb;

        // bool var to know if i have a romb on a grid square or not
        bool free = true;
        

    };
