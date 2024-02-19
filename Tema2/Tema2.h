#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
//#include "components/transform.h"
#include "lab_m1/Tema2/transforms3D.h"
#include "lab_m1/Tema2/tank.h"
#include "lab_m1/Tema2/stateMachine.h"
#include "lab_m1/Tema2/building.h"


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

        //void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), const int hp = 0);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderTank(tank::Tank* tank);

    protected:
        implementedCamera::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        glm::vec3 tunPos;
        glm::vec3 turelaPos;
        glm::vec3 senilePos;
        glm::vec3 corpPos;

        glm::vec3 colourTunSenile;
        glm::vec3 colourCorp;
        glm::vec3 colourTurela;

        // TODO(student): If you need any other class variables, define them here.
        float fov;
        float left, right, bottom, top;

        float speed;
        float tankRotation;
        glm::vec3 translateTank;
        float rotateCamera;
        glm::vec3 translateProjectile;
        glm::vec3 directionProjectile;
        float rotationProjectile;
        bool pressLeft = false;
        glm::mat4 modelMatrixTun;

        // get a tank object
        tank::Tank* tank;

        float TimeSinceLastProjectile = 1.0f;
        std::list<tank::Tank> enemies;
        float TimePassedSinceChange = 0.0f;

        //stateMachine::MovementState movementState = stateMachine::MovementState::GoingForward;
        // a StateMachine object
        //stateMachine::StateMachine* state;
        // a list of states
        std::list<stateMachine::StateMachine> states;
        int nrOfEnemies = 5;
        int nrOfBuildings = 60;
        float ExitTime = 0.0f;

        // list of buildings
        std::list<building::Building> buildings;
        bool mouseRightPressed = false;
        
    };
}   // namespace m1
