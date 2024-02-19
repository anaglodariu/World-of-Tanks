#pragma once

#include "utils/glm_utils.h"
#include "lab_m1/Tema2/projectile.h"
#include <iostream>

namespace tank
{
	class Tank
	{
	public:
		Tank()
		{
			tunPos = glm::vec3(-0.000273f, 0.532353f, -0.305598f);
			turelaPos = glm::vec3(0.000874f, 0.54406f, 0.18204f);
			senilePos = glm::vec3(-0.001839f, 0.124941f, 0.171678f);
			corpPos = glm::vec3(-0.001344, 0.324175, 0.186592);

			colourTunSenile = glm::vec3(0.38f, 0.18f, 0.345f);
			colourCorp = glm::vec3(0.522f, 0.063f, 0.439f);
			colourTurela = glm::vec3(0.929f, 0.478f, 0.847f);

			tankRotation = 0;
			translateTank = glm::vec3(0, 0, 0);

			radius = 0.82456f;
			// initialise the projectiles
			projectiles = std::list<projectile::Projectile>();
			hp = 0;
			score = 0;
			destroyed = false;
			turelaRotation = 0;
			killed = 0;
			tankInProximity = false;
			player = true; // true = player, false = enemy
			timeSinceEnemyShot = 1.0f;
			
		}

		~Tank()
		{ }

		bool collisionProjectile(projectile::Projectile* projectile) {
			//std::cout << projectile->positionTank.x << " " << translateTank.x << std::endl;
			//std::cout << projectile->positionTank.y << " " << translateTank.y << std::endl;
			//std::cout << projectile->positionTank.z << " " << translateTank.z << std::endl;
			//exit(0);
			float distance = std::sqrt(
				(projectile->positionTank.x - translateTank.x) * (projectile->positionTank.x - translateTank.x) +
				(projectile->positionTank.z - translateTank.z) * (projectile->positionTank.z - translateTank.z) +
				(projectile->positionTank.y - translateTank.y) * (projectile->positionTank.y - translateTank.y));
			//std::cout << "distance: " << distance << std::endl;
			//std::cout << "radius: " << radius + projectile->radius << std::endl;
			
			return distance < radius + projectile->radius;
		
		}

		float CollisionTank(Tank* tank) {
			float distance = std::sqrt(
				(translateTank.x - tank->translateTank.x) * (translateTank.x - tank->translateTank.x) +
				(translateTank.z - tank->translateTank.z) * (translateTank.z - tank->translateTank.z) +
				(translateTank.y - tank->translateTank.y) * (translateTank.y - tank->translateTank.y));
			return distance;
			//return distance < radius + tank->radius;
		}

		bool InProximity(Tank* tank) {
			// vector de directie intre tancuri
			//glm::vec3 direction = glm::normalize(translateTank - tank->translateTank);
			// calculate distance between tanks
			float distance = CollisionTank(tank);
			return distance < 3.0f;
		}

	public:
		float tankRotation;
		glm::vec3 translateTank;

		glm::vec3 tunPos;
		glm::vec3 turelaPos;
		glm::vec3 senilePos;
		glm::vec3 corpPos;
		float radius;

		glm::vec3 colourTunSenile;
		glm::vec3 colourCorp;
		glm::vec3 colourTurela;

		std::list<projectile::Projectile> projectiles;
		float turelaRotation;

		int hp;
		int score = 0;
		int killed = 0;
		bool destroyed = false;
		bool tankInProximity = false;
		bool player = true;
		float timeSinceEnemyShot = 1.0f;
	};
}