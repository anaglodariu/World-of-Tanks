#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implementedCamera::Camera();
    
    // initialize the position of the tank
    tunPos = glm::vec3(-0.000273f, 0.532353f, -0.305598f);
    turelaPos = glm::vec3(0.000874f, 0.54406f, 0.18204f);
    senilePos = glm::vec3(-0.001839f, 0.124941f, 0.171678f);
    corpPos = glm::vec3(-0.001344, 0.324175, 0.186592);

    camera->Set(glm::vec3(0, corpPos.y + 0.5f, 3), corpPos, glm::vec3(0, 1, 0));

    // initialize the color of the tank
    colourTunSenile = glm::vec3(0.38f, 0.18f, 0.345f);
    colourCorp = glm::vec3(0.522f, 0.063f, 0.439f);
    colourTurela = glm::vec3(0.929f, 0.478f, 0.847f);
    tankRotation = 0;
    translateTank = glm::vec3(0, 0, 0);
    translateProjectile = glm::vec3(0, 0, 0);
    directionProjectile = glm::vec3(0, 0, 0);
    rotationProjectile = 0;
    

    tank = new tank::Tank();
    // BUILDINGS
	{
		Mesh* mesh = new Mesh("building");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

    // SUPRAFATA PLANA
    {
        Mesh * mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // PROIECTIL
    {
		Mesh* mesh = new Mesh("proiectil");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/tun"), "tun.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/turela"), "turela.fbx");
        meshes[mesh->GetMeshID()] = mesh;

    }

    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/senile"), "senile.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/corp"), "corp.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Shader* shader = new Shader("MyShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
    }

    std::random_device random;
    std::mt19937_64 gen(random());
    std::uniform_real_distribution<float> posX(1.0f, 10.0f);
    std::uniform_real_distribution<float> posZ(-10.0f, -1.0f);

    std::uniform_real_distribution<float> scaleX(0.5f, 4.5f);
    std::uniform_real_distribution<float> scaleY(0.5f, 5.0f);
    std::uniform_real_distribution<float> scaleZ(0.5f, 1.5f);


    std::uniform_real_distribution<float> buildingPosX(-20.0f, 20.0f);
    std::uniform_real_distribution<float> buildingPosZ(-20.0f, 20.0f);
    std::uniform_real_distribution<float> buildingColor(0.0f, 1.0f);

    // create buildings
    for (int i = 0; i < nrOfBuildings; i++) {
        // create a new building
        building::Building* building = new building::Building();
        building->scale = glm::vec3(scaleX(gen), scaleY(gen), scaleZ(gen));
        building->position = glm::vec3(buildingPosX(gen), tank->senilePos.y - tank->corpPos.y, buildingPosZ(gen));
        building->colour += glm::vec3(-buildingColor(gen), buildingColor(gen), -buildingColor(gen));
        // add it to the list of buildings
        buildings.push_back(*building);
    }

    // create enemies
    for (int i = 0; i < nrOfEnemies; i++) {
        // create a new enemy
        tank::Tank* enemy = new tank::Tank();
        enemy->colourTunSenile = glm::vec3(0.039f, 0.451f, 0.424f);
        enemy->colourCorp = glm::vec3(0.769f, 0.969f, 0.431f);
        enemy->colourTurela = glm::vec3(0.71f, 1, 0.839f);
        enemy->translateTank = glm::vec3(posX(gen), 0, posZ(gen));
        enemy->player = false; // enemy

        //create stateMachine object
        stateMachine::StateMachine* stateMachine = new stateMachine::StateMachine();
        // add it to states list
        states.push_back(*stateMachine);

        enemies.push_back(*enemy);
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.667f, 0.871f, 0.969f, 0.72f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderTank(tank::Tank* tank) {

    glm::mat4 modelMatrixTun = glm::mat4(1);
    glm::vec3 translateTank = tank->translateTank + tank->tunPos;
    float tankRotation = tank->turelaRotation;

    // SENILE
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transforms3D::Translate(-tank->corpPos.x, -tank->corpPos.y, -tank->corpPos.z);
        glm::vec3 position = tank->senilePos + tank->translateTank;
        modelMatrix *= transforms3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transforms3D::RotateOY(tank->tankRotation);
        modelMatrix *= transforms3D::RotateOY(RADIANS(180.0f));
        modelMatrix *= transforms3D::RotateOX(RADIANS(-90.0f));

        RenderSimpleMesh(meshes["senile"], shaders["MyShader"], modelMatrix, tank->colourTunSenile);
    }

    // CORP
    {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transforms3D::Translate(-tank->corpPos.x, -tank->corpPos.y, -tank->corpPos.z);
        glm::vec3 position = tank->corpPos + tank->translateTank;
        modelMatrix *= transforms3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transforms3D::RotateOY(tank->tankRotation);
        modelMatrix *= transforms3D::RotateOY(RADIANS(180.0f));
        modelMatrix *= transforms3D::RotateOX(RADIANS(-90.0f));

		RenderSimpleMesh(meshes["corp"], shaders["MyShader"], modelMatrix, tank->colourCorp, tank->hp);
	}

    // TURELA
    {

        if (tank->player == false && tank->destroyed == true) 
            tank->turelaRotation = tank->tankRotation;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transforms3D::Translate(-tank->corpPos.x, -tank->corpPos.y, -tank->corpPos.z);
		glm::vec3 position = tank->turelaPos + tank->translateTank;
		modelMatrix *= transforms3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transforms3D::RotateOY(tank->turelaRotation);
		modelMatrix *= transforms3D::RotateOY(RADIANS(180.0f));
		modelMatrix *= transforms3D::RotateOX(RADIANS(-90.0f));

		RenderSimpleMesh(meshes["turela"], shaders["MyShader"], modelMatrix, tank->colourTurela, tank->hp);
	}

    // TUN
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transforms3D::Translate(-tank->corpPos.x, -tank->corpPos.y, -tank->corpPos.z);
        glm::vec3 position = tank->tunPos + tank->translateTank;
        modelMatrix *= transforms3D::Translate(position.x, position.y, position.z);

        glm::vec3 rotation = tank->corpPos - tank->tunPos;

        modelMatrix *= transforms3D::Translate(rotation.x, rotation.y, rotation.z);
        modelMatrix *= transforms3D::RotateOY(tank->turelaRotation);
        modelMatrix *= transforms3D::Translate(-rotation.x, -rotation.y, -rotation.z);

        modelMatrix *= transforms3D::RotateOY(RADIANS(180.0f));
        modelMatrix *= transforms3D::RotateOX(RADIANS(-90.0f));

        RenderSimpleMesh(meshes["tun"], shaders["MyShader"], modelMatrix, tank->colourTunSenile, tank->hp);

    }

    // PROIECTIL
    {
        if (TimeSinceLastProjectile < 1.0f) 
            pressLeft = false;

        if (pressLeft && TimeSinceLastProjectile > 1.0f && tank->player == true) {
            
            projectile::Projectile* projectile = new projectile::Projectile();
            projectile->positionTank = translateTank;
            // add to the position so that the projectile shoots from the end of the tun
            projectile->positionTank -= glm::vec3(sin(M_PI - tankRotation), 0, -cos(M_PI - tankRotation)) * (- tank->tunPos.z + tank->corpPos.z + 0.1f);

            projectile->rotationTank = tankRotation;
            tank->projectiles.push_back(*projectile);

            pressLeft = false;
            TimeSinceLastProjectile = 0;
        }

        

        if (tank->timeSinceEnemyShot > 1.0f && tank->player == false && tank->tankInProximity == true && tank->destroyed == false) {
			// create projectile
			
			projectile::Projectile* projectile = new projectile::Projectile();
			projectile->positionTank = translateTank;
            projectile->positionTank -= glm::vec3(sin(M_PI - tankRotation), 0, -cos(M_PI - tankRotation)) * (-tank->tunPos.z + tank->corpPos.z + 0.1f);
			projectile->rotationTank = tankRotation;
			tank->projectiles.push_back(*projectile);
			tank->timeSinceEnemyShot = 0;
        }

    }

}


void Tema2::Update(float deltaTimeSeconds)
{
    TimePassedSinceChange += deltaTimeSeconds;
    TimeSinceLastProjectile += deltaTimeSeconds;
    ExitTime += deltaTimeSeconds;
    

    if (ExitTime > 60) {
        cout << "TIME IS UP" << endl;
        cout << "SCORE: " << tank->score << endl;
        cout << "ENEMIES KILLED: " << tank->killed << endl;
		cout << "EXITING" << endl;
		exit(0);
    }

    RenderTank(tank);
    

    // render projectiles
    for (auto it = tank->projectiles.begin(); it != tank->projectiles.end();) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transforms3D::Translate(-tank->corpPos.x, -tank->corpPos.y, -tank->corpPos.z);
        modelMatrix *= transforms3D::Translate(it->positionTank.x, it->positionTank.y, it->positionTank.z);
        glm::vec3 rotation = tank->corpPos - tank->tunPos;
        modelMatrix *= transforms3D::Translate(rotation.x, rotation.y, rotation.z);
        modelMatrix *= transforms3D::RotateOY(it->rotationTank);
        modelMatrix *= transforms3D::Translate(-rotation.x, -rotation.y, -rotation.z);
        modelMatrix *= transforms3D::Scale(0.1f, 0.1f, 0.1f);

        glm::vec3 direction = glm::vec3(sin(M_PI - it->rotationTank), 0, -cos(M_PI - it->rotationTank));
        it->positionTank -= glm::normalize(direction) * deltaTimeSeconds * 2.0f;


        if (it->TimeSinceShot > 15.0f) {
            it = tank->projectiles.erase(it);
            continue;
        }
        it->TimeSinceShot += deltaTimeSeconds;
        
        RenderSimpleMesh(meshes["proiectil"], shaders["MyShader"], modelMatrix, glm::vec3(0.9, 0.6, 0.9));
        it++;

	}


    // SUPRAFATA PLANA
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transforms3D::Translate(0, tank->senilePos.y - 0.4f, 0);
        modelMatrix *= transforms3D::Scale(1.0f, 0.25f, 1.0f);
        RenderSimpleMesh(meshes["plane"], shaders["MyShader"], modelMatrix, glm::vec3(0.431, 0.157, 0.035));
    }



    if (TimePassedSinceChange > 20) {
        TimePassedSinceChange = 0;

    }

    // go through states
    for (auto it = states.begin(); it != states.end(); it++) {
        it->TimeInCurrentState += deltaTimeSeconds;
        if (it->TimeInCurrentState > 3) {
			it->TimeInCurrentState = 0;
			it->currentState = it->GetNextMovementState(it->currentState);
		}
    }

    // enemy turela points to tank when it's close and shoots
    for (auto itE = enemies.begin(); itE != enemies.end();) {
        tank::Tank* enemy = &(*itE);
        if (tank->InProximity(enemy)) {
            enemy->tankInProximity = true;
            // get direction vector between tanks
            glm::vec3 direction = -tank->translateTank + enemy->translateTank;
            direction = glm::normalize(direction);
            // turelaRotation goes into the direction of direction vector
            enemy->turelaRotation = atan2(direction.x, direction.z);
            enemy->timeSinceEnemyShot += deltaTimeSeconds;
            


        } else {
			enemy->tankInProximity = false;
            enemy->turelaRotation = enemy->tankRotation;
        }


        for (auto itP = enemy->projectiles.begin(); itP != enemy->projectiles.end();) {
            projectile::Projectile* projectile = &(*itP);

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transforms3D::Translate(-enemy->corpPos.x, -enemy->corpPos.y, -enemy->corpPos.z);
            modelMatrix *= transforms3D::Translate(projectile->positionTank.x, projectile->positionTank.y, projectile->positionTank.z);
            glm::vec3 rotation = enemy->corpPos - enemy->tunPos;
            modelMatrix *= transforms3D::Translate(rotation.x, rotation.y, rotation.z);
            modelMatrix *= transforms3D::RotateOY(projectile->rotationTank);
            modelMatrix *= transforms3D::Translate(-rotation.x, -rotation.y, -rotation.z);
            modelMatrix *= transforms3D::Scale(0.1f, 0.1f, 0.1f);

            glm::vec3 direction = glm::vec3(sin(M_PI - projectile->rotationTank), 0, -cos(M_PI - projectile->rotationTank));
            projectile->positionTank -= glm::normalize(direction) * deltaTimeSeconds * 2.0f;
            projectile->TimeSinceShot += deltaTimeSeconds;

            if (tank->collisionProjectile(projectile)) {
                itP = enemy->projectiles.erase(itP);
                tank->hp++;
                if (tank->hp > 3) {
					cout << "PLAYER DIED" << endl;
					cout << "SCORE: " << tank->score << endl;
					cout << "ENEMIES KILLED: " << tank->killed << endl;
					cout << "EXITING" << endl;
					exit(0);
				}
                continue;
            }

            if (projectile->TimeSinceShot > 5.0f) {
				itP = enemy->projectiles.erase(itP);
				continue;
			}

            RenderSimpleMesh(meshes["proiectil"], shaders["MyShader"], modelMatrix, glm::vec3(0.9, 0.6, 0.9));
            itP++;
        }

        itE++;
    }

    // iterator for states list
    auto itStates = states.begin();

    // render enemies
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        if (it->destroyed == false) {
            stateMachine::MovementState movementState = itStates->currentState;
            glm::vec3 direction = glm::vec3(1);
            switch (movementState)
            {
            case stateMachine::MovementState::GoingForward:
                direction = glm::vec3(sin(M_PI - it->tankRotation), 0, -cos(M_PI - it->tankRotation));
                it->translateTank -= glm::normalize(direction) * deltaTimeSeconds;

                break;
            case stateMachine::MovementState::GoingBackward:
                direction = glm::vec3(sin(M_PI - it->tankRotation), 0, -cos(M_PI - it->tankRotation));
                it->translateTank += glm::normalize(direction) * deltaTimeSeconds;
                break;
            case stateMachine::MovementState::InPlaceRotationLeft:
                it->tankRotation -= deltaTimeSeconds;
                if (it->tankRotation < 0) {
                    it->tankRotation = 2 * M_PI;
                    
                }
                break;
            case stateMachine::MovementState::InPlaceRotationRight:
                it->tankRotation += deltaTimeSeconds;
                if (it->tankRotation > 2 * M_PI) {
                    it->tankRotation = 0;
                }
                break;
            }
        }
		RenderTank(&(*it));
		itStates++;
	}



    // collision projectiles with enemies
    for (auto itE = enemies.begin(); itE != enemies.end(); itE++) {
        tank::Tank* enemy = &(*itE);
        for (auto itP = tank->projectiles.begin(); itP != tank->projectiles.end();) {
            projectile::Projectile* projectile = &(*itP);
            if (enemy->destroyed == false && enemy->collisionProjectile(projectile)) {
                itP = tank->projectiles.erase(itP);
                enemy->hp++;
                tank->score++;
                if (enemy->hp > 3) {
                    cout << "ENEMY DIED" << endl;
                    enemy->destroyed = true;
                    tank->score += 5;
                    tank->killed++;
                }
                continue;
            }
            itP++;
        }
    }



    for (auto itE = enemies.begin(); itE != enemies.end();) {
        tank::Tank* enemy = &(*itE);
        float distance = tank->CollisionTank(enemy);
        if (distance < tank->radius + enemy->radius) {
            glm::vec3 diff = tank->translateTank - enemy->translateTank;
            float P = abs(tank->radius + enemy->radius - distance);
            glm::vec3 displacement = P * glm::normalize(diff);
            displacement.y = 0;
            tank->translateTank += displacement * (7.5f) * deltaTimeSeconds;
            enemy->translateTank -= displacement * (7.5f) * deltaTimeSeconds;
        }
		itE++;
	}
    

    // check collision between enemies
    for (auto itE = enemies.begin(); itE != enemies.end();) {
		tank::Tank* enemy1 = &(*itE);
		for (auto itE1 = enemies.begin(); itE1 != enemies.end();) {
			tank::Tank* enemy2 = &(*itE1);
			if (enemy1 != enemy2) {
				float distance = enemy1->CollisionTank(enemy2);
				if (distance < enemy1->radius + enemy2->radius) {
					glm::vec3 diff = enemy1->translateTank - enemy2->translateTank;
					float P = abs(enemy1->radius + enemy2->radius - distance);
					glm::vec3 displacement = P * glm::normalize(diff);
                    displacement.y = 0;
					enemy1->translateTank += displacement * (7.5f) * deltaTimeSeconds;
					enemy2->translateTank -= displacement * (7.5f) * deltaTimeSeconds;
				}
			}
			itE1++;
		}
		itE++;
	}


    // render buildings
    for (auto it = buildings.begin(); it != buildings.end(); it++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transforms3D::Translate(it->position.x, it->position.y, it->position.z);
		modelMatrix *= transforms3D::Scale(it->scale.x, it->scale.y, it->scale.z);
        RenderSimpleMesh(meshes["building"], shaders["MyShader"], modelMatrix, it->colour);
	}

    // collision buildings projectiles
    for (auto itB = buildings.begin(); itB != buildings.end(); itB++) {
        building::Building* building = &(*itB);
        for (auto itP = tank->projectiles.begin(); itP != tank->projectiles.end();) {
            projectile::Projectile* projectile = &(*itP);
            if (building->collisionProjectile(projectile)) {
                itP = tank->projectiles.erase(itP);
                continue;
            }
            itP++;
        }
    }

    // collision buildings tank
    for (auto itB = buildings.begin(); itB != buildings.end(); itB++) {
		building::Building* building = &(*itB);
		float distance = building->collisionTank(tank);
		if (distance < tank->radius) {
            glm::vec3 diff = tank->translateTank - building->position;
            float P = abs(tank->radius - distance);
            glm::vec3 displacement = P * glm::normalize(diff);
            displacement.y = 0;
			tank->translateTank += displacement * (10.5f) * deltaTimeSeconds;
		}
	}

    // collision buildings enemy
    for (auto itB = buildings.begin(); itB != buildings.end(); itB++) {
		building::Building* building = &(*itB);
        for (auto itE = enemies.begin(); itE != enemies.end(); itE++) {
            tank::Tank* enemy = &(*itE);
            float distance = building->collisionTank(enemy);
            if (distance < enemy->radius) {
				glm::vec3 diff = enemy->translateTank - building->position;
				float P = abs(enemy->radius - distance);
				glm::vec3 displacement = P * glm::normalize(diff);
				displacement.y = 0;
				enemy->translateTank += displacement * (10.5f) * deltaTimeSeconds;
			}

        }
		
    }

    // collision enemy projectiles with other enemies
    for (auto itE1 = enemies.begin(); itE1 != enemies.end(); itE1++) {
		tank::Tank* enemy1 = &(*itE1);
        for (auto itE2 = enemies.begin(); itE2 != enemies.end(); itE2++) {
			tank::Tank* enemy2 = &(*itE2);
            if (enemy1 != enemy2) {
                for (auto itP = enemy2->projectiles.begin(); itP != enemy2->projectiles.end();) {
					projectile::Projectile* projectile = &(*itP);
                    if (enemy1->collisionProjectile(projectile)) {
						itP = enemy2->projectiles.erase(itP);
						continue;
					}
					itP++;
				}
			}
			
		}
		
	}

    // collision enemy projectiles with buildings
    for (auto itB = buildings.begin(); itB != buildings.end(); itB++) {
        building::Building* building = &(*itB);
        for (auto itE = enemies.begin(); itE != enemies.end(); itE++) {
            tank::Tank* enemy = &(*itE);
            for (auto itP = enemy->projectiles.begin(); itP != enemy->projectiles.end();) {
                projectile::Projectile* projectile = &(*itP);
                if (building->collisionProjectile(projectile)) {
                    itP = enemy->projectiles.erase(itP);
                    continue;
                }
                itP++;
            }
        }
    }



    camera->SetDependingOnTank(tank->corpPos + tank->translateTank);
}


