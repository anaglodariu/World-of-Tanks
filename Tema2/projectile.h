#pragma once

#include "utils/glm_utils.h"

namespace projectile
{
	class Projectile
	{
	public:
		Projectile()
		{
			translateProjectile = glm::vec3(0, 0, 0);
			positionTank = glm::vec3(0, 0, 0);
			rotationTank = 0;
			rotationProjectile = 0;
			this->modelMatrixTun = modelMatrixTun;
			TimeSinceShot = 0;
			radius = 0.05f;

		}


		~Projectile()
		{ }

	public:
		glm::vec3 translateProjectile;
		float rotationProjectile;
		glm::mat4 modelMatrixTun;
		float TimeSinceShot = 0;
		float radius;
		glm::vec3 positionTank;
		float rotationTank;
	};
}