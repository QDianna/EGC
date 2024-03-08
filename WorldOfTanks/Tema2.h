#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "Tank.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented_Tema2::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        int projectionType;
        float left, right, bottom, top;
        float zNear, zFar, fov;

        Tank myTank;
        std::vector<Tank> enemyTanks;

        Tank::TankBody tankBody;
        Tank::TankTurret tankTurret;
        Tank::TankTreads tankTreads;
        Tank::TankCannon tankCannon;

        bool movingW = false;
        bool rotatingA = false;
        bool movingS = false;
        bool rotatingD = false;

        bool firing = false;
        bool gameRunning = true;
        bool setCamera = true;
        float cameraAngle;
        float cameraPitch;

        Mesh* XOZplane;

        class Building {
        public:
            float length;
            float width;
            float height;
            float translateX;
            float translateZ;

            Mesh* buildingMesh;

            Mesh* CreateBuilding(const std::string& name,
                                 glm::vec3 center);
        };

        std::vector<Building> buildings;
    };
}