void Tema2::FrameEnd()
{
}



void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int hp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;


    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // set material property uniform (object color)
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // set hp uniform
    glUniform1i(glGetUniformLocation(shader->program, "hp"), hp);

    // TODO(student): Get shader location for uniform mat4 "Model"

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));


    // TODO(student): Set shader uniform "Projection" to projectionMatrix
   // glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
       
        glm::vec3 direction = glm::vec3(sin(M_PI - tank->tankRotation), 0, -cos(M_PI - tank->tankRotation));
       
        tank->translateTank -= glm::normalize(direction) * deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_S)) {
		
		glm::vec3 direction = glm::vec3(sin(M_PI - tank->tankRotation), 0, -cos(M_PI - tank->tankRotation));
		
        tank->translateTank += glm::normalize(direction) * deltaTime;

       

	}
    if (window->KeyHold(GLFW_KEY_A)) {
        // rotate the tank to the left
        float initialRotation = tank->tankRotation;
   
        tank->tankRotation += deltaTime * 2;
        float finalRotation = tank->tankRotation;
        camera->RotateThirdPerson_OY((finalRotation - initialRotation));
       
        float cameraRotation = deltaTime * 1.5;
        if (cameraRotation > 2 * M_PI) {
            cameraRotation = 0;
        }

    
        if (tank->tankRotation > 2 * M_PI) {
            tank->tankRotation = 0;
		}
        
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        float initialRotation = tank->tankRotation;
        tank->tankRotation -= deltaTime * 2;
        float finalRotation = tank->tankRotation;

        camera->RotateThirdPerson_OY((finalRotation - initialRotation));
        float cameraRotation = -deltaTime * 1.5;
        if (cameraRotation < 0) {
			cameraRotation += 2 * M_PI;
		}

        

        
        if (tank->tankRotation < 0) {
            tank->tankRotation = 2 * M_PI;
        }
        
	}
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        mouseRightPressed = true;
        
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    
     
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (mouseRightPressed == false) {
        // calculate the rotation of the tank depending on the mouse movement
        mouseY = window->GetResolution().y - mouseY;
        float sensitivity = 0.009f;
        float rotationDelta = sensitivity * deltaX;

        glm::vec2 resolution = window->GetResolution();
        if (mouseX > resolution.x / 2) {
            tank->turelaRotation += rotationDelta;
        }
        else {
            tank->turelaRotation += rotationDelta;
        }

        if (tank->turelaRotation < 0) {
            tank->turelaRotation += 2 * M_PI;
        }

        else if (tank->turelaRotation > 2 * M_PI) {
            tank->turelaRotation -= 2 * M_PI;
        }
    } else if (mouseRightPressed == true) {
		// calculate the rotation of the camera depending on the mouse movement
		mouseY = window->GetResolution().y - mouseY;
		float sensitivity = 0.009f;
		float rotationDelta = sensitivity * deltaX;
        
		camera->RotateThirdPerson_OY(rotationDelta);
        camera->delta = rotationDelta;
	}
    


    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        pressLeft = true;
		
    }
    
    
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
		mouseRightPressed = false;
	}
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
