#ifndef TENNISTABLE_H
#define TENNISTABLE_H

#include <glm/glm.hpp>
#include <vector>
#include "Tennis.h"

class TennisTable {
private:
	const float width, height;
	const glm::vec3 center;

public:
	static float u; //Ä¦²ÁÏµÊý

	TennisTable(const glm::vec3& pos):
		width(13.68f), height(6.5f), center(pos){}
	TennisTable() = default;
	~TennisTable() = default;
	TennisTable(const TennisTable&) = default;

	bool Collision_detection(const Tennis& tennis);

	glm::vec3 normal_of_border(const Tennis& tennis);

	void updateTennis(Tennis& tennis);

};

#endif // !TENNISTABLE_H
