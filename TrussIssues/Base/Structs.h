#pragma once
#include <glm\glm.hpp>
struct Transform
{
	GLfloat mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 scale;
};