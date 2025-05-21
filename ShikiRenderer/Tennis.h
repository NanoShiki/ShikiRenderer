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
	float w; //角速度
	float I; //转动惯量. 球的转动惯量为2/5 * m * r^2.

	//台球半径比现实大2.5倍, 质量应为2.5^3倍.
	Tennis(const glm::vec3& pos) : 
		radius(0.29f * 0.5f), m(0.16f * 2.5f * 2.5f * 2.5f), v(glm::vec3(0.0f)),
		a(glm::vec3(0.0f)), position(pos), w(0.0f), I(2.0f / 5.0f * m * radius * radius)
	{ };
	Tennis() = default;
	Tennis(const Tennis&) = default;
	~Tennis() = default;

	float getRadius() const { return radius; }

	float getM() const { return m; }

	void collision_and_update(Tennis& another);
};

#endif 
