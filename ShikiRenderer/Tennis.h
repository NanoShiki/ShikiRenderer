#ifndef TENNIS_H
#define TENNIS_H

#include <glm/glm.hpp>
#include <iostream>

class Tennis {
private:
	const float radius, m;
public:
	static float start_speed;
	glm::vec3 position, v, a;

	Tennis(const glm::vec3& pos) : 
		radius(0.29f * 0.5f), m(1.0f), v(glm::vec3(0.0f)), a(glm::vec3(0.0f)), position(pos) {};
	Tennis() = default;
	Tennis(const Tennis&) = default;
	~Tennis() = default;

	float getRadius() const { return radius; }

	float getM() const { return m; }

	void collision_and_update(Tennis& another);
};

#endif 
