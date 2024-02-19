#pragma once

#include "utils/glm_utils.h"
#include "projectile.h"

namespace building
{
	class Building
	{
	public:
		Building() {
			radius = 0.0f;
			position = glm::vec3(0.0f);
			scale = glm::vec3(0.0f);
			colour = glm::vec3(1.0f, 0.0f, 1.0f);
		}

		~Building() {

		}

		bool collisionProjectile(projectile::Projectile* projectile) {
			// get building's closest point to projectile center
			float x = std::max(position.x - scale.x / 2, std::min(projectile->positionTank.x, position.x + scale.x / 2));
			float y = std::max(position.y - scale.y / 2, std::min(projectile->positionTank.y, position.y + scale.y / 2));
			float z = std::max(position.z - scale.z / 2, std::min(projectile->positionTank.z, position.z + scale.z / 2));

			float distance = glm::sqrt(
				(x - projectile->positionTank.x) * (x - projectile->positionTank.x) +
				(y - projectile->positionTank.y) * (y - projectile->positionTank.y) +
				(z - projectile->positionTank.z) * (z - projectile->positionTank.z));

			return distance < projectile->radius;

		}

		float collisionTank(tank::Tank* tank) {
			// get building's closest point to projectile center
			//std::cout << "tank position: " << tank->translateTank.x << " " << tank->translateTank.y << " " << tank->translateTank.z << std::endl;
			//std::cout << "building position: " << position.x << " " << position.y << " " << position.z << std::endl;
			float x = std::max(position.x - scale.x / 2, std::min(tank->translateTank.x, position.x + scale.x / 2));
			float y = std::max(position.y - scale.y / 2, std::min(tank->translateTank.y, position.y + scale.y / 2));
			float z = std::max(position.z - scale.z / 2, std::min(tank->translateTank.z, position.z + scale.z / 2));

			float distance = glm::sqrt(
				(x - tank->translateTank.x) * (x - tank->translateTank.x) +
				(y - tank->translateTank.y) * (y - tank->translateTank.y) +
				(z - tank->translateTank.z) * (z - tank->translateTank.z));

			return distance;
		}

	public:

		float radius;
		glm::vec3 position;
		glm::vec3 colour;
		glm::vec3 scale;
	};
}