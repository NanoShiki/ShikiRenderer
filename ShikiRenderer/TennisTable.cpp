#include "TennisTable.h"

float TennisTable::u = 0.015f;

bool TennisTable::Collision_detection(const Tennis& tennis) {
	return
		std::abs(tennis.position.x - center.x) + tennis.getRadius() > width / 2.0f ||
		std::abs(tennis.position.z - center.z) + tennis.getRadius() > height / 2.0f;
}

glm::vec3 TennisTable::normal_of_border(const Tennis& tennis) {
	if (std::abs(tennis.position.x - center.x) + tennis.getRadius() > width / 2.0f) {
		if (tennis.position.x - center.x > 0) return glm::vec3(-1.0f, 0.0f, 0.0f);
		else return glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else {
		if (tennis.position.z - center.z > 0) return glm::vec3(0.0f, 0.0f, -1.0f);
		else return glm::vec3(0.0f, 0.0f, 1.0f);
	}
}

void TennisTable::updateTennis(Tennis& tennis) {
	bool collision = Collision_detection(tennis);
	if (collision) {
		//与桌面边缘碰撞, 反弹.
		glm::vec3 normal = normal_of_border(tennis);

		//法向速度, 切向速度.
		glm::vec3 vn = glm::dot(tennis.v, -normal) * (-normal);
		glm::vec3 vt = tennis.v - vn;

		tennis.v = -vn + vt;
	}
}

