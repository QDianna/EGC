#pragma once

#include "star.h"
#include "hexagon.h"
#include "romb.h"
#include "square.h"

#include "components/simple_scene.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        // initial window resolution
        float resx, resy;

        // objects modelation matrix
        glm::mat3 modelMatrix;

        // mouse coords transformation matrix for potential window resize
        glm::mat3 visMatrix = glm::mat3(1);

        // coords for the corner of the window
        glm::vec3 corner;

        glm::vec3 gridLeftBottom;
        glm::vec3 healthRectangleLeftBottom;
        glm::vec3 guiSquareLeftBottom;
        glm::vec3 healthSquareLeftBottom;

        glm::vec3 squareCentersX;
        glm::vec3 squareCentersY;

        glm::vec4 guiSquareCentersX;
        float guiSquareCentersY;
        
        float gridSize = 300.0f;
        float squareSize = gridSize / 3.0f;
        float squareSpacing = 30;

        float rectangleLength = gridSize + 2 * squareSpacing;
        float rectangleWidth = squareSize / 2;

        float hexaRadius = squareSize * 0.4f;
        float starRadius = squareSize * 0.4f;
        float rombLength = squareSize * 0.4f;

        int lives = 3;
        int resources = 5;

        std::vector<Square> gridSquares;
        std::vector<Square> blackContourSquares;

        std::vector<Hexagon> spawnedHexas;
        std::vector<Hexagon> despawningHexas;

        std::vector<Romb> spawnedRombs;
        std::vector<Romb> despawningRombs;

        std::vector<Star> spawnedStars;
        std::vector<Star> spawnedGuiStars;

        Square selectedGuiSquare;
        bool selecting = false;
        float selectingX, selectingY;

    };
}   // namespace m1
